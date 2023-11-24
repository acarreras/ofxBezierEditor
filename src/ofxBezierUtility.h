//
//  ofxBezierUtility.hpp
//  example-ui
//
//  Created by Fred Rodrigues on 23/11/2023.
//

#pragma once

#include "ofMain.h" // or other necessary includes

class ofxBezierUtility {
	public:
		static void ReflectEndPoints(ofPolyline & _inLine, ofPolyline & _outLine);

		static void CalculateInterpolatedTangentsAndPoints(const ofPolyline & line, vector <ofVec3f> & points, vector <ofVec3f> & tangents, int _meshLengthPrecisionMultiplier);

		static void CalculateInterpolatedTangentsNormalsAndPoints(const ofPolyline & line, vector <ofVec3f> & points, vector <ofVec3f> & tangents, vector <ofVec3f> & normals, int _meshLengthPrecisionMultiplier);

		static void CalculateLineLengths(const vector <ofVec3f> & points, std::vector <float> & segmentDistances,  float & totalLineLength, const bool _roundCap, float roundCapOffset);

	private:
		ofxBezierUtility(){
		}                 // Private constructor to prevent instantiation
};
