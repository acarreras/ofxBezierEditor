//
//  MeshGenerator.hpp
//  example-ui
//
//  Created by Fred Rodrigues on 21/11/2023.
//

#include "ofMain.h"
#include "BezierCurve.h"

class MeshGenerator {
public:
    MeshGenerator();
    ~MeshGenerator();

    ofVboMesh generateRibbonMesh(const BezierCurve& curve);
    ofVboMesh generateTubeMesh(const BezierCurve& curve);

private:
    float ribbonWidth;
    float tubeRadius;
    int tubeResolution;
};
