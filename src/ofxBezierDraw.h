//
//  ofxBezierDraw.hpp
//  example-ui
//
//  Created by Fred Rodrigues on 22/11/2023.
//

#pragma once

#include "ofMain.h"
#include "ofxBezierEditorSettings.h"
#include "draggableVertex.h"
#include "ofxBezierEvents.h"

class ofxBezierDraw {
public:
    ofxBezierDraw(ofxBezierEditorSettings& settings,
                  std::vector<draggableVertex>& curveVertices,
                  std::vector<draggableVertex>& controlPoint1,
                  std::vector<draggableVertex>& controlPoint2);
    
    ~ofxBezierDraw();
    
    // Drawing Functions
    void draw();
    void drawOutline();
    void drawHelp();
    void drawWithNormals(const ofPolyline& polyline);
    void updateNormalLine(ofPolyline line);

private:
    
    ofxBezierEditorSettings& settings;
    std::vector<draggableVertex>& curveVertices;
    std::vector<draggableVertex>& controlPoint1;
    std::vector<draggableVertex>& controlPoint2;
    void onTriggerUpdate(TriggerUpdateEventArgs& args);
    ofPolyline normalLine;
    
};
