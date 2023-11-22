//
//  InteractionManager.hpp
//  example-ui
//
//  Created by Fred Rodrigues on 21/11/2023.
//

#pragma once
#include "ofMain.h"
#include "BezierCurve.h"

class InteractionManager {
public:
    InteractionManager();
    ~InteractionManager();

    void handleMouseEvents(ofMouseEventArgs &args);
    void handleKeyEvents(ofKeyEventArgs &args);

    void setBezierCurve(BezierCurve* curve);
    
private:
    BezierCurve* bezierCurve;
};
