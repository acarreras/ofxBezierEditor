#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxBezierTubeMeshBuilder.h"
#include "ofxBezierDraw.h"
#include "ofxBezierRibbonMeshBuilder.h"
#include "ofxBezierUI.h"
#include "ofxBezierEditorSettings.h"
#include "draggableVertex.h"
#include "ofxBezierEvents.h"


class ofxBezierEditor {
	public:
		ofxBezierEditor();
		~ofxBezierEditor();

		shared_ptr <vector <draggableVertex> > curveVertices = make_shared <vector <draggableVertex> >();
		shared_ptr <vector <draggableVertex> > controlPoint1 = make_shared <vector <draggableVertex> >();
		shared_ptr <vector <draggableVertex> > controlPoint2 = make_shared <vector <draggableVertex> >();

		ofPoint getCenter(){
			return center;
		}
		ofPolyline getPolyline(){
			return polyLineFromPoints;
		}

		void draw();
		void drawHelp();

		void loadPoints(string filename);
		void savePoints(string filename);
		void setFillBezier(bool value);
		bool getFillBezier();
		void setClosed(bool value);
		bool getClosed();
		void setRibbonWidth(float value);
		float getRibbonWidth();
		void setColorFill(ofColor value);
		ofColor getColourFill();
		void setColorStroke(ofColor value);
		ofColor getColourStroke();
		void setMeshLengthPrecisionMultiplier(int value);
		int getMeshLengthPrecisionMultiplier();
		void setTubeRadius(float value);
		float getTubeRadius();
		void setTubeResolution(int value);
		int getTubeResolution();
		void setHasRoundCaps(bool value);
		bool getHasRoundCaps();
		void setUseRibbonMesh(bool value);
		bool getUseRibbonMesh();
		void setUseTubeMesh(bool value);
		bool getUseTubeMesh();

		void setReactToMouseAndKeyEvents(bool value);

		ofVboMesh getRibbonMesh();
		ofVboMesh getRibbonMeshFromPolyline(ofPolyline polyline);

		ofVboMesh getTubeMesh();
		ofVboMesh getTubeMeshFromPolyline(ofPolyline polyline);

	private:

		ofxBezierEditorSettings settings;
		ofxBezierTubeMeshBuilder tubeMeshBuilder;
		ofxBezierRibbonMeshBuilder ribbonMeshBuilder;
		ofxBezierDraw bezierDraw;
		ofxBezierUI bezierUI;

		void updateBoundingBox();

		ofPoint center;
		void calculateCenter();

		ofPolyline polyLineFromPoints;
		void updatePolyline();

		void updateAllFromVertices();

		void onTriggerUpdate(TriggerUpdateEventArgs & args);

};

