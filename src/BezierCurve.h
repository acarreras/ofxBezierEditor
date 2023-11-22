//
//  BezierCurve.hpp
//  example-ui
//
//  Created by Fred Rodrigues on 21/11/2023.
//

#pragma once
#include "ofMain.h"

class BezierCurve {
public:
    BezierCurve();
    ~BezierCurve();

    void calculateCurvePoints(); // Recalculates the curve based on control points
    void addControlPoint(const glm::vec2& point); // Adds a new control point
    void removeControlPoint(int index); // Removes a control point by index
    void moveControlPoint(int index, const glm::vec2& newPosition); // Moves a specific control point

    const vector<glm::vec2>& getCurveVertices() const; // Returns the calculated curve vertices
    const vector<glm::vec2>& getControlPoints() const; // Returns the control points

private:
    vector<glm::vec2> curveVertices; // Vertices that make up the curve
    vector<glm::vec2> controlPoints; // Control points used to calculate the curve

    void updateCurve(); // Internal function to update the curve vertices
};
