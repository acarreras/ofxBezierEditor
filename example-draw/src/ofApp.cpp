#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    bezierEditor.setRibbonWidth(30);
    bezierEditor.setUseRibbonMesh(true);
    bezierEditor.setClosed(false);
    bezierEditor.setHasRoundCaps(true);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    if(ofGetMousePressed()){
        ofSetColor(255, 0, 0);
        drawLine.draw();
    }
    else{
        ofSetColor(0, 255, 0);
        ribbonMesh.draw();
    }
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'c'){
        drawLine.clear();
        ribbonMesh.clear();
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
    drawLine.addVertex(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    ribbonMesh = bezierEditor.getTubeMeshFromPolyline(drawLine);
    cout<<"Released"<<endl;
}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
