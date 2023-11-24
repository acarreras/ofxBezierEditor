//
//  ofxBezierRibbonMeshBuilder.hpp
//  example-ui
//
//  Created by Fred Rodrigues on 22/11/2023.
//

#pragma once
#include "ofMain.h"
#include "ofxBezierEditorSettings.h"
#include "draggableVertex.h"
#include "ofxBezierEvents.h"

class ofxBezierRibbonMeshBuilder {
public:
    ofxBezierRibbonMeshBuilder(ofxBezierEditorSettings& settings);
    
    ~ofxBezierRibbonMeshBuilder();
    

    ofVboMesh getRibbonMesh() { return ribbonMesh; };
    ofVboMesh getRibbonMeshFromPolyline(ofPolyline inPoly);
    void generateRibbonMeshFromPolyline(ofPolyline inPoly);


private:
    ofVboMesh ribbonMesh;
    void generateCurvedRibbonCap(ofVec3f centre,  ofVec3f tangent, bool forwards, float totalLineLength);
    ofxBezierEditorSettings& settings;
    void onTriggerUpdate(TriggerUpdateEventArgs& args);
    
    std::vector<float> segmentDistances;
    
    float totalLineLength = 0;

};
