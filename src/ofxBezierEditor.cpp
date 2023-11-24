#include "ofxBezierEditor.h"


ofxBezierEditor::ofxBezierEditor()
	:   settings(*curveVertices, *controlPoint1, *controlPoint2),
	tubeMeshBuilder(settings),
	ribbonMeshBuilder(settings),
	bezierDraw(settings, *curveVertices, *controlPoint1, *controlPoint2),
	bezierUI(settings, *curveVertices, *controlPoint1, *controlPoint2){

	draggableVertex vtx;
	curveVertices->assign(0, vtx);
	controlPoint1->assign(0, vtx);
	controlPoint2->assign(0, vtx);

	center.set(0, 0);

	polyLineFromPoints.setClosed(false);

	ofAddListener(triggerUpdateEvent, this, &ofxBezierEditor::onTriggerUpdate);

}

ofxBezierEditor::~ofxBezierEditor(){
	ofRemoveListener(triggerUpdateEvent, this, &ofxBezierEditor::onTriggerUpdate);

}

void ofxBezierEditor::updateBoundingBox(){
	settings.boundingBox = polyLineFromPoints.getBoundingBox();
}

void ofxBezierEditor::calculateCenter(){
	center.set(settings.boundingBox.x + 0.5f * settings.boundingBox.width, settings.boundingBox.y + 0.5f * settings.boundingBox.height);
}

void ofxBezierEditor::updatePolyline(){
	polyLineFromPoints.clear();
	if(curveVertices->size() > 0){

		for(int i = 0; i < curveVertices->size(); i++){
			if(i == 0){
				polyLineFromPoints.addVertex(curveVertices->at(0).pos.x, curveVertices->at(0).pos.y); // we need to duplicate 0 for the curve to start at point 0
			}else{
				polyLineFromPoints.bezierTo(controlPoint1->at(i).pos.x, controlPoint1->at(i).pos.y, controlPoint2->at(i).pos.x, controlPoint2->at(i).pos.y, curveVertices->at(i).pos.x, curveVertices->at(i).pos.y);
			}
		}
		if(settings.bIsClosed){
			polyLineFromPoints.bezierTo(controlPoint1->at(0).pos.x, controlPoint1->at(0).pos.y, controlPoint2->at(0).pos.x, controlPoint2->at(0).pos.y, curveVertices->at(0).pos.x, curveVertices->at(0).pos.y);
			polyLineFromPoints.setClosed(settings.bIsClosed);
		}

	}
}


void ofxBezierEditor::updateAllFromVertices(){

	updatePolyline();

	updateBoundingBox();

	calculateCenter();

	if(settings.bUseRibbonMesh){
		ribbonMeshBuilder.generateRibbonMeshFromPolyline(polyLineFromPoints);

	}
	if(settings.bUseTubeMesh){
		tubeMeshBuilder.generateTubeMeshFromPolyline(polyLineFromPoints);
	}
	bezierDraw.updateNormalLine(polyLineFromPoints);

}

void ofxBezierEditor::draw(){
	bezierDraw.draw();
}

void ofxBezierEditor::drawHelp(){
	bezierDraw.drawHelp();
}

void ofxBezierEditor::loadPoints(string filename){
	settings.loadPoints(filename);
}

void ofxBezierEditor::savePoints(string filename){
	settings.savePoints(filename);
}

void ofxBezierEditor::setFillBezier(bool value){
	settings.bfillBezier = value;
	updateAllFromVertices();
}

bool ofxBezierEditor::getFillBezier(){
	return settings.bfillBezier;
}

void ofxBezierEditor::setClosed(bool value){
	settings.bIsClosed = value;
	updateAllFromVertices();
}

bool ofxBezierEditor::getClosed(){
	return settings.bIsClosed;
}

void ofxBezierEditor::setRibbonWidth(float value){
	settings.ribbonWidth = value;
	updateAllFromVertices();
}

float ofxBezierEditor::getRibbonWidth(){
	return settings.ribbonWidth;
}

void ofxBezierEditor::setColorFill(ofColor value){
	settings.colorFill = value;
	updateAllFromVertices();
}

ofColor ofxBezierEditor::getColourFill(){
	return settings.colorFill;
}

void ofxBezierEditor::setColorStroke(ofColor value){
	settings.colorStroke = value;
	updateAllFromVertices();
}

ofColor ofxBezierEditor::getColourStroke(){
	return settings.colorStroke;
}

void ofxBezierEditor::setMeshLengthPrecisionMultiplier(int value){
	settings.meshLengthPrecisionMultiplier = value;
	updateAllFromVertices();
}

int ofxBezierEditor::getMeshLengthPrecisionMultiplier(){
	return settings.meshLengthPrecisionMultiplier;
}

void ofxBezierEditor::setTubeRadius(float value){
	settings.tubeRadius = value;
	updateAllFromVertices();
}

float ofxBezierEditor::getTubeRadius(){
	return settings.tubeRadius;
}

void ofxBezierEditor::setTubeResolution(int value){
	settings.tubeResolution = value;
	updateAllFromVertices();
}

int ofxBezierEditor::getTubeResolution(){
	return settings.tubeResolution;
}

void ofxBezierEditor::setHasRoundCaps(bool value){
	settings.roundCap = value;
	updateAllFromVertices();
}

bool ofxBezierEditor::getHasRoundCaps(){
	return settings.roundCap;
}

void ofxBezierEditor::setUseRibbonMesh(bool value){
	settings.bUseRibbonMesh = value;
	updateAllFromVertices();
}

bool ofxBezierEditor::getUseRibbonMesh(){
	return settings.bUseRibbonMesh;
}

void ofxBezierEditor::setUseTubeMesh(bool value){
	settings.bUseTubeMesh = value;
}

bool ofxBezierEditor::getUseTubeMesh(){
	return settings.bUseTubeMesh;
}

ofVboMesh ofxBezierEditor::getRibbonMeshFromPolyline(ofPolyline polyline){
	return ribbonMeshBuilder.getRibbonMeshFromPolyline(polyline);
}

ofVboMesh ofxBezierEditor::getTubeMeshFromPolyline(ofPolyline polyline){
	return tubeMeshBuilder.getTubeMeshFromPolyline(polyline);
}

ofVboMesh ofxBezierEditor::getRibbonMesh(){
	return ribbonMeshBuilder.getRibbonMesh();
}

ofVboMesh ofxBezierEditor::getTubeMesh(){
	return tubeMeshBuilder.getTubeMesh();
}

void ofxBezierEditor::setReactToMouseAndKeyEvents(bool value){
	bezierUI.setReactToMouseAndKeyEvents(value);
}

void ofxBezierEditor::onTriggerUpdate(TriggerUpdateEventArgs & args){
	updateAllFromVertices();
}
