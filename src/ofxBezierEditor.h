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


    shared_ptr<vector<draggableVertex>> curveVertices = make_shared<vector<draggableVertex>>();
    shared_ptr<vector<draggableVertex>> controlPoint1 = make_shared<vector<draggableVertex>>();
    shared_ptr<vector<draggableVertex>> controlPoint2 = make_shared<vector<draggableVertex>>();


    ofPoint getCenter() { return center; };
    ofPolyline getPolyline() { return polyLineFromPoints; }
    
    
    ofColor getColorStroke();
    ofVboMesh getRibbonMesh();
    
    ofVboMesh getRibbonMeshFromPolyline(ofPolyline polyline);

    ofVboMesh getTubeMesh();
    ofVboMesh getTubeMeshFromPolyline(ofPolyline polyline);


    
    void draw();
    void drawHelp();
    void loadPoints(string filename);
    void setFillBezier(bool value);
    void setClosed(bool value);
    void setRibbonWidth(float value);
    void setColorFill(ofColor value);
    void setColorStroke(ofColor value);
    void setMeshLengthPrecisionMultiplier(int value);
    void setTubeRadius(float value);
    void setTubeResolution(int value);
    void setHasRoundCaps(bool value);
    void setReactToMouseAndKeyEvents(bool value);
    void setUseRibbonMesh(bool value);
    void setUseTubeMesh(bool value);

private:

    void updateBoundingBox();

    ofPoint center;
    void calculateCenter();


    ofPolyline polyLineFromPoints;
    void updatePolyline();
    
    // Mesh Generation
    void updateAllFromVertices();
  
    
    ofxBezierEditorSettings settings;
    ofxBezierTubeMeshBuilder tubeMeshBuilder;
    ofxBezierRibbonMeshBuilder ribbonMeshBuilder;
    ofxBezierDraw bezierDraw;
    ofxBezierUI bezierUI;
    
    void onTriggerUpdate(TriggerUpdateEventArgs& args);

};

