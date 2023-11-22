#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    myBezier.loadPoints("ofxBezierInfo.json");
    bdrawBezierInfo = false;
    
    ofSetSmoothLighting(true);
    
    light.setDiffuseColor(ofColor(0.f, 255.f, 0.f));
    light.setSpecularColor(ofColor(255.f, 255.f, 255.f));
    light.setPosition(0, 300, 0);
    cam.disableMouseInput();
    ofSetBackgroundColor(255);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if(bdrawBezierInfo){
        myBezier.drawHelp();
    }
    else{
        if(mode == POLYLINE_MODE){
            myBezier.draw();
        }
        
        else if(mode == FAT_LINE_MODE){
            ofSetColor(myBezier.getColorStroke());
            myBezier.getRibbonMesh().draw();
        }
        
        else if(mode == TUBE_MESH_MODE){
            ofEnableDepthTest();
            cam.begin();
            light.enable();
            ofScale(1, -1);
            ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
            myBezier.getTubeMesh().draw();
            cam.end();
            ofDisableDepthTest();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == '1'){
        mode = POLYLINE_MODE;
        cam.disableMouseInput();
        cam.reset();
    }
    else if(key == '2'){
        mode = FAT_LINE_MODE;
        cam.disableMouseInput();
        cam.reset();
    }
    else if(key == '3'){
        mode = TUBE_MESH_MODE;
        cam.reset();
        cam.enableMouseInput();
    }
    else if(key == 'h'){
        bdrawBezierInfo = !bdrawBezierInfo;
        myBezier.setReactToMouseAndKeyEvents(bdrawBezierInfo);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
