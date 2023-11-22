//
//  CurveRenderer.cpp
//  example-ui
//
//  Created by Fred Rodrigues on 21/11/2023.
//

#include "CurveRenderer.h"

CurveRenderer::CurveRenderer() : controlPointRadius(6.0f) {
    // Default colors
    strokeColor = ofColor(2, 189, 190, 100);
    fillColor = ofColor(188, 4, 62, 100);
    controlPointColor = ofColor(255, 0, 0, 100); // Red for control points
}

CurveRenderer::~CurveRenderer() {
    // Cleanup (if needed)
}

void CurveRenderer::draw(const BezierCurve& curve) {
    // Draw the Bezier curve
    const auto& curveVertices = curve.getCurveVertices();
    if (!curveVertices.empty()) {
        // Drawing the curve
        ofPushStyle();
        ofSetLineWidth(2);
        ofSetColor(strokeColor);
        ofNoFill();
        ofBeginShape();
        for (const auto& vert : curveVertices) {
            ofVertex(vert.x, vert.y);
        }
        ofEndShape(false);
        ofPopStyle();

        // Optionally fill the curve if it's closed
        if (curve.getClosed()) {
            ofPushStyle();
            ofFill();
            ofSetColor(fillColor);
            ofBeginShape();
            for (const auto& vert : curveVertices) {
                ofVertex(vert.x, vert.y);
            }
            ofEndShape(true);
            ofPopStyle();
        }
    }

    // Draw control points
    const auto& controlPoints = curve.getControlPoints();
    for (const auto& point : controlPoints) {
        ofPushStyle();
        ofFill();
        ofSetColor(controlPointColor);
        ofDrawCircle(point, controlPointRadius);
        ofPopStyle();
    }
}

void CurveRenderer::setStrokeColor(const ofColor& color) {
    strokeColor = color;
}

void CurveRenderer::setFillColor(const ofColor& color) {
    fillColor = color;
}

void CurveRenderer::setControlPointColor(const ofColor& color) {
    controlPointColor = color;
}

void CurveRenderer::setControlPointRadius(float radius) {
    controlPointRadius = radius;
}
