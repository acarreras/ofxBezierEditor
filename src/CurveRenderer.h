//
//  CurveRenderer.hpp
//  example-ui
//
//  Created by Fred Rodrigues on 21/11/2023.
//

#pragma once
#include "ofMain.h"
#include "BezierCurve.h"

class CurveRenderer {
public:
    CurveRenderer();
    ~CurveRenderer();

    void draw(const BezierCurve& curve); // Draws the Bezier curve and control points
    void setStrokeColor(const ofColor& color); // Sets the stroke color for the curve
    void setFillColor(const ofColor& color); // Sets the fill color for the curve
    void setControlPointColor(const ofColor& color); // Sets the color for control points
    void setControlPointRadius(float radius); // Sets the radius for control points

private:
    ofColor strokeColor;
    ofColor fillColor;
    ofColor controlPointColor;
    float controlPointRadius;
};
