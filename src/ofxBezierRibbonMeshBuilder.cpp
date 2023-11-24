//
//  ofxBezierRibbonMeshBuilder.cpp
//  example-ui
//
//  Created by Fred Rodrigues on 22/11/2023.
//

#include "ofxBezierRibbonMeshBuilder.h"
#include "ofxBezierUtility.h"

ofxBezierRibbonMeshBuilder::ofxBezierRibbonMeshBuilder(ofxBezierEditorSettings & settings) : settings(settings){

}

ofxBezierRibbonMeshBuilder::~ofxBezierRibbonMeshBuilder(){
	// Cleanup
}

ofVboMesh ofxBezierRibbonMeshBuilder::getRibbonMeshFromPolyline(ofPolyline inPoly){
	generateRibbonMeshFromPolyline(inPoly);
	return ribbonMesh;
}

void ofxBezierRibbonMeshBuilder::generateRibbonMeshFromPolyline(ofPolyline inPoly){
	if(inPoly.getVertices().size() > 2){
		// Clear mesh (triangle strip)
		ribbonMesh.clear();
		ribbonMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
		ofPolyline workerLine;

		vector <ofVec3f> points;
		vector <ofVec3f> tangents;
		segmentDistances.clear();
		totalLineLength = 0;

		ofxBezierUtility::ReflectEndPoints(inPoly, workerLine);
		ofxBezierUtility::CalculateInterpolatedTangentsAndPoints(workerLine, points, tangents, settings.meshLengthPrecisionMultiplier);
		ofxBezierUtility::CalculateLineLengths(points, segmentDistances, totalLineLength, settings.roundCap, settings.ribbonWidth);

		if(settings.roundCap){
			//get the poiunts on a hald circle for the cap, the circle centre is the first point and the direction is the tangent. There should be 20 pounts on the half circle.
			generateCurvedRibbonCap(points[0], tangents[0], true, totalLineLength);
		}

		for(int i = 0; i < points.size(); i++){
			// Calculate the perpendicular vector
			ofVec2f perpendicular(-tangents[i].y, tangents[i].x); // Perpendicular vector
			perpendicular.normalize();

			// Calculate the vertices for both sides
			ofVec3f currentPoint = points[i];
			ofVec3f leftVertex = currentPoint - perpendicular * (settings.ribbonWidth * 0.5);
			ofVec3f rightVertex = currentPoint + perpendicular * (settings.ribbonWidth * 0.5);

			// Add vertices to the mesh in a zigzag manner, suitable for a triangle strip
			ribbonMesh.addVertex(leftVertex); // Add left vertex
			ribbonMesh.addTexCoord(ofVec2f(0, segmentDistances[i] / totalLineLength)); // Add left texture coordinate

			ribbonMesh.addVertex(rightVertex); // Add right vertex
			ribbonMesh.addTexCoord(ofVec2f(1, segmentDistances[i] / totalLineLength)); // Add left texture coordinate

		}

		if(settings.roundCap){
			//get the poiunts on a hald circle for the cap, the circle centre is the first point and the direction is the tangent. There should be 20 pounts on the half circle.
			generateCurvedRibbonCap(points[points.size() - 1], tangents[tangents.size() - 1], false, totalLineLength);
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

	std::vector <ofVec3f> halfCirclePoints;
	float textureCoordX = 0.5;
	float textureCoordY = 0; // Top of the curve
	for(int i = 0; i <= numPointsHalfCircle; ++i){
		float angle;
		if(forwards){
			angle = ofDegToRad(90 + angleStep * i); // Start from -90 degrees to make a semicircle
		}else{
			angle = ofDegToRad(angleStep * i - 90); // Start from -90 degrees to make a semicircle
		}

		// Calculate the point on the circle using cosine and sine for the respective axes
		ofVec3f circlePoint = circleCenter + circleTangent * cos(angle) * (settings.ribbonWidth * 0.5) + circleNormal * sin(angle) * (settings.ribbonWidth * 0.5);

		// Add the calculated point to the half circle points vector
		ribbonMesh.addVertex(circlePoint);
		ribbonMesh.addVertex(circleCenter);

		// Now calculate distances relative to the equator
		float distanceToEquator = settings.ribbonWidth * 0.5  * 0.5  + ((settings.ribbonWidth * 0.5  * 0.5) * sin(angle));
		float distanceToEquatorPerp = abs((settings.ribbonWidth * 0.5) * cos(angle));
		float texCoordX = distanceToEquator / (settings.ribbonWidth * 0.5); // Normalize between 0 and 1

		float texCoordY;
		if(forwards){
			texCoordY = (settings.ribbonWidth / 2 - distanceToEquatorPerp) / totalLineLength; // Normalize angle between 0 and 1
		}else{
			texCoordY = (distanceToEquatorPerp + (totalLineLength - settings.ribbonWidth / 2)) / totalLineLength; // Normalize angle between 0 and 1
		}

		// Add texture coordinates for the semicircle point
		ribbonMesh.addTexCoord(ofVec2f(texCoordX, texCoordY));

		//the next texCoord is always the circle centre, this is easy to find
		if(forwards){
			ribbonMesh.addTexCoord(ofVec2f(0.5, (settings.ribbonWidth / 2) / totalLineLength));
		}else{
			ribbonMesh.addTexCoord(ofVec2f(0.5, (totalLineLength - (settings.ribbonWidth / 2)) / totalLineLength));
		}
	}
}
