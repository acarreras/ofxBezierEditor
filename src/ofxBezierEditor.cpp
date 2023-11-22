#include "ofxBezierEditor.h"

ofxBezierEditor::ofxBezierEditor(){
    
    setRadiusVertex(8);
    setRadiusControlPoints(6);
    
    currentPointToMove = 0;
    lastVertexSelected = 0;
    
    draggableVertex vtx;
    curveVertices.assign(0, vtx);
    controlPoint1.assign(0, vtx);
    controlPoint2.assign(0, vtx);
    
    
    bfillBezier = true;
    colorFill = ofColor(188,4,62, 100);
    colorStroke = ofColor(2,189,190, 100);
    
    boundingBox.set(0,0,0,0);
    bshowBoundingBox = false;
    center.set(0,0);
    
    beditBezier = false;
    
    
    jsonFileName = "ofxBezierInfo.json";
    
    setReactToMouseAndKeyEvents(true);
    polyLineFromPoints.setClosed(false);
}


//--------------------------------------------------------------







//--------------------------------------------------------------
void ofxBezierEditor::updateBoundingBox(){
    boundingBox = polyLineFromPoints.getBoundingBox();
}

//--------------------------------------------------------------
void ofxBezierEditor::calculateCenter(){
    center.set(boundingBox.x + 0.5f*boundingBox.width, boundingBox.y + 0.5f*boundingBox.height);
}

//--------------------------------------------------------------
void ofxBezierEditor::updatePolyline(){
    polyLineFromPoints.clear();
    if(curveVertices.size() > 0){
        
        for (int i = 0; i < curveVertices.size(); i++){
            if (i == 0){
                polyLineFromPoints.addVertex(curveVertices.at(0).pos.x, curveVertices.at(0).pos.y); // we need to duplicate 0 for the curve to start at point 0
            }
            else{
                polyLineFromPoints.bezierTo(controlPoint1.at(i).pos.x, controlPoint1.at(i).pos.y, controlPoint2.at(i).pos.x, controlPoint2.at(i).pos.y, curveVertices.at(i).pos.x, curveVertices.at(i).pos.y);
            }
        }
        if(bIsClosed){
            polyLineFromPoints.bezierTo(controlPoint1.at(0).pos.x, controlPoint1.at(0).pos.y, controlPoint2.at(0).pos.x, controlPoint2.at(0).pos.y, curveVertices.at(0).pos.x, curveVertices.at(0).pos.y);
            polyLineFromPoints.setClosed(bIsClosed);
        }
        
    }
}


void ofxBezierEditor::updateAllFromVertices(){
    
    updatePolyline();
    
    updateBoundingBox();
    
    calculateCenter();
    
    if(bUseRibbonMesh){
        generateTriangleStripFromPolyline(polyLineFromPoints);
    }
    if(bUseTubeMesh){
        generateTubeMeshFromPolyline(polyLineFromPoints);
    }
    if(bUseRibbonMesh || bUseTubeMesh){
        
    }
    
}





