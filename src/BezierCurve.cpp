//
//  BezierCurve.cpp
//  example-ui
//
//  Created by Fred Rodrigues on 21/11/2023.
//

#include "BezierCurve.h"

BezierCurve::BezierCurve() {
    // Initialization (if needed)
}

BezierCurve::~BezierCurve() {
    // Cleanup (if needed)
}

void BezierCurve::calculateCurvePoints() {
    updateCurve();
}

void BezierCurve::addControlPoint(const glm::vec2& point) {
    controlPoints.push_back(point);
    updateCurve();
}

void BezierCurve::removeControlPoint(int index) {
    if (index >= 0 && index < controlPoints.size()) {
        controlPoints.erase(controlPoints.begin() + index);
        updateCurve();
    }
}

void BezierCurve::moveControlPoint(int index, const glm::vec2& newPosition) {
    if (index >= 0 && index < controlPoints.size()) {
        controlPoints[index] = newPosition;
        updateCurve();
    }
}

const vector<glm::vec2>& BezierCurve::getCurveVertices() const {
    return curveVertices;
}

const vector<glm::vec2>& BezierCurve::getControlPoints() const {
    return controlPoints;
}

void BezierCurve::updateCurve() {
    curveVertices.clear();
    // Logic to calculate Bezier curve based on controlPoints
    // This typically involves a Bezier curve algorithm, which interpolates
    // between the control points to create the smooth curve.
    // The complexity of the algorithm depends on whether you're using
    // quadratic, cubic, or higher order Bezier curves.

    // For example, a simple linear interpolation (not an actual Bezier curve):
//    for (size_t i = 1; i < controlPoints.size(); ++i) {
//        // Linear interpolation for demonstration purposes
//        float t = 0.5; // Interpolation factor
//        glm::vec2 interpolatedPoint = controlPoints[i-1] * (1 - t) + controlPoints[i] * t;
//        curveVertices.push_back(interpolatedPoint);
//    }
}
