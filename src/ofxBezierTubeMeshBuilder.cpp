//
//  ofxBezierTubeMeshBuilder.cpp
//  example-ui
//
//  Created by Fred Rodrigues on 22/11/2023.
//

#include "ofxBezierTubeMeshBuilder.h"

ofxBezierTubeMeshBuilder::ofxBezierTubeMeshBuilder(ofxBezierEditorSettings& settings) : settings(settings){
    // Initialization
}

ofxBezierTubeMeshBuilder::~ofxBezierTubeMeshBuilder() {
    // Cleanup
}
ofVboMesh ofxBezierTubeMeshBuilder::getTubeMeshFromPolyline(ofPolyline inPoly){
    generateTubeMeshFromPolyline(inPoly);
    return tubeMesh;
}

void ofxBezierTubeMeshBuilder::generateTubeMeshFromPolyline(ofPolyline inPoly){
    if(inPoly.size() > 1){
        
        tubeMesh.clear();
        tubeMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        ofPolyline workerLine = inPoly;
        //lets add extra points at the start and end of the line using reflection so we get good normals for the actual start and end
        glm::vec3 firstPoint = workerLine.getVertices()[0];
        glm::vec3 secondPoint = workerLine.getVertices()[1];
        glm::vec3 reflectedFirst = firstPoint - (secondPoint - firstPoint);
        
        // Reflect the second-to-last point over the last for the end of the line
        glm::vec3 lastPoint = workerLine.getVertices()[workerLine.getVertices().size() - 1];
        glm::vec3 secondLastPoint = workerLine.getVertices()[workerLine.getVertices().size() - 2];
        glm::vec3 reflectedLast = lastPoint - (secondLastPoint - lastPoint);
        
        //add the firstPoint to the start of the line
        workerLine.insertVertex(reflectedFirst, 0);
        workerLine.addVertex(reflectedLast);
        
        vector<ofVec3f> points;
        vector<ofVec3f> tangents;
        vector<ofVec3f> normals;
        
        for (int i = 1; i < workerLine.size() - 1; i++) {
            points.push_back(ofVec3f(workerLine[i].x, workerLine[i].y, 0));
            
            tangents.push_back(workerLine.getTangentAtIndex(i));
            normals.push_back(workerLine.getNormalAtIndex(i));
            
            // Add intermediate points and tangents based on precisionMultiplier
            if (i < workerLine.size() - 2 ) {
                
                for (int j = 1; j < settings.meshLengthPrecisionMultiplier; j++) {
                    float t = float(j) / float(settings.meshLengthPrecisionMultiplier);
                    ofVec3f interpolatedPoint = workerLine.getPointAtIndexInterpolated(i + t);
                    ofVec2f interpolatedTangent = workerLine.getTangentAtIndexInterpolated(i + t);
                    ofVec2f interpolatedNormal = workerLine.getNormalAtIndexInterpolated(i + t);
                    
                    points.push_back(interpolatedPoint);
                    tangents.push_back(interpolatedTangent);
                    normals.push_back(interpolatedNormal);
                }
            }
        }
        
        std::vector<float> segmentDistances;
        
        float totalLineLength = 0;
        
        
        segmentDistances.push_back(settings.tubeRadius);
        totalLineLength +=  settings.tubeRadius * 2;
        
        
        
        //get the distance between each point on the line
        for(int i = 0; i < points.size()-1; i++){
            float distance = points[i].distance(points[i+1]);
            
            segmentDistances.push_back(distance + segmentDistances.back());
            
            totalLineLength += distance;
        }
        
        vector<vector<ofVec3f>> allCircles;
        vector<vector<ofVec3f>> allCircleNormals;
        vector<vector<ofVec2f>> allCircleTexCoords;
        
        
        if(settings.roundCap){
            generateCurvedTubeCap(allCircles, allCircleNormals, allCircleTexCoords, points[0], tangents[0], normals[0], true, totalLineLength);
        }
        if(!settings.roundCap){
            generateFlatTubeCap(allCircles, allCircleNormals, allCircleTexCoords, points[0], tangents[0], normals[0], true, totalLineLength);
        }
        
        
        // Generate vertices for the circles
        for (int i = 0; i < points.size(); i++) {
            const ofVec3f &p0 = points[i];
            const ofVec3f &n0 = normals[i];
            const ofVec3f &t0 = tangents[i];
            
            vector<ofVec3f> circleVertices;
            vector<ofVec3f> circleNormals;
            vector<ofVec2f> circleTexCoords;
            
            for (int j = 0; j <= settings.tubeResolution; j++) { // Use <= to include the last point in the circle
                float p = j / static_cast<float>(settings.tubeResolution);
                float a = p * 360;
                ofVec3f v0 = n0.getRotated(a, t0) * settings.tubeRadius + p0;
                circleVertices.push_back(v0);
                
                ofVec3f normal = v0 - p0;
                normal.normalize();
                circleNormals.push_back(normal);
                
                ofVec2f texCoord(a / 360.0, segmentDistances[i] / totalLineLength);
                circleTexCoords.push_back(texCoord);
                
                
                
            }
            
            allCircles.push_back(circleVertices);
            allCircleNormals.push_back(circleNormals);
            allCircleTexCoords.push_back(circleTexCoords);
            
        }
        if(settings.roundCap){
            generateCurvedTubeCap(allCircles, allCircleNormals, allCircleTexCoords, points[points.size()-1], tangents[tangents.size()-1], normals[normals.size()-1], false, totalLineLength);
            
        }
        if(!settings.roundCap){
            generateFlatTubeCap(allCircles, allCircleNormals, allCircleTexCoords, points[points.size()-1], tangents[tangents.size()-1], normals[normals.size()-1], false, totalLineLength);
        }
        
        
        //go through the circles and build the mesh
        for (int i = 0; i < allCircles.size() - 1; i++) {
            const vector<ofVec3f>& circle1 = allCircles[i];
            const vector<ofVec3f>& circle2 = allCircles[i + 1];
            const vector<ofVec3f>& normals1 = allCircleNormals[i];
            const vector<ofVec3f>& normals2 = allCircleNormals[i + 1];
            
            const vector<ofVec2f>& texCoords1 = allCircleTexCoords[i];
            const vector<ofVec2f>& texCoords2 = allCircleTexCoords[i + 1];
            
            
            for (int j = 0; j < circle1.size(); j++) {
                
                // Add two vertices at a time: one from the current circle, one from the next
                tubeMesh.addVertex(circle1[j]);
                tubeMesh.addVertex(circle2[j]);
                tubeMesh.addNormal(normals1[j]);
                tubeMesh.addNormal(normals2[j]);
                tubeMesh.addTexCoord(texCoords1[j]);
                tubeMesh.addTexCoord(texCoords2[j]);
            }
        }
    }
}

void ofxBezierTubeMeshBuilder::generateCurvedTubeCap(vector<vector<ofVec3f>>& allCircles,
                                            vector<vector<ofVec3f>>& allCircleNormals,
                                            vector<vector<ofVec2f>>& allCircleTexCoords,
                                            const ofVec3f& centre,
                                            const ofVec3f& tangent,
                                            const ofVec3f& normal, bool forwards, float _totalLineLength) {
    float step = settings.tubeRadius / (settings.tubeResolution + 1);
    ofVec3f poleCenter;
    if(forwards){
        poleCenter = centre - tangent * settings.tubeRadius;
        
        addRing(allCircles, allCircleNormals, allCircleTexCoords, poleCenter, 0, tangent, normal, centre, 0,_totalLineLength);
        // Loop to generate each ring, starting from the one closest to the equator and moving towards the pole
        for (int i = settings.tubeResolution; i >= 1; i--) {
            // Calculate the vertical distance from the center to the current ring
            float verticalDistance = step * i;
            // Calculate the actual distance from the sphere center to the ring center on the surface of the hemisphere
            float ringDistance = settings.tubeRadius * cos(asin(verticalDistance / settings.tubeRadius));
            // Calculate the ring center point
            ofVec3f ringCenter = centre - tangent * verticalDistance;
            
            // Calculate the ring radius
            float ringRadius = sqrt(settings.tubeRadius * settings.tubeRadius - verticalDistance * verticalDistance);
            
            // Add the calculated ring to the vectors
            addRing(allCircles, allCircleNormals, allCircleTexCoords, ringCenter, ringRadius, tangent, normal, centre, settings.tubeRadius - verticalDistance,_totalLineLength );
        }
    }
    
    
    // Add the special ring at the pole with zero radius
    // The center of this ring is the point on the hemisphere's surface directly above the sphere's center
    
    if(!forwards){
        
        for (int i = 1; i <= settings.tubeResolution; i++) {
            // Calculate the vertical distance from the center to the current ring
            float verticalDistance = step * i;
            // Calculate the actual distance from the sphere center to the ring center on the surface of the hemisphere
            float ringDistance = settings.tubeRadius * cos(asin(verticalDistance / settings.tubeRadius));
            // Calculate the ring center point
            ofVec3f ringCenter = centre + tangent * verticalDistance;
            
            // Calculate the ring radius
            float ringRadius = sqrt(settings.tubeRadius * settings.tubeRadius - verticalDistance * verticalDistance);
            
            // Add the calculated ring to the vectors
            addRing(allCircles, allCircleNormals, allCircleTexCoords, ringCenter, ringRadius, tangent, normal, centre,  (_totalLineLength - settings.tubeRadius) + verticalDistance, _totalLineLength );
        }
        ofVec3f southPoleCenter = centre + tangent * settings.tubeRadius;
        addRing(allCircles, allCircleNormals, allCircleTexCoords, southPoleCenter, 0, tangent, normal, centre , _totalLineLength,_totalLineLength);
    }
    
}

void ofxBezierTubeMeshBuilder::generateFlatTubeCap(vector<vector<ofVec3f>>& allCircles,
                                          vector<vector<ofVec3f>>& allCircleNormals,
                                          vector<vector<ofVec2f>>& allCircleTexCoords,
                                          const ofVec3f& flatCapCentre,
                                          const ofVec3f& tangent,
                                          const ofVec3f& normal, bool forwards, float _totalLineLength) {
    
    if(forwards){
        for(int i = 0; i < settings.tubeResolution; i++){
            
            vector<ofVec3f> circleVertices;
            vector<ofVec3f> circleNormals;
            vector<ofVec2f> circleTexCoords;
            
            float ringRadius = (settings.tubeRadius/settings.tubeResolution) * i;
            
            for (int j = 0; j <= settings.tubeResolution; j++) { // Use <= to include the last point in the circle
                float p = j / static_cast<float>(settings.tubeResolution);
                float a = p * 360;
                ofVec3f v0 = normal.getRotated(a, tangent) * ringRadius + flatCapCentre;
                circleVertices.push_back(v0);
                
                ofVec3f normal = tangent * -1;
                normal.normalize();
                circleNormals.push_back(normal);
                
                ofVec2f texCoord(a / 360.0, ringRadius / _totalLineLength);
                circleTexCoords.push_back(texCoord);
            }
            
            allCircles.push_back(circleVertices);
            allCircleNormals.push_back(circleNormals);
            allCircleTexCoords.push_back(circleTexCoords);
        }
    }
    else{
        for(int i = settings.tubeResolution - 1; i >= 0; i--){
            
            vector<ofVec3f> circleVertices;
            vector<ofVec3f> circleNormals;
            vector<ofVec2f> circleTexCoords;
            
            float ringRadius = (settings.tubeRadius/settings.tubeResolution) * i;
            
            for (int j = 0; j <= settings.tubeResolution; j++) { // Use <= to include the last point in the circle
                float p = j / static_cast<float>(settings.tubeResolution);
                float a = p * 360;
                ofVec3f v0 = normal.getRotated(a, tangent) * ringRadius + flatCapCentre;
                circleVertices.push_back(v0);
                
                ofVec3f normal = tangent;
                normal.normalize();
                circleNormals.push_back(normal);
                
                ofVec2f texCoord(a / 360.0, (ringRadius + (_totalLineLength - settings.tubeRadius)) / _totalLineLength);
                circleTexCoords.push_back(texCoord);
            }
            
            allCircles.push_back(circleVertices);
            allCircleNormals.push_back(circleNormals);
            allCircleTexCoords.push_back(circleTexCoords);
        }
    }
    
}


void ofxBezierTubeMeshBuilder::addRing(vector<vector<ofVec3f>>& allCircles,
                              vector<vector<ofVec3f>>& allCircleNormals,
                              vector<vector<ofVec2f>>& allCircleTexCoords, const ofVec3f& ringCenter, float radius, const ofVec3f& tangent, const ofVec3f& normal, const ofVec3f& sphereCenter, float distanceFromStart, float _totalLineLength) {
    std::vector<ofVec3f> circleVertices;
    std::vector<ofVec3f> circleNormals;
    std::vector<ofVec2f> circleTexCoords;
    
    for (int j = 0; j <= settings.tubeResolution; j++) { // Use <= to include the last point in the circle
        float p = j / static_cast<float>(settings.tubeResolution);
        float a = p * 360;
        ofVec3f v0 = normal.getRotated(a, tangent) * radius + ringCenter;
        circleVertices.push_back(v0);
        
        ofVec3f normalAtVertex = (v0 - sphereCenter).getNormalized();
        circleNormals.push_back(normalAtVertex);
        
        circleTexCoords.push_back(ofVec2f(p, distanceFromStart / _totalLineLength));
    }
    
    allCircles.push_back(circleVertices);
    allCircleNormals.push_back(circleNormals);
    allCircleTexCoords.push_back(circleTexCoords);
}

