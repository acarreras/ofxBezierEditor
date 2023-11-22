//
//  ofxBezierDraw.hpp
//  example-ui
//
//  Created by Fred Rodrigues on 22/11/2023.
//

#pragma once
#include "ofMain.h"

class ofxBezierDraw {
public:
    ofxBezierDraw();
    ~ofxBezierDraw();

    // Color Control
    void setColorFill(ofColor c);
    void setColorFill(float r, float g, float b, float a = 255.0);
    void setColorFillR(float c);
    void setColorFillG(float c);
    void setColorFillB(float c);
    void setColorFillA(float a);
    
    void setColorStroke(ofColor c);
    void setColorStroke(float r, float g, float b, float a = 255.0);
    void setColorStrokeR(float c);
    void setColorStrokeG(float c);
    void setColorStrokeB(float c);
    void setColorStrokeA(float a);
    
    ofColor getColorFill() { return colorFill; };
    ofColor getColorStroke() { return colorStroke; };
    
    // Drawing Functions
    void draw();
    void drawOutline();
    void drawHelp();

 

    // Visual Attributes
    int getRadiusVertex() { return radiusVertex; };
    void setRadiusVertex(int r) { radiusVertex = r; };

private:
    
 
  
};
