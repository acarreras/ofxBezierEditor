//
//  ofxBezierTubeMeshBuilder.hpp
//  example-ui
//
//  Created by Fred Rodrigues on 22/11/2023.
//


#pragma once
#include "ofMain.h"
#include "ofxBezierEditorSettings.h"
#include "ofxBezierEvents.h"

class ofxBezierTubeMeshBuilder {
	public:
		ofxBezierTubeMeshBuilder(ofxBezierEditorSettings & settings);

		~ofxBezierTubeMeshBuilder();

		ofVboMesh getTubeMesh(){
			return tubeMesh;
		}
		ofVboMesh getTubeMeshFromPolyline(ofPolyline inPoly);
		void generateTubeMeshFromPolyline(ofPolyline inPoly);


	private:


		void createLatitudeRings(const ofVec3f & center, const ofVec3f & extension, float tubeResolution, float tubeRadius, bool forwards);

		void generateCurvedTubeCap(const ofVec3f & center, const ofVec3f & tangent, const ofVec3f & normal, bool forwards, float _totalLineLength);

		void generateFlatTubeCap(const ofVec3f & flatCapCentre, const ofVec3f & tangent, const ofVec3f & normal, bool forwards, float _totalLineLength);

		void addRing(const ofVec3f & ringCenter, float radius, const ofVec3f & tangent, const ofVec3f & norma, const ofVec3f & sphereCenter, float distanceFromStart, float _totalLineLength);

		ofVboMesh tubeMesh;

		ofxBezierEditorSettings & settings;

		void onTriggerUpdate(TriggerUpdateEventArgs & args);

		//We use these to store the information from our line
		vector <ofVec3f> points;
		vector <ofVec3f> tangents;
		vector <ofVec3f> normals;

		//We make the tube entirely from rings around the points on the line (even the caps are made this way.
		//These vectors hold all the ring information
		vector <vector <ofVec3f> > allCircleVertices;
		vector <vector <ofVec3f> > allCircleNormals;
		vector <vector <ofVec2f> > allCircleTexCoords;

		std::vector <float> segmentDistances;
		float totalLineLength = 0;


};
