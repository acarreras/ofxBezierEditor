//
//  ofxBezierRibbonMeshBuilder.hpp
//  example-ui
//
//  Created by Fred Rodrigues on 22/11/2023.
//

#pragma once
#include "ofMain.h"

class ofxBezierRibbonMeshBuilder {
public:
    ofxBezierRibbonMeshBuilder();
    ~ofxBezierRibbonMeshBuilder();
    ofVboMesh getRibbonMesh() { return ribbonMesh; };
    ofVboMesh getRibbonMeshFromPolyline(ofPolyline inPoly);


private:
    ofVboMesh ribbonMesh;
    void generateTriangleStripFromPolyline(ofPolyline inPoly);


};
