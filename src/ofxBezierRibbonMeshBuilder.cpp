//
//  ofxBezierRibbonMeshBuilder.cpp
//  example-ui
//
//  Created by Fred Rodrigues on 22/11/2023.
//

#include "ofxBezierRibbonMeshBuilder.h"

ofxBezierRibbonMeshBuilder::ofxBezierRibbonMeshBuilder() {
    // Initialization
}

ofxBezierRibbonMeshBuilder::~ofxBezierRibbonMeshBuilder() {
    // Cleanup
}

ofVboMesh ofxBezierRibbonMeshBuilder::getRibbonMeshFromPolyline(ofPolyline inPoly){
    generateTriangleStripFromPolyline(inPoly);
    return ribbonMesh;
}

void ofxBezierRibbonMeshBuilder::generateTriangleStripFromPolyline(ofPolyline inPoly) {
    if (inPoly.getVertices().size() > 2) {
        // Clear mesh (triangle strip)
        ribbonMesh.clear();
        ribbonMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        
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
        
        // Create vectors to store points and tangents
        vector<ofVec3f> points;
        vector<ofVec2f> tangents;
        
        for (int i = 1; i < workerLine.size() - 1; i++) {
            points.push_back(ofVec3f(workerLine[i].x, workerLine[i].y, 0));
            ofVec2f tangent = workerLine.getTangentAtIndex(i);
            tangents.push_back(tangent);
            if (i < workerLine.size() - 2) {
                // Add intermediate points and tangents based on precisionMultiplier
                for (int j = 1; j < meshLengthPrecisionMultiplier; j++) {
                    float t = static_cast<float>(j) / static_cast<float>(meshLengthPrecisionMultiplier);
                    ofVec3f interpolatedPoint = workerLine.getPointAtIndexInterpolated(i + t);
                    ofVec2f interpolatedTangent = workerLine.getTangentAtIndexInterpolated(i + t);
                    points.push_back(interpolatedPoint);
                    tangents.push_back(interpolatedTangent);
                }
            }
            
        }
        std::vector<float> segmentDistances;
        
        float totalLineLength = 0;
        
        if(roundCap){
            segmentDistances.push_back(ribbonWidth/2);
            totalLineLength +=  ribbonWidth;
        }
        else{
            segmentDistances.push_back(0);
        }
        
        //get the distance between each point on the line
        for(int i = 0; i < points.size()-1; i++){
            float distance = points[i].distance(points[i+1]);
            
            segmentDistances.push_back(distance + segmentDistances.back());
            
            totalLineLength += distance;
        }
        
        if(roundCap){
            //get the poiunts on a hald circle for the cap, the circle centre is the first point and the direction is the tangent. There should be 20 pounts on the half circle.
            generateCurvedRibbonCap(points[0], tangents[0], true, totalLineLength);
        }
        
        for (int i = 0; i < points.size(); i++) {
            // Calculate the perpendicular vector
            ofVec2f perpendicular(-tangents[i].y, tangents[i].x); // Perpendicular vector
            perpendicular.normalize();
            
            // Calculate the vertices for both sides
            ofVec3f currentPoint = points[i];
            ofVec3f leftVertex = currentPoint - perpendicular * (ribbonWidth * 0.5);
            ofVec3f rightVertex = currentPoint + perpendicular * (ribbonWidth * 0.5);
            
            // Add vertices to the mesh in a zigzag manner, suitable for a triangle strip
            ribbonMesh.addVertex(leftVertex); // Add left vertex
            ribbonMesh.addTexCoord(ofVec2f(0, segmentDistances[i] / totalLineLength)); // Add left texture coordinate
            ribbonMesh.addVertex(rightVertex); // Add right vertex
            ribbonMesh.addTexCoord(ofVec2f(1, segmentDistances[i] / totalLineLength)); // Add left texture coordinate
        }
        
        if(roundCap){
            //get the poiunts on a hald circle for the cap, the circle centre is the first point and the direction is the tangent. There should be 20 pounts on the half circle.
            generateCurvedRibbonCap(points[points.size()-1], tangents[tangents.size()-1], false, totalLineLength);
        }
    }
}

void ofxBezierRibbonMeshBuilder::generateCurvedRibbonCap(ofVec3f centre,  ofVec3f tangent, bool forwards, float totalLineLength){
    // Number of points to create the half circle
    const int numPointsHalfCircle = 20;
    float angleStep = 180.0f / numPointsHalfCircle; // Half circle so we step through 180 degrees
    
    ofVec3f circleCenter = centre;
    ofVec2f circleTangent = tangent.normalize(); // Ensure the tangent is normalized
    
    // Calculate the normal to the tangent
    ofVec2f circleNormal(-circleTangent.y, circleTangent.x); // Perpendicular to the tangent
    
    std::vector<ofVec3f> halfCirclePoints;
    float textureCoordX = 0.5;
    float textureCoordY = 0; // Top of the curve
    for(int i = 0; i <= numPointsHalfCircle; ++i) {
        float angle;
        if(forwards){
            angle = ofDegToRad(90 + angleStep * i); // Start from -90 degrees to make a semicircle
            //cout << "forwards angle: " << ofRadToDeg(angle) << endl;
        }
        else{
            angle = ofDegToRad(angleStep * i - 90); // Start from -90 degrees to make a semicircle
            //cout << "backwards angle: " << ofRadToDeg(angle) << endl;
            
        }
        
        // Calculate the point on the circle using cosine and sine for the respective axes
        ofVec3f circlePoint = circleCenter + circleTangent * cos(angle) * (ribbonWidth * 0.5) + circleNormal * sin(angle) * (ribbonWidth * 0.5);
        
        
        // Add the calculated point to the half circle points vector
        ribbonMesh.addVertex(circlePoint);
        ribbonMesh.addVertex(circleCenter);
        
        
        // Now calculate distances relative to the equator
        float distanceToEquator = ribbonWidth * 0.5  * 0.5  + ((ribbonWidth * 0.5  * 0.5) * sin(angle));
        float distanceToEquatorPerp = abs((ribbonWidth * 0.5) * cos(angle));
        
        
        float texCoordX = distanceToEquator / (ribbonWidth * 0.5); // Normalize between 0 and 1
        
        float texCoordY;
        if(forwards){
            texCoordY  = (ribbonWidth / 2 - distanceToEquatorPerp)/totalLineLength; // Normalize angle between 0 and 1
            
        }
        else{
            texCoordY  = (distanceToEquatorPerp + (totalLineLength - ribbonWidth/2)) /totalLineLength; // Normalize angle between 0 and 1
        }
        
        // Add texture coordinates for the semicircle point
        ribbonMesh.addTexCoord(ofVec2f(texCoordX, texCoordY));
        
        //the next texCoord is always the circle centre, this is easy to find
        if(forwards){
            ribbonMesh.addTexCoord(ofVec2f(0.5, (ribbonWidth/2) /totalLineLength));
            
        }
        else{
            ribbonMesh.addTexCoord(ofVec2f(0.5, (totalLineLength - (ribbonWidth/2)) /totalLineLength));
            
        }
    }
}
