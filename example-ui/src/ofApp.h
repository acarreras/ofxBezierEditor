#pragma once

#include "ofMain.h"
#include "ofxBezierEditor.h"
#include "ofxGui.h"

enum OperationMode {
    POLYLINE_MODE,
    FAT_LINE_MODE,
    TUBE_MESH_MODE,
};

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();
    
    
    ofxBezierEditor myBezier;
    bool bdrawBezierInfo;
        
    ofEasyCam cam;
    ofLight light;
    
    ofxPanel gui;
    ofParameter<int>OperationModeSlider = POLYLINE_MODE;

    ofParameter<bool> fillToggle;
    ofParameter<bool> closedToggle;
    ofParameter<bool> drawBoundingBox;
    ofParameter<ofColor> fillColor;
    ofParameter<ofColor> strokeColor;
    ofParameter<float> ribbonWidthSlider;
    ofParameter<int> meshLengthPrecisionSlider;
    ofParameter<float> tubeRadiusSlider;
    ofParameter<int> tubePrecisionSlider;
    ofParameter<bool> useRoundCapsToggle;
    ofParameter<bool> useTexToggle;
    ofParameter<bool> wireframeToggle;
    ofParameter<bool> drawNormalsToggle;

    ofImage uvCheck;
    
    void OperationModeSliderChanged(int &value);
    void fillToggleChanged(bool &value);
    void closedToggleChanged(bool &value);
    void drawBoundingBoxChanged(bool &value);
    void fillColorChanged(ofColor &value);
    void strokeColorChanged(ofColor &value);
    
    void ribbonWidthSliderChanged(float &value);
    void ribbonWidthSliderChnaged(float &value);
    void meshLengthPrecisionSliderChanged(int &value);
    void tubeRadiusSliderChanged(float &value);
    void tubePrecisionSliderChanged(int &value);
    void useRoundCapsToggleChanged(bool &value);
    void useTexToggleChanged(bool &value);

    
    
};
