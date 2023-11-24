//
//  ofxBezierDraw.cpp
//  example-ui
//
//  Created by Fred Rodrigues on 22/11/2023.
//

#include "ofxBezierDraw.h"

ofxBezierDraw::ofxBezierDraw(ofxBezierEditorSettings & settings,
							 std::vector <draggableVertex> & curveVertices,
							 std::vector <draggableVertex> & controlPoint1,
							 std::vector <draggableVertex> & controlPoint2)
	: settings(settings), curveVertices(curveVertices), controlPoint1(controlPoint1), controlPoint2(controlPoint2){


}

ofxBezierDraw::~ofxBezierDraw(){
}
//--------------------------------------------------------------
void ofxBezierDraw::draw(){

	if(curveVertices.size() > 0){
		ofPushMatrix();
		if(settings.bfillBezier && settings.bIsClosed){
			ofFill();
			ofSetColor(settings.colorFill);
			ofBeginShape();
			for(int i = 0; i < curveVertices.size(); i++){
				if(i == 0){
					ofVertex(curveVertices.at(0).pos.x, curveVertices.at(0).pos.y); // we need to duplicate 0 for the curve to start at point 0
				}else{
					ofBezierVertex(controlPoint1.at(i).pos.x, controlPoint1.at(i).pos.y, controlPoint2.at(i).pos.x, controlPoint2.at(i).pos.y, curveVertices.at(i).pos.x, curveVertices.at(i).pos.y);
				}
			}

			ofBezierVertex(controlPoint1.at(0).pos.x, controlPoint1.at(0).pos.y, controlPoint2.at(0).pos.x, controlPoint2.at(0).pos.y, curveVertices.at(0).pos.x, curveVertices.at(0).pos.y);

			ofEndShape(settings.bIsClosed);
		}


		ofSetColor(settings.colorStroke);
		ofNoFill();
		ofBeginShape();
		for(int i = 0; i < curveVertices.size(); i++){
			if(i == 0){
				ofVertex(curveVertices.at(0).pos.x, curveVertices.at(0).pos.y); // we need to duplicate 0 for the curve to start at point 0
			}else{
				ofBezierVertex(controlPoint1.at(i).pos.x, controlPoint1.at(i).pos.y, controlPoint2.at(i).pos.x, controlPoint2.at(i).pos.y, curveVertices.at(i).pos.x, curveVertices.at(i).pos.y);
			}
		}
		if(settings.bIsClosed){

			ofBezierVertex(controlPoint1.at(0).pos.x, controlPoint1.at(0).pos.y, controlPoint2.at(0).pos.x, controlPoint2.at(0).pos.y, curveVertices.at(0).pos.x, curveVertices.at(0).pos.y);
		}
		ofEndShape(settings.bIsClosed);
		ofPopMatrix();
	}

	ofPopStyle(); // Restore the previous style settings

}

//--------------------------------------------------------------
void ofxBezierDraw::drawOutline(){

	if(curveVertices.size() > 0){

		ofSetColor(settings.colorStroke);
		ofNoFill();
		ofBeginShape();
		for(int i = 0; i < curveVertices.size(); i++){
			if(i == 0){
				ofVertex(curveVertices.at(0).pos.x, curveVertices.at(0).pos.y); // we need to duplicate 0 for the curve to start at point 0
			}else{
				ofBezierVertex(controlPoint1.at(i).pos.x, controlPoint1.at(i).pos.y, controlPoint2.at(i).pos.x, controlPoint2.at(i).pos.y, curveVertices.at(i).pos.x, curveVertices.at(i).pos.y);
			}
		}
		if(settings.bIsClosed){
			ofBezierVertex(controlPoint1.at(0).pos.x, controlPoint1.at(0).pos.y, controlPoint2.at(0).pos.x, controlPoint2.at(0).pos.y, curveVertices.at(0).pos.x, curveVertices.at(0).pos.y);
		}
		ofEndShape(settings.bIsClosed);
		ofPopMatrix();
	}

}

void ofxBezierDraw::drawWithNormals(const ofPolyline & polyline){

	ofPushStyle(); // Push the current style settings
	for(int i = 0; i < polyline.size(); i++){
		const ofPoint & cur = polyline[i];
		const ofPoint & normal = polyline.getNormalAtIndex(i);

		ofPushMatrix();
		ofTranslate(cur.x, cur.y, 0);

		ofSetColor(255, 0, 255); // Set color before drawing lines

		ofDrawLine(0, 0, normal.x * 20, normal.y * 20); // Draw normals

		ofPopMatrix();
	}
}
//--------------------------------------------------------------
void ofxBezierDraw::drawHelp(){
	ofSetBackgroundColor(200);
	if(curveVertices.size() > 0){
		draw();

		drawWithNormals(normalLine);
		ofPushMatrix();
		ofSetLineWidth(1);
		ofNoFill();
		ofSetColor(0, 0, 0, 40);
		for(int i = 0; i < curveVertices.size(); i++){
			ofDrawLine(curveVertices.at(i).pos.x, curveVertices.at(i).pos.y, controlPoint2.at(i).pos.x, controlPoint2.at(i).pos.y);
		}
		for(int i = 1; i < curveVertices.size(); i++){
			ofDrawLine(curveVertices.at(i - 1).pos.x, curveVertices.at(i - 1).pos.y, controlPoint1.at(i).pos.x, controlPoint1.at(i).pos.y);
		}
		ofDrawLine(curveVertices.at(curveVertices.size() - 1).pos.x, curveVertices.at(curveVertices.size() - 1).pos.y, controlPoint1.at(0).pos.x, controlPoint1.at(0).pos.y); // =


		for(int i = 0; i < curveVertices.size(); i++){
			ofSetColor(settings.vertexColour);
			ofNoFill();
			if(curveVertices.at(i).bOver == true){
				ofSetColor(settings.vertexHoverColor);
				ofFill();
			}
			if(curveVertices.at(i).bBeingDragged == true){
				ofSetColor(settings.vertexDraggedColour);
				ofFill();
			}
			ofDrawCircle(curveVertices.at(i).pos.x, curveVertices.at(i).pos.y, settings.radiusVertex);
			if(curveVertices.at(i).bBeingSelected == true){
				ofSetColor(settings.vertexSelectedColour);
				ofFill();
				ofDrawCircle(curveVertices.at(i).pos.x, curveVertices.at(i).pos.y, settings.radiusVertex);
				ofNoFill();
				ofDrawCircle(curveVertices.at(i).pos.x, curveVertices.at(i).pos.y, 2 * settings.radiusVertex);
			}
			ofSetColor(settings.vertexLabelColour);
			ofDrawBitmapString("v_" + ofToString(i), curveVertices.at(i).pos.x + 3, curveVertices.at(i).pos.y + 3);
		}


		for(int i = 0; i < controlPoint1.size(); i++){
			ofSetColor(settings.ctrPtColour);
			ofNoFill();
			if(controlPoint1.at(i).bOver == true){
				ofSetColor(settings.ctrPtHoverColor);
				ofFill();
			}
			if(controlPoint1.at(i).bBeingDragged == true){
				ofSetColor(settings.ctrPtDraggedColour);
				ofFill();
			}
			ofDrawCircle(controlPoint1.at(i).pos.x, controlPoint1.at(i).pos.y, settings.radiusControlPoints);
			ofSetColor(settings.ctrPtLabelColour);
			ofDrawBitmapString("cp1_" + ofToString(i), controlPoint1.at(i).pos.x + 3, controlPoint1.at(i).pos.y + 3);
		}

		for(int i = 0; i < controlPoint2.size(); i++){
			ofSetColor(settings.ctrPtColour);
			ofNoFill();
			if(controlPoint2.at(i).bOver == true){
				ofSetColor(settings.ctrPtHoverColor);
				ofFill();
			}
			if(controlPoint2.at(i).bBeingDragged == true){
				ofSetColor(settings.ctrPtDraggedColour);
				ofFill();
			}
			ofDrawCircle(controlPoint2.at(i).pos.x, controlPoint2.at(i).pos.y, settings.radiusControlPoints);
			ofSetColor(settings.ctrPtLabelColour);
			ofDrawBitmapString("cp2_" + ofToString(i), controlPoint2.at(i).pos.x + 3, controlPoint2.at(i).pos.y + 3);
		}

		int range = settings.currentPointToMove / curveVertices.size();
		int mod = settings.currentPointToMove % curveVertices.size();

		ofNoFill();

		if(range == 0){
			ofSetColor(255, 255, 0);
			ofDrawCircle(curveVertices.at(mod).pos.x, curveVertices.at(mod).pos.y, 2 * settings.radiusControlPoints);
			ofSetColor(255, 255, 0);
			ofDrawBitmapString("fine tune with arrows", curveVertices.at(mod).pos.x + 3, curveVertices.at(mod).pos.y + 3);
		}else if(range == 1){
			ofSetColor(255, 0, 255);
			ofDrawCircle(controlPoint1.at(mod).pos.x, controlPoint1.at(mod).pos.y, 2 * settings.radiusControlPoints);
			ofSetColor(255, 0, 255);
			ofDrawBitmapString("fine tune with arrows", controlPoint1.at(mod).pos.x + 3, controlPoint1.at(mod).pos.y + 3);
		}else if(range == 2){
			ofSetColor(255, 0, 0);
			ofDrawCircle(controlPoint2.at(mod).pos.x, controlPoint2.at(mod).pos.y, 2 * settings.radiusControlPoints);
			ofSetColor(255, 0, 0);
			ofDrawBitmapString("fine tune with arrows", controlPoint2.at(mod).pos.x + 3, controlPoint2.at(mod).pos.y + 3);
		}

		if(settings.bshowBoundingBox){
			ofFill();
			ofSetColor(10, 250, 255, 150);
			ofDrawRectangle(settings.boundingBox);
			ofNoFill();
			ofSetLineWidth(2);
			ofSetColor(10, 250, 255);
			ofDrawRectangle(settings.boundingBox);
		}
		ofPopMatrix();
	}

	ofSetColor(0, 0, 0);
	ofDrawBitmapString("VERTEX: " + ofToString(curveVertices.size()) + " PRESS e to EDIT the BEZIER: " + ofToString(settings.beditBezier), 20, 20);
	ofDrawBitmapString("mouse left button to add a point at the end", 20, 40);
	ofDrawBitmapString("backspace to delete last point added", 20, 60);
	ofDrawBitmapString("drag mouse to move vertex and control points", 20, 80);
	ofDrawBitmapString("mouse right button to select two vertex", 20, 100);
	ofDrawBitmapString("mouse right button to add a new vertex between the two selected vertex", 20, 120);
	ofDrawBitmapString("supr to delete last vertex added", 20, 140);
	ofDrawBitmapString("currentPointToMove [n++|m--]: " + ofToString(settings.currentPointToMove) + "\n fine tune with arrows", 20, 160);
	ofDrawBitmapString("[l] load stored bezier", 20, 200);
	ofDrawBitmapString("[s] save current bezier", 20, 220);
	ofDrawBitmapString("[f] toogle fill: " + ofToString(settings.bfillBezier), 20, 240);
	ofDrawBitmapString("[b] show/hide bounding box: " + ofToString(settings.bshowBoundingBox), 20, 260);
	ofDrawBitmapString("drag bounding box to move all the bezier", 20, 280);
	ofDrawBitmapString("COLORS:\nfill " + ofToString((float)settings.colorFill.r) + "(r) " + ofToString((float)settings.colorFill.g) + "(g) " + ofToString((float)settings.colorFill.b) + "(b)" + "\nstroke " + ofToString((float)settings.colorStroke.r) + "(r) " + ofToString((float)settings.colorStroke.g) + "(g) " + ofToString((float)settings.colorStroke.b) + "(b)", 20, 300);

}

void ofxBezierDraw::updateNormalLine(ofPolyline line){
	normalLine = line;
}
