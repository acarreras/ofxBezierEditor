//
//  ofxBezierUI.hpp
//  example-ui
//
//  Created by Fred Rodrigues on 22/11/2023.
//

#pragma once
#include "ofMain.h"

class ofxBezierUI {
public:
    ofxBezierUI();
    ~ofxBezierUI();

    // Mouse and Key Event Handlers
    virtual void mouseMoved(ofMouseEventArgs &args);
    virtual void mouseDragged(ofMouseEventArgs &args);
    virtual void mousePressed(ofMouseEventArgs &args);
    virtual void mouseReleased(ofMouseEventArgs &args);
    virtual void mouseScrolled(ofMouseEventArgs &args);
    virtual void mouseEntered(ofMouseEventArgs &args);
    virtual void mouseExited(ofMouseEventArgs &args);
    virtual void keyPressed(ofKeyEventArgs &args);
    virtual void keyReleased(ofKeyEventArgs &args);
    
    // Getters and Setters
    int getCurrentPointToMove() { return currentPointToMove; };
    void setCurrentPointToMove(int p) { currentPointToMove = p; };
    
    int getLastVertexSelected() { return lastVertexSelected; };
    void setLastVertexSelected(int v) { lastVertexSelected = v; };
    
    int getRadiusControlPoints() { return radiusControlPoints; };
    void setRadiusControlPoints(int r) { radiusControlPoints = r; };
    
    // Event Handling
    void setReactToMouseAndKeyEvents(bool b);
    void registerToEvents();
    void unregisterFromEvents();
    
    
    

private:
    // Private Variables
    int currentPointToMove;
    int lastVertexSelected;
    float translateX, translateY;
    float mouseX, mouseY;
    bool bfillBezier;

};

