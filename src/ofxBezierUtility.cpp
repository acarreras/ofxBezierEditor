//
//  ofxBezierUtility.cpp
//  example-ui
//
//  Created by Fred Rodrigues on 23/11/2023.
//

#include "ofxBezierUtility.h"

void ofxBezierUtility::ReflectEndPoints(ofPolyline & _inLine, ofPolyline & _outLine){
	_outLine = _inLine;
	//lets add extra points at the start and end of the line using reflection so we get good normals for the actual start and end
	glm::vec3 firstPoint = _outLine.getVertices()[0];
	glm::vec3 secondPoint = _outLine.getVertices()[1];
	glm::vec3 reflectedFirst = firstPoint - (secondPoint - firstPoint);

	// Reflect the second-to-last point over the last for the end of the line
	glm::vec3 lastPoint = _outLine.getVertices()[_outLine.getVertices().size() - 1];
	glm::vec3 secondLastPoint = _outLine.getVertices()[_outLine.getVertices().size() - 2];
	glm::vec3 reflectedLast = lastPoint - (secondLastPoint - lastPoint);

	//add the firstPoint to the start of the line
	_outLine.insertVertex(reflectedFirst, 0);
	_outLine.addVertex(reflectedLast);
}

void ofxBezierUtility::CalculateInterpolatedTangentsAndPoints(const ofPolyline & line, vector <ofVec3f> & points, vector <ofVec3f> & tangents, int _meshLengthPrecisionMultiplier){
	for(int i = 1; i < line.size() - 1; i++){
		points.push_back(ofVec3f(line[i].x, line[i].y, 0));
		ofVec2f tangent = line.getTangentAtIndex(i);
		tangents.push_back(tangent);
		if(i < line.size() - 2){
			// Add intermediate points and tangents based on precisionMultiplier
			for(int j = 1; j < _meshLengthPrecisionMultiplier; j++){
				float t = static_cast <float>(j) / static_cast <float>(_meshLengthPrecisionMultiplier);
				ofVec3f interpolatedPoint = line.getPointAtIndexInterpolated(i + t);
				ofVec2f interpolatedTangent = line.getTangentAtIndexInterpolated(i + t);
				points.push_back(interpolatedPoint);
				tangents.push_back(interpolatedTangent);
			}
		}
	}
}
void ofxBezierUtility::CalculateInterpolatedTangentsNormalsAndPoints(const ofPolyline & line, vector <ofVec3f> & points, vector <ofVec3f> & tangents, vector <ofVec3f> & normals, int _meshLengthPrecisionMultiplier){

	for(int i = 1; i < line.size() - 1; i++){
		points.push_back(ofVec3f(line[i].x, line[i].y, line[i].z));
		tangents.push_back(line.getTangentAtIndex(i));
		normals.push_back(line.getNormalAtIndex(i));

		// Add intermediate points and tangents based on precisionMultiplier
		if(i < line.size() - 2){

			for(int j = 1; j < _meshLengthPrecisionMultiplier; j++){
				float t = float(j) / float(_meshLengthPrecisionMultiplier);
				ofVec3f interpolatedPoint = line.getPointAtIndexInterpolated(i + t);
				ofVec2f interpolatedTangent = line.getTangentAtIndexInterpolated(i + t);
				ofVec2f interpolatedNormal = line.getNormalAtIndexInterpolated(i + t);

				points.push_back(interpolatedPoint);
				tangents.push_back(interpolatedTangent);
				normals.push_back(interpolatedNormal);
			}
		}
	}
}


void ofxBezierUtility::CalculateLineLengths(const vector <ofVec3f> & points, std::vector <float> & segmentDistances,  float & totalLineLength, const bool _roundCap, float roundCapOffset){
	if(_roundCap){
		//the first segment is from the tip of the curve to the first tangent
		segmentDistances.push_back(roundCapOffset / 2);
		//The total length has 2 caps
		totalLineLength += roundCapOffset;
	}else{
		segmentDistances.push_back(0);
	}

	//get the distance between each point on the line
	for(int i = 0; i < points.size() - 1; i++){
		float distance = points[i].distance(points[i + 1]);

		segmentDistances.push_back(distance + segmentDistances.back());

		totalLineLength += distance;
	}

}
