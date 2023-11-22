//
//  ofxBezierDraw.cpp
//  example-ui
//
//  Created by Fred Rodrigues on 22/11/2023.
//

#include "ofxBezierDraw.h"

ofxBezierDraw::ofxBezierDraw() {
    // Initialization
}

ofxBezierDraw::~ofxBezierDraw() {
    // Cleanup
}
//--------------------------------------------------------------
void ofxBezierDraw::draw(){
    
    if(curveVertices.size() > 0){
        ofPushMatrix();
        if(bfillBezier && bIsClosed){
            ofFill();
            ofSetColor(colorFill);
            ofBeginShape();
            for (int i = 0; i < curveVertices.size(); i++){
                if (i == 0){
                    ofVertex(curveVertices.at(0).pos.x, curveVertices.at(0).pos.y); // we need to duplicate 0 for the curve to start at point 0
                }
                else {
                    ofBezierVertex(controlPoint1.at(i).pos.x, controlPoint1.at(i).pos.y, controlPoint2.at(i).pos.x, controlPoint2.at(i).pos.y, curveVertices.at(i).pos.x, curveVertices.at(i).pos.y);
                }
            }
            
            ofBezierVertex(controlPoint1.at(0).pos.x, controlPoint1.at(0).pos.y, controlPoint2.at(0).pos.x, controlPoint2.at(0).pos.y, curveVertices.at(0).pos.x, curveVertices.at(0).pos.y);
            
            ofEndShape(bIsClosed);
        }
        
        
        ofSetColor(colorStroke);
        ofNoFill();
        ofBeginShape();
        for (int i = 0; i < curveVertices.size(); i++){
            if (i == 0){
                ofVertex(curveVertices.at(0).pos.x, curveVertices.at(0).pos.y); // we need to duplicate 0 for the curve to start at point 0
            }
            else {
                ofBezierVertex(controlPoint1.at(i).pos.x, controlPoint1.at(i).pos.y, controlPoint2.at(i).pos.x, controlPoint2.at(i).pos.y, curveVertices.at(i).pos.x, curveVertices.at(i).pos.y);
            }
        }
        if(bIsClosed){
            
            ofBezierVertex(controlPoint1.at(0).pos.x, controlPoint1.at(0).pos.y, controlPoint2.at(0).pos.x, controlPoint2.at(0).pos.y, curveVertices.at(0).pos.x, curveVertices.at(0).pos.y);
        }
        ofEndShape(bIsClosed);
        ofPopMatrix();
    }
    
    ofPopStyle(); // Restore the previous style settings
    
}

//--------------------------------------------------------------
void ofxBezierDraw::drawOutline(){
    
    if(curveVertices.size() > 0){
        
        ofSetColor(colorStroke);
        ofNoFill();
        ofBeginShape();
        for (int i = 0; i < curveVertices.size(); i++){
            if (i == 0){
                ofVertex(curveVertices.at(0).pos.x, curveVertices.at(0).pos.y); // we need to duplicate 0 for the curve to start at point 0
            }
            else {
                ofBezierVertex(controlPoint1.at(i).pos.x, controlPoint1.at(i).pos.y, controlPoint2.at(i).pos.x, controlPoint2.at(i).pos.y, curveVertices.at(i).pos.x, curveVertices.at(i).pos.y);
            }
        }
        if(bIsClosed){
            ofBezierVertex(controlPoint1.at(0).pos.x, controlPoint1.at(0).pos.y, controlPoint2.at(0).pos.x, controlPoint2.at(0).pos.y, curveVertices.at(0).pos.x, curveVertices.at(0).pos.y);
        }
        ofEndShape(bIsClosed);
        ofPopMatrix();
    }
    
}

//--------------------------------------------------------------
void ofxBezierDraw::drawHelp(){
    ofSetBackgroundColor(200);
    if(curveVertices.size() > 0){
        draw();
        
        drawWithNormals(polyLineFromPoints);
        ofPushMatrix();
        ofSetLineWidth(1);
        ofNoFill();
        ofSetColor(0,0,0,40);
        for (int i = 0; i < curveVertices.size(); i++){
            ofDrawLine(curveVertices.at(i).pos.x, curveVertices.at(i).pos.y, controlPoint2.at(i).pos.x, controlPoint2.at(i).pos.y);
        }
        for (int i = 1; i < curveVertices.size(); i++){
            ofDrawLine(curveVertices.at(i-1).pos.x, curveVertices.at(i-1).pos.y, controlPoint1.at(i).pos.x, controlPoint1.at(i).pos.y);
        }
        ofDrawLine(curveVertices.at(curveVertices.size()-1).pos.x, curveVertices.at(curveVertices.size()-1).pos.y, controlPoint1.at(0).pos.x, controlPoint1.at(0).pos.y); // =
        
        
        for (int i = 0; i < curveVertices.size(); i++){
            ofSetColor(vertexColour);
            ofNoFill();
            if(curveVertices.at(i).bOver == true){
                ofSetColor(vertexHoverColor);
                ofFill();
            }
            if(curveVertices.at(i).bBeingDragged == true){
                ofSetColor(vertexDraggedColour);
                ofFill();
            }
            ofDrawCircle(curveVertices.at(i).pos.x, curveVertices.at(i).pos.y, radiusVertex);
            if(curveVertices.at(i).bBeingSelected == true){
                ofSetColor(vertexSelectedColour);
                ofFill();
                ofDrawCircle(curveVertices.at(i).pos.x, curveVertices.at(i).pos.y, radiusVertex);
                ofNoFill();
                ofDrawCircle(curveVertices.at(i).pos.x, curveVertices.at(i).pos.y, 2*radiusVertex);
            }
            ofSetColor(vertexLabelColour);
            ofDrawBitmapString("v_" + ofToString(i), curveVertices.at(i).pos.x+3, curveVertices.at(i).pos.y+3);
        }
        
        
        for (int i = 0; i < controlPoint1.size(); i++){
            ofSetColor(ctrPtColour);
            ofNoFill();
            if(controlPoint1.at(i).bOver == true){
                ofSetColor(ctrPtHoverColor);
                ofFill();
            }
            if(controlPoint1.at(i).bBeingDragged == true){
                ofSetColor(ctrPtDraggedColour);
                ofFill();
            }
            ofDrawCircle(controlPoint1.at(i).pos.x, controlPoint1.at(i).pos.y, radiusControlPoints);
            ofSetColor(ctrPtLabelColour);
            ofDrawBitmapString("cp1_" + ofToString(i), controlPoint1.at(i).pos.x+3, controlPoint1.at(i).pos.y+3);
        }
        
        for (int i = 0; i < controlPoint2.size(); i++){
            ofSetColor(ctrPtColour);
            ofNoFill();
            if(controlPoint2.at(i).bOver == true){
                ofSetColor(ctrPtHoverColor);
                ofFill();
            }
            if(controlPoint2.at(i).bBeingDragged == true){
                ofSetColor(ctrPtDraggedColour);
                ofFill();
            }
            ofDrawCircle(controlPoint2.at(i).pos.x, controlPoint2.at(i).pos.y, radiusControlPoints);
            ofSetColor(ctrPtLabelColour);
            ofDrawBitmapString("cp2_" + ofToString(i), controlPoint2.at(i).pos.x+3, controlPoint2.at(i).pos.y+3);
        }
        
        int range = currentPointToMove / curveVertices.size();
        int mod = currentPointToMove % curveVertices.size();
        ofNoFill();
        if(range == 0){
            ofSetColor(255,255,0);
            ofDrawCircle(curveVertices.at(mod).pos.x, curveVertices.at(mod).pos.y, 2*radiusControlPoints);
            ofSetColor(255,255,0);
            ofDrawBitmapString("fine tune with arrows", curveVertices.at(mod).pos.x+3, curveVertices.at(mod).pos.y+3);
        }
        else if(range == 1){
            ofSetColor(255,0,255);
            ofDrawCircle(controlPoint1.at(mod).pos.x, controlPoint1.at(mod).pos.y, 2*radiusControlPoints);
            ofSetColor(255,0,255);
            ofDrawBitmapString("fine tune with arrows", controlPoint1.at(mod).pos.x+3, controlPoint1.at(mod).pos.y+3);
        }
        else if(range == 2){
            ofSetColor(255,0,0);
            ofDrawCircle(controlPoint2.at(mod).pos.x, controlPoint2.at(mod).pos.y, 2*radiusControlPoints);
            ofSetColor(255,0,0);
            ofDrawBitmapString("fine tune with arrows", controlPoint2.at(mod).pos.x+3, controlPoint2.at(mod).pos.y+3);
        }
        
        if(bshowBoundingBox){
            ofFill();
            ofSetColor(10,250,255,150);
            ofDrawRectangle(boundingBox);
            ofNoFill();
            ofSetLineWidth(2);
            ofSetColor(10,250,255);
            ofDrawRectangle(boundingBox);
        }
        ofPopMatrix();
        
        
        
    }
    
    ofSetColor(0,0,0);
    ofDrawBitmapString("VERTEX: " + ofToString(curveVertices.size()) + "PRESS e to EDIT the BEZIER: " + ofToString(beditBezier), 20,20);
    ofDrawBitmapString("mouse left button to add a point at the end", 20,40);
    ofDrawBitmapString("backspace to delete last point added", 20,60);
    ofDrawBitmapString("drag mouse to move vertex and control points", 20,80);
    ofDrawBitmapString("mouse right button to select two vertex", 20,100);
    ofDrawBitmapString("mouse right button to add a new vertex between the two selected vertex", 20,120);
    ofDrawBitmapString("supr to delete last vertex added", 20,140);
    ofDrawBitmapString("currentPointToMove [n++|m--]: " + ofToString(currentPointToMove) + "\n fine tune with arrows", 20,160);
    ofDrawBitmapString("[l] load stored bezier", 20,200);
    ofDrawBitmapString("[s] save current bezier", 20,220);
    ofDrawBitmapString("[f] toogle fill: " + ofToString(bfillBezier), 20,240);
    ofDrawBitmapString("[b] show/hide bounding box: " + ofToString(bshowBoundingBox), 20,260);
    ofDrawBitmapString("drag bounding box to move all the bezier", 20,280);
    ofDrawBitmapString("COLORS:\nfill " + ofToString((float)colorFill.r) + "(r) " + ofToString((float)colorFill.g) + "(g) " + ofToString((float)colorFill.b) + "(b)" + "\nstroke " + ofToString((float)colorStroke.r) + "(r) " + ofToString((float)colorStroke.g) + "(g) " + ofToString((float)colorStroke.b) + "(b)", 20,300);
    
}

void ofxBezierDraw::setColorFill(ofColor c){
    colorFill = c;
}
void ofxBezierDraw::setColorFill(float r, float g, float b, float a){
    colorFill = ofColor(r,g,b,a);
}
void ofxBezierDraw::setColorFillR(float c){
    colorFill.r = c;
}
void ofxBezierDraw::setColorFillG(float c){
    colorFill.g = c;
}
void ofxBezierDraw::setColorFillB(float c){
    colorFill.b = c;
}
void ofxBezierDraw::setColorFillA(float a){
    colorFill.a = a;
}
void ofxBezierDraw::setColorStroke(ofColor c){
    colorStroke = c;
}
void ofxBezierDraw::setColorStroke(float r, float g, float b, float a){
    colorStroke = ofColor(r,g,b,a);
}
void ofxBezierDraw::setColorStrokeR(float c){
    colorStroke.r = c;
}
void ofxBezierDraw::setColorStrokeG(float c){
    colorStroke.g = c;
}
void ofxBezierDraw::setColorStrokeB(float c){
    colorStroke.b = c;
}
void ofxBezierDraw::setColorStrokeA(float a){
    colorStroke.a = a;
}
