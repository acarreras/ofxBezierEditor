//
//  InteractionManager.cpp
//  example-ui
//
//  Created by Fred Rodrigues on 21/11/2023.
//

#include "InteractionManager.h"

InteractionManager::InteractionManager() : bezierCurve(nullptr) {
    // Register event listeners
}

InteractionManager::~InteractionManager() {
    // Unregister event listeners
}

void InteractionManager::handleMouseEvents(ofMouseEventArgs &args) {
    // Handle mouse interactions
}

void InteractionManager::handleKeyEvents(ofKeyEventArgs &args) {
    // Handle key interactions
}

void InteractionManager::setBezierCurve(BezierCurve* curve) {
    bezierCurve = curve;
}
