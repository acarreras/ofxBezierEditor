#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    myBezier.loadXmlPoints();

    gui = new ofxUICanvas(0, 0, ofGetWidth()/4, ofGetHeight());
    gui->addFPSSlider("FPS SLIDER", 200);

    gui->addLabel("SHAPE", OFX_UI_FONT_MEDIUM);
    gui->addToggle("fill", false, 12, 12);
    gui->addSlider("stroke", 1.0, 11.0, 1.0);
    gui->addLabel("FILL COLOR", OFX_UI_FONT_MEDIUM);
    gui->addSlider("fill red", 0.0, 255.0, 128.0);
    gui->addSlider("fill green", 0.0, 255.0, 128.0);
    gui->addSlider("fill blue", 0.0, 255.0, 128.0);
    gui->addLabel("CONTOUR COLOR", OFX_UI_FONT_MEDIUM);
    gui->addSlider("stroke red", 0.0, 255.0, 128.0);
    gui->addSlider("stroke green", 0.0, 255.0, 128.0);
    gui->addSlider("stroke blue", 0.0, 255.0, 128.0);

    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    myBezier.draw();
    if(bdrawBezierInfo){
        myBezier.drawHelp();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == 'h'){
        bdrawBezierInfo = !bdrawBezierInfo;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
void testApp::exit(){
    gui->saveSettings("guiSettings.xml");
	delete gui;
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e){
	string name = e.getName();
	int kind = e.getKind();

	if(name == "fill"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
		myBezier.setFillBezier(toggle->getValue());
	}
	if(name == "stroke"){
	    ofxUISlider *slider = (ofxUISlider *) e.widget;
		myBezier.setStrokeWeight(slider->getScaledValue());
	}
	if(name == "fill red"){
	    ofxUISlider *slider = (ofxUISlider *) e.widget;
		myBezier.setColorFillR(slider->getScaledValue());
	}
	if(name == "fill green"){
	    ofxUISlider *slider = (ofxUISlider *) e.widget;
		myBezier.setColorFillG(slider->getScaledValue());
	}
	if(name == "fill blue"){
	    ofxUISlider *slider = (ofxUISlider *) e.widget;
		myBezier.setColorFillB(slider->getScaledValue());
	}
	if(name == "stroke red"){
	    ofxUISlider *slider = (ofxUISlider *) e.widget;
		myBezier.setColorStrokeR(slider->getScaledValue());
	}
	if(name == "stroke green"){
	    ofxUISlider *slider = (ofxUISlider *) e.widget;
		myBezier.setColorStrokeG(slider->getScaledValue());
	}
	if(name == "stroke blue"){
	    ofxUISlider *slider = (ofxUISlider *) e.widget;
		myBezier.setColorStrokeB(slider->getScaledValue());
	}
}
