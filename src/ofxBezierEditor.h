#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxBezierTubeMeshBuilder.h"
#include "ofxBezierDraw.h"
#include "ofxBezierRibbonMeshBuilder.h"
#include "ofxBezierUI.h"
#include "ofxBezierEditorSettings.h"


class draggableVertex {
public:
    glm::vec2 pos = glm::vec2(0,0);
    bool bBeingDragged = false;
    bool bOver = false;
    bool bBeingSelected = false;
    
};

class ofxBezierEditor {

public:
    // Constructors and Destructor
    ~ofxBezierEditor();

    ofxBezierEditor();


    // Vertex and Control Point Containers
    vector<draggableVertex> curveVertices;
    vector<draggableVertex> controlPoint1;
    vector<draggableVertex> controlPoint2;

    ofPoint getCenter() { return center; };
    ofPolyline getPolyline() { return polyLineFromPoints; }
    
private:

    ofRectangle boundingBox;
    void updateBoundingBox();

    ofPoint center;
    void calculateCenter();


    ofPolyline polyLineFromPoints;
    void updatePolyline();
    
    // Mesh Generation
    void updateAllFromVertices();
  
    
   

};

