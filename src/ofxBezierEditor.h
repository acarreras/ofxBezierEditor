#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxXmlSettings.h"

class draggableVertex{
	public:
        float 	x;
        float 	y;
        bool 	bBeingDragged;
        bool 	bOver;
        bool 	bBeingSelected;
};

class ofxBezierEditor{

	public:
	    ~ofxBezierEditor();
        ofxBezierEditor();

        vector <draggableVertex> curveVertices;
		vector <draggableVertex> controlPoint1;
		vector <draggableVertex> controlPoint2;
		ofPolyline polyLineFromPoints;
		void createPolyLineFromPoints();

		void loadXmlPoints();
		void saveXmlPoints();

		int getCurrentPointToMove(){ return currentPointToMove; };
		void setCurrentPointToMove(int p){ currentPointToMove = p; };

		int getLastVertexSelected(){ return lastVertexSelected; };
		void setLastVertexSelected(int v){ lastVertexSelected = v; };

        void draw();
        void drawHelp();
        virtual void mouseMoved(ofMouseEventArgs &args);
        virtual void mouseDragged(ofMouseEventArgs &args);
        virtual void mousePressed(ofMouseEventArgs &args);
        virtual void mouseReleased(ofMouseEventArgs &args);
        virtual void keyPressed(ofKeyEventArgs &args);
        virtual void keyReleased(ofKeyEventArgs &args);

        int getRadiusVertex(){ return radiusVertex; };
		void setRadiusVertex(int r){ radiusVertex = r; };

		int getRadiusControlPoints(){ return radiusControlPoints; };
		void setRadiusControlPoints(int r){ radiusControlPoints = r; };

		bool getFillBezier(){ return bfillBezier; };
		void setFillBezier(bool b){ bfillBezier = b; };

		void setColorFill(ofColor c){ colorFill = c; };
		void setColorFill(float r, float g, float b, float a=255.0){ colorFill.set(r,g,b,a); };
		void setColorFillR(float c){ colorFill.r = c; };
		void setColorFillG(float c){ colorFill.g = c; };
		void setColorFillB(float c){ colorFill.b = c; };
		void setColorFillA(float a){ colorFill.a = a; };
		void setColorStroke(ofColor c){ colorStroke = c; };
		void setColorStroke(float r, float g, float b, float a=255.0){ colorStroke.set(r,g,b,a); };
		void setColorStrokeR(float c){ colorStroke.r = c; };
		void setColorStrokeG(float c){ colorStroke.g = c; };
		void setColorStrokeB(float c){ colorStroke.b = c; };
		void setColorStrokeA(float a){ colorStroke.a = a; };

		int getStrokeWeight(){ return strokeWeight; };
		void setStrokeWeight(int w){ strokeWeight = w; };

		float getTranslateX(){ return translateX; };
		void setTranslateX(float t){ translateX = t; };
		float getTranslateY(){ return translateY; };
		void setTranslateY(float t){ translateY = t; };

		ofPoint getCenter(){ return center; };

	private:
        int currentPointToMove;
		int lastVertexSelected;

		ofxXmlSettings XMLbezier;

		int radiusVertex;
		int radiusControlPoints;

		bool bfillBezier;
		ofColor colorFill;
		ofColor colorStroke;
		int strokeWeight;

		ofRectangle boundingBox;
		void updateBoundingBox();
		bool bshowBoundingBox;

		ofPoint center;
        void calculateCenter();

		float translateX, translateY;
		float mouseX, mouseY;

		bool beditBezier;
};
