#include "ofxBezierEditor.h"

//--------------------------------------------------------------
void drawWithNormals(const ofPolyline& polyline) {
	for(int i=0; i< (int) polyline.size(); i++ ) {
		bool repeatNext = i == (int)polyline.size() - 1;

		const ofPoint& cur = polyline[i];
		const ofPoint& next = repeatNext ? polyline[0] : polyline[i + 1];

		float angle = atan2f(next.y - cur.y, next.x - cur.x) * RAD_TO_DEG;
		float distance = cur.distance(next);

		if(repeatNext) {
			ofSetColor(255, 0, 255);
		}
		glPushMatrix();
		glTranslatef(cur.x, cur.y, 0);
		ofRotate(angle);
		ofLine(0, 0, 0, distance); // normals
		ofLine(0, 0, distance, 0);
		glPopMatrix();
	}
}

ofxBezierEditor::~ofxBezierEditor(){
    ofUnregisterMouseEvents(this);
    ofUnregisterKeyEvents(this);
}

ofxBezierEditor::ofxBezierEditor(){
    setRadiusVertex(6);
    setRadiusControlPoints(4);

    currentPointToMove = 0;
    lastVertexSelected = 0;

    draggableVertex vtx;
    curveVertices.assign(0, vtx);
	controlPoint1.assign(0, vtx);
	controlPoint2.assign(0, vtx);

	bfillBezier = true;
    colorFill = ofColor(188,4,62, 100);
	colorStroke = ofColor(2,189,190, 100);
	strokeWeight = 1;

	boundingBox.set(0,0,0,0);
	bshowBoundingBox = false;
    center.set(0,0);

	beditBezier = true;

	translateX = translateY = 0;

	ofRegisterMouseEvents(this);
	ofRegisterKeyEvents(this);
}

//--------------------------------------------------------------
void ofxBezierEditor::loadXmlPoints(){
    XMLbezier.loadFile("ofxBezierInfo.xml");
    curveVertices.clear();
    controlPoint1.clear();
    controlPoint2.clear();

    bfillBezier = XMLbezier.getValue("bezier:fill", true);
	colorFill.set(XMLbezier.getValue("bezier:colorFill:r", 200), XMLbezier.getValue("bezier:colorFill:g", 200), XMLbezier.getValue("bezier:colorFill:b", 200));
	colorStroke.set(XMLbezier.getValue("bezier:colorStroke:r", 100), XMLbezier.getValue("bezier:colorStroke:g", 100), XMLbezier.getValue("bezier:colorStroke:b", 100));
	strokeWeight = XMLbezier.getValue("bezier:strokeWeight", 1);

	int nCurveVertices = XMLbezier.getNumTags("vertex");
    if(nCurveVertices > 0){
		for(int i=0; i<nCurveVertices; i++){
            draggableVertex vtx;
            vtx.x = XMLbezier.getValue("vertex:x", 0, i);
            vtx.y = XMLbezier.getValue("vertex:y", 0, i);
            vtx.bOver 			= false;
            vtx.bBeingDragged 	= false;
            vtx.bBeingSelected 	= false;
            curveVertices.push_back(vtx);
        }
		XMLbezier.popTag();
	}

	int nControlPoints1 = XMLbezier.getNumTags("cp1");
    if(nCurveVertices > 0){
		for(int i=0; i<nControlPoints1; i++){
            draggableVertex cp;
            cp.x = XMLbezier.getValue("cp1:x", 0, i);
            cp.y = XMLbezier.getValue("cp1:y", 0, i);
            cp.bOver 			= false;
            cp.bBeingDragged 	= false;
            controlPoint1.push_back(cp);
        }
		XMLbezier.popTag();
	}

	int nControlPoints2 = XMLbezier.getNumTags("cp2");
    if(nCurveVertices > 0){
		for(int i=0; i<nControlPoints2; i++){
            draggableVertex cp;
            cp.x = XMLbezier.getValue("cp2:x", 0, i);
            cp.y = XMLbezier.getValue("cp2:y", 0, i);
            cp.bOver 			= false;
            cp.bBeingDragged 	= false;
            controlPoint2.push_back(cp);
        }
		XMLbezier.popTag();
	}

	updateBoundingBox();
	calculateCenter();
}

//--------------------------------------------------------------
void ofxBezierEditor::saveXmlPoints(){
    XMLbezier.clear();

    XMLbezier.addTag("bezier");
	XMLbezier.setValue("bezier:fill", bfillBezier);
	XMLbezier.setValue("bezier:colorFill:r", colorFill.r);
	XMLbezier.setValue("bezier:colorFill:g", colorFill.g);
	XMLbezier.setValue("bezier:colorFill:b", colorFill.b);
	XMLbezier.setValue("bezier:colorStroke:r", colorStroke.r);
	XMLbezier.setValue("bezier:colorStroke:g", colorStroke.g);
	XMLbezier.setValue("bezier:colorStroke:b", colorStroke.b);
	XMLbezier.setValue("bezier:strokeWeight", strokeWeight);

    for(int i=0; i<curveVertices.size(); i++){
        int tagNum = XMLbezier.addTag("vertex");
		XMLbezier.setValue("vertex:x", curveVertices.at(i).x + translateX, tagNum);
		XMLbezier.setValue("vertex:y", curveVertices.at(i).y + translateY, tagNum);

		XMLbezier.popTag();
    }
    for(int i=0; i<controlPoint1.size(); i++){
        int tagNum = XMLbezier.addTag("cp1");
		XMLbezier.setValue("cp1:x", controlPoint1.at(i).x + translateX, tagNum);
		XMLbezier.setValue("cp1:y", controlPoint1.at(i).y + translateY, tagNum);

		XMLbezier.popTag();
    }
    for(int i=0; i<controlPoint2.size(); i++){
        int tagNum = XMLbezier.addTag("cp2");
		XMLbezier.setValue("cp2:x", controlPoint2.at(i).x + translateX, tagNum);
		XMLbezier.setValue("cp2:y", controlPoint2.at(i).y + translateY, tagNum);

		XMLbezier.popTag();
    }

    XMLbezier.saveFile("ofxBezierInfo.xml");

}

//--------------------------------------------------------------
void ofxBezierEditor::draw(){
    ofEnableAlphaBlending();

    if(curveVertices.size() > 0){
        ofPushMatrix();
        ofTranslate(translateX, translateY);
        if(bfillBezier){
            ofFill();
            ofSetColor(colorFill);
            ofBeginShape();
            for (int i = 0; i < curveVertices.size(); i++){
                if (i == 0){
                    ofVertex(curveVertices.at(0).x, curveVertices.at(0).y); // we need to duplicate 0 for the curve to start at point 0
                    }
                else {
                    ofBezierVertex(controlPoint1.at(i).x, controlPoint1.at(i).y, controlPoint2.at(i).x, controlPoint2.at(i).y, curveVertices.at(i).x, curveVertices.at(i).y);
                }
            }
            ofBezierVertex(controlPoint1.at(0).x, controlPoint1.at(0).y, controlPoint2.at(0).x, controlPoint2.at(0).y, curveVertices.at(0).x, curveVertices.at(0).y);
            ofEndShape(true);
        }

        ofSetLineWidth(strokeWeight);
        ofSetColor(colorStroke);
        ofNoFill();
        ofBeginShape();
        for (int i = 0; i < curveVertices.size(); i++){
            if (i == 0){
                ofVertex(curveVertices.at(0).x, curveVertices.at(0).y); // we need to duplicate 0 for the curve to start at point 0
                }
            else {
                ofBezierVertex(controlPoint1.at(i).x, controlPoint1.at(i).y, controlPoint2.at(i).x, controlPoint2.at(i).y, curveVertices.at(i).x, curveVertices.at(i).y);
            }
        }
        ofBezierVertex(controlPoint1.at(0).x, controlPoint1.at(0).y, controlPoint2.at(0).x, controlPoint2.at(0).y, curveVertices.at(0).x, curveVertices.at(0).y);
        ofEndShape(true);
        ofPopMatrix();
    } // end of if(curveVertices.size() > 0){

    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void ofxBezierEditor::drawHelp(){
    ofEnableAlphaBlending();
    if(curveVertices.size() > 0){
        ofPushMatrix();
        ofTranslate(translateX, translateY);
        ofSetLineWidth(1);
        ofNoFill();
        ofSetColor(0,0,0,40);
        for (int i = 0; i < curveVertices.size(); i++){
            ofLine(curveVertices.at(i).x, curveVertices.at(i).y, controlPoint2.at(i).x, controlPoint2.at(i).y);
        }
        for (int i = 1; i < curveVertices.size(); i++){
            ofLine(curveVertices.at(i-1).x, curveVertices.at(i-1).y, controlPoint1.at(i).x, controlPoint1.at(i).y);
        }
        ofLine(curveVertices.at(curveVertices.size()-1).x, curveVertices.at(curveVertices.size()-1).y, controlPoint1.at(0).x, controlPoint1.at(0).y); // the last one

//        ofNoFill();
//        ofSetColor(0,0,0,40);
//        for (int i = 1; i < curveVertices.size(); i++){
//            ofLine(curveVertices.at(i).x, curveVertices.at(i).y, curveVertices.at(i-1).x, curveVertices.at(i-1).y);
//        }

        for (int i = 0; i < curveVertices.size(); i++){
            ofSetColor(0,0,0,40);
            ofNoFill();
            if(curveVertices.at(i).bOver == true){
                ofSetColor(0,0,0,80);
                ofFill();
            }
            if(curveVertices.at(i).bBeingDragged == true){
                ofSetColor(2,77,77,200);
                ofFill();
            }
            ofCircle(curveVertices.at(i).x, curveVertices.at(i).y, radiusVertex);
            if(curveVertices.at(i).bBeingSelected == true){
                ofSetColor(2,77,77,200);
                ofFill();
                ofCircle(curveVertices.at(i).x, curveVertices.at(i).y, radiusVertex);
                ofNoFill();
                ofCircle(curveVertices.at(i).x, curveVertices.at(i).y, 2*radiusVertex);
            }
            ofSetColor(0,0,0,80);
            ofDrawBitmapString("v_" + ofToString(i), curveVertices.at(i).x+3, curveVertices.at(i).y+3);
        }

        for (int i = 0; i < controlPoint1.size(); i++){
            ofSetColor(100,53,68,40);
            ofNoFill();
            if(controlPoint1.at(i).bOver == true){
                ofSetColor(100,53,68,80);
                ofFill();
            }
            if(controlPoint1.at(i).bBeingDragged == true){
                ofSetColor(95,52,98,200);
                ofFill();
            }
            ofCircle(controlPoint1.at(i).x, controlPoint1.at(i).y, radiusControlPoints);
            ofSetColor(100,53,68,80);
            ofDrawBitmapString("cp1_" + ofToString(i), controlPoint1.at(i).x+3, controlPoint1.at(i).y+3);
        }

        for (int i = 0; i < controlPoint2.size(); i++){
            ofSetColor(100,53,68,40);
            ofNoFill();
            if(controlPoint2.at(i).bOver == true){
                ofSetColor(100,53,68,80);
                ofFill();
            }
            if(controlPoint2.at(i).bBeingDragged == true){
                ofSetColor(95,52,98,200);
                ofFill();
            }
            ofCircle(controlPoint2.at(i).x, controlPoint2.at(i).y, radiusControlPoints);
            ofSetColor(100,53,68,80);
            ofDrawBitmapString("cp2_" + ofToString(i), controlPoint2.at(i).x+3, controlPoint2.at(i).y+3);
        }

        int range = currentPointToMove / curveVertices.size();
        int mod = currentPointToMove % curveVertices.size();
        ofNoFill();
        if(range == 0){
            ofSetColor(40,103,99);
            ofCircle(curveVertices.at(mod).x, curveVertices.at(mod).y, 2*radiusControlPoints);
            ofSetColor(40,103,99, 100);
            ofDrawBitmapString("fine tune with arrows", curveVertices.at(mod).x+3, curveVertices.at(mod).y+3);
        }
        else if(range == 1){
            ofSetColor(40,103,99);
            ofCircle(controlPoint1.at(mod).x, controlPoint1.at(mod).y, 2*radiusControlPoints);
            ofSetColor(40,103,99, 100);
            ofDrawBitmapString("fine tune with arrows", controlPoint1.at(mod).x+3, controlPoint1.at(mod).y+3);
        }
        else if(range == 2){
            ofSetColor(40,103,99);
            ofCircle(controlPoint2.at(mod).x, controlPoint2.at(mod).y, 2*radiusControlPoints);
            ofSetColor(40,103,99, 100);
            ofDrawBitmapString("fine tune with arrows", controlPoint2.at(mod).x+3, controlPoint2.at(mod).y+3);
        }

        if(bshowBoundingBox){
            ofFill();
            ofSetColor(10,250,255,150);
            ofRect(boundingBox);
            ofNoFill();
            ofSetLineWidth(2);
            ofSetColor(10,250,255);
            ofRect(boundingBox);
        }
        ofPopMatrix();

        // polyline
        ofNoFill();
        ofSetLineWidth(1);
        ofSetColor(0, 200, 200, 255);
        drawWithNormals(polyLineFromPoints);
    } // end of if(curveVertices.size() > 0){

    ofSetColor(0,0,0,80);
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
    ofDrawBitmapString("stroke weight [q++|w--]: " + ofToString(strokeWeight), 20,260);
    ofDrawBitmapString("[b] show/hide bounding box: " + ofToString(bshowBoundingBox), 20,280);
    ofDrawBitmapString("drag bounding box to move all the bezier", 20,300);
    ofDrawBitmapString("COLORS:\nfill " + ofToString((float)colorFill.r) + "(r) " + ofToString((float)colorFill.g) + "(g) " + ofToString((float)colorFill.b) + "(b)" +
                        "\nstroke " + ofToString((float)colorStroke.r) + "(r) " + ofToString((float)colorStroke.g) + "(g) " + ofToString((float)colorStroke.b) + "(b)", 20,320);
    ofDrawBitmapString("[p] create polyline", 20,360);

    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void ofxBezierEditor::mouseMoved(ofMouseEventArgs &args){
    if(beditBezier == true){
        for (int i = 0; i < curveVertices.size(); i++){
            float diffx = args.x - curveVertices.at(i).x;
            float diffy = args.y - curveVertices.at(i).y;
            float dist = sqrt(diffx*diffx + diffy*diffy);
            if (dist < radiusVertex){
                curveVertices.at(i).bOver = true;
            } else {
                curveVertices.at(i).bOver = false;
            }
        }

        for (int i = 0; i < controlPoint1.size(); i++){
            float diffx = args.x - controlPoint1.at(i).x;
            float diffy = args.y - controlPoint1.at(i).y;
            float dist = sqrt(diffx*diffx + diffy*diffy);
            if (dist < radiusControlPoints){
                controlPoint1.at(i).bOver = true;
            } else {
                controlPoint1.at(i).bOver = false;
            }
        }

        for (int i = 0; i < controlPoint2.size(); i++){
            float diffx = args.x - controlPoint2.at(i).x;
            float diffy = args.y - controlPoint2.at(i).y;
            float dist = sqrt(diffx*diffx + diffy*diffy);
            if (dist < radiusControlPoints){
                controlPoint2.at(i).bOver = true;
            } else {
                controlPoint2.at(i).bOver = false;
            }
        }
    }
}

//--------------------------------------------------------------
void ofxBezierEditor::mouseDragged(ofMouseEventArgs &args){
    if(beditBezier){
        if(bshowBoundingBox){
            translateX = args.x - mouseX;
            translateY = args.y - mouseY;
        }

        for (int i = 0; i < curveVertices.size(); i++){
            if (curveVertices.at(i).bBeingDragged == true){
                curveVertices.at(i).x = args.x - translateX;
                curveVertices.at(i).y = args.y - translateY;
            }
        }

        for (int i = 0; i < controlPoint1.size(); i++){
            if (controlPoint1.at(i).bBeingDragged == true){
                controlPoint1.at(i).x = args.x - translateX;
                controlPoint1.at(i).y = args.y - translateY;
            }
        }

        for (int i = 0; i < controlPoint2.size(); i++){
            if (controlPoint2.at(i).bBeingDragged == true){
                controlPoint2.at(i).x = args.x - translateX;
                controlPoint2.at(i).y = args.y - translateY;
            }
        }
    }
}

//--------------------------------------------------------------
void ofxBezierEditor::mousePressed(ofMouseEventArgs &args){
    if(beditBezier){
        if(args.button == OF_MOUSE_BUTTON_LEFT){
            if(bshowBoundingBox){
                mouseX = args.x - translateX;
                mouseY = args.y - translateY;
            }
            else{
                bool bAnyVertexDragged = false;
                // MOVE vertex
                for (int i = 0; i < curveVertices.size(); i++){
                    float diffx = args.x - translateX - curveVertices.at(i).x;
                    float diffy = args.y - translateY - curveVertices.at(i).y;
                    float dist = sqrt(diffx*diffx + diffy*diffy);
                    if (dist < radiusVertex){
                        curveVertices.at(i).bBeingDragged = true;
                        bAnyVertexDragged = true;
                    } else {
                        curveVertices.at(i).bBeingDragged = false;
                    }
                }

                for (int i = 0; i < controlPoint1.size(); i++){
                    float diffx = args.x - translateX - controlPoint1.at(i).x;
                    float diffy = args.y - translateY - controlPoint1.at(i).y;
                    float dist = sqrt(diffx*diffx + diffy*diffy);
                    if (dist < radiusControlPoints){
                        controlPoint1.at(i).bBeingDragged = true;
                        bAnyVertexDragged = true;
                    } else {
                        controlPoint1.at(i).bBeingDragged = false;
                    }
                }

                for (int i = 0; i < controlPoint2.size(); i++){
                    float diffx = args.x - translateX - controlPoint2.at(i).x;
                    float diffy = args.y - translateY - controlPoint2.at(i).y;
                    float dist = sqrt(diffx*diffx + diffy*diffy);
                    if (dist < radiusControlPoints){
                        controlPoint2.at(i).bBeingDragged = true;
                        bAnyVertexDragged = true;
                    } else {
                        controlPoint2.at(i).bBeingDragged = false;
                    }
                }

                // ADD vertex to the end
                if(bAnyVertexDragged == false){
                    draggableVertex vtx;
                    vtx.x = args.x - translateX;
                    vtx.y = args.y - translateY;
                    vtx.bOver 			= true;
                    vtx.bBeingDragged 	= false;
                    vtx.bBeingSelected  = false;
                    curveVertices.push_back(vtx);

                    draggableVertex cp;
                    int nEnd = curveVertices.size()-1;
                    cp.x = ofLerp(curveVertices.at(0).x - translateX, curveVertices.at(nEnd).x - translateX, 0.66);
                    cp.y = ofLerp(curveVertices.at(0).y - translateY, curveVertices.at(nEnd).y - translateY, 0.66);
                    cp.bOver 			= false;
                    cp.bBeingDragged 	= false;
                    cp.bBeingSelected 	= false;
                    controlPoint1.push_back(cp);
                    cp.x = ofLerp(curveVertices.at(0).x - translateX, curveVertices.at(nEnd).x - translateX, 0.33);
                    cp.y = ofLerp(curveVertices.at(0).y - translateY, curveVertices.at(nEnd).y - translateY, 0.33);
                    controlPoint2.push_back(cp);

                    updateBoundingBox();
                    calculateCenter();
                }
            } // end else bshowBoundingBox = false
        }
        if(args.button == OF_MOUSE_BUTTON_RIGHT){
            // SELECT several vertex
            bool bAnyVertexSelected = false;
            for (int i = 0; i < curveVertices.size(); i++){
                float diffx = args.x - translateX - curveVertices.at(i).x;
                float diffy = args.y - translateY - curveVertices.at(i).y;
                float dist = sqrt(diffx*diffx + diffy*diffy);
                if (dist < radiusVertex){
                    curveVertices.at(i).bBeingSelected = !curveVertices.at(i).bBeingSelected;
                    bAnyVertexSelected = true;
                }
            }

            if(bAnyVertexSelected == false){
                int numVertexSelected = 0;
                lastVertexSelected = 0;
                for (int i = 0; i < curveVertices.size(); i++){
                    if(curveVertices.at(i).bBeingSelected == true){
                        numVertexSelected++;
                        lastVertexSelected = i;
                    }
                }
                // ADD vertex between two points
                if(numVertexSelected >= 2){
                    draggableVertex vtx;
                    vtx.x = args.x - translateX;
                    vtx.y = args.y - translateY;
                    vtx.bOver 			= true;
                    vtx.bBeingDragged 	= false;
                    vtx.bBeingSelected 	= false;
                    curveVertices.insert(curveVertices.begin()+lastVertexSelected, vtx);

                    draggableVertex cp;
                    cp.x = ofLerp(curveVertices.at(lastVertexSelected-1).x - translateX, curveVertices.at(lastVertexSelected).x - translateX, 0.66);
                    cp.y = ofLerp(curveVertices.at(lastVertexSelected-1).y - translateY, curveVertices.at(lastVertexSelected).y - translateY, 0.66);
                    cp.bOver 			= false;
                    cp.bBeingDragged 	= false;
                    cp.bBeingSelected 	= false;
                    controlPoint1.insert(controlPoint1.begin()+lastVertexSelected,cp);
                    cp.x = ofLerp(curveVertices.at(lastVertexSelected).x - translateX, curveVertices.at(lastVertexSelected).x - translateX, 0.33);
                    cp.y = ofLerp(curveVertices.at(lastVertexSelected-1).y - translateY, curveVertices.at(lastVertexSelected).y - translateY, 0.33);
                    controlPoint2.insert(controlPoint2.begin()+lastVertexSelected,cp);

                    updateBoundingBox();
                    calculateCenter();
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofxBezierEditor::mouseReleased(ofMouseEventArgs &args){
	if(beditBezier){
        for (int i = 0; i < curveVertices.size(); i++){
            curveVertices.at(i).bBeingDragged = false;
        }
        for (int i = 0; i < controlPoint1.size(); i++){
            controlPoint1.at(i).bBeingDragged = false;
        }
        for (int i = 0; i < controlPoint2.size(); i++){
            controlPoint2.at(i).bBeingDragged = false;
        }
	}
}

//--------------------------------------------------------------
void ofxBezierEditor::keyPressed(ofKeyEventArgs &args){
    if(args.key == 'e'){
        beditBezier = !beditBezier;
    }
    if(beditBezier){
        if(args.key == 's'){
            saveXmlPoints();
        }
        else if(args.key == 'l'){
            loadXmlPoints();
        }
        else if(args.key == 'p'){
            createPolyLineFromPoints();
        }
        else if(args.key == 'f'){
            bfillBezier = !bfillBezier;
        }
        else if(args.key == 'b'){
            bshowBoundingBox = !bshowBoundingBox;
        }
        else if(args.key == 'q'){
            strokeWeight++;
        }
        else if(args.key == 'w'){
            strokeWeight--;
        }
        else if(args.key == 'n'){
            currentPointToMove++;
            if(currentPointToMove > curveVertices.size() + controlPoint1.size() + controlPoint2.size() -1){
                currentPointToMove = 0;
            }
        }
        else if(args.key == 'm'){
           currentPointToMove--;
           if(currentPointToMove < 0){
                currentPointToMove = curveVertices.size() + controlPoint1.size() + controlPoint2.size() -1;
            }
        }
        else if(args.key == OF_KEY_UP){
            int range = currentPointToMove / curveVertices.size();
            int mod = currentPointToMove % curveVertices.size();
            if(range == 0){
                curveVertices.at(mod).y--;
            }
            else if(range == 1){
                controlPoint1.at(mod).y--;
            }
            else if(range == 2){
                controlPoint2.at(mod).y--;
            }
        }
        else if(args.key == OF_KEY_DOWN){
            int range = currentPointToMove / curveVertices.size();
            int mod = currentPointToMove % curveVertices.size();
            if(range == 0){
                curveVertices.at(mod).y++;
            }
            else if(range == 1){
                controlPoint1.at(mod).y++;
            }
            else if(range == 2){
                controlPoint2.at(mod).y++;
            }
        }
        else if(args.key == OF_KEY_LEFT){
            int range = currentPointToMove / curveVertices.size();
            int mod = currentPointToMove % curveVertices.size();
            if(range == 0){
                curveVertices.at(mod).x--;
            }
            else if(range == 1){
                controlPoint1.at(mod).x--;
            }
            else if(range == 2){
                controlPoint2.at(mod).x--;
            }
        }
        else if(args.key == OF_KEY_RIGHT){
            int range = currentPointToMove / curveVertices.size();
            int mod = currentPointToMove % curveVertices.size();
            if(range == 0){
                curveVertices.at(mod).x++;
            }
            else if(range == 1){
                controlPoint1.at(mod).x++;
            }
            else if(range == 2){
                controlPoint2.at(mod).x++;
            }
        }
        if(args.key == OF_KEY_BACKSPACE){
            // REMOVE last vertex
            curveVertices.pop_back();
            controlPoint1.pop_back();
            controlPoint2.pop_back();

            updateBoundingBox();
            calculateCenter();
        }
        if(args.key == OF_KEY_DEL){
            // REMOVE last intermediate vertex added
            curveVertices.erase(curveVertices.begin()+lastVertexSelected);
            controlPoint1.erase(controlPoint1.begin()+lastVertexSelected);
            controlPoint2.erase(controlPoint2.begin()+lastVertexSelected);

            updateBoundingBox();
            calculateCenter();
        }
    }
}

//--------------------------------------------------------------
void ofxBezierEditor::keyReleased(ofKeyEventArgs &args){

}

//--------------------------------------------------------------
void ofxBezierEditor::updateBoundingBox(){
    float minx, maxx, miny, maxy;
    minx = miny = 100000;
    maxx = maxy = -10000;

    for (int i = 0; i < curveVertices.size(); i++){
        if(curveVertices.at(i).x > maxx){
            maxx = curveVertices.at(i).x;
        }
        if(curveVertices.at(i).x < minx){
            minx = curveVertices.at(i).x;
        }
        if(curveVertices.at(i).y > maxy){
            maxy = curveVertices.at(i).y;
        }
        if(curveVertices.at(i).y < miny){
            miny = curveVertices.at(i).y;
        }
    }
    boundingBox.set(minx, miny, maxx-minx, maxy-miny);
}

//--------------------------------------------------------------
void ofxBezierEditor::calculateCenter(){
    center.set(boundingBox.x + 0.5f*boundingBox.width, boundingBox.y + 0.5f*boundingBox.height);
}

//--------------------------------------------------------------
void ofxBezierEditor::createPolyLineFromPoints(){
    if(curveVertices.size() > 0){
        for (int i = 0; i < curveVertices.size(); i++){
           if (i == 0){
                polyLineFromPoints.addVertex(curveVertices.at(0).x, curveVertices.at(0).y); // we need to duplicate 0 for the curve to start at point 0
           }
           else{
                polyLineFromPoints.bezierTo(controlPoint1.at(i).x, controlPoint1.at(i).y, controlPoint2.at(i).x, controlPoint2.at(i).y, curveVertices.at(i).x, curveVertices.at(i).y);
            }
        }
        polyLineFromPoints.bezierTo(controlPoint1.at(0).x, controlPoint1.at(0).y, controlPoint2.at(0).x, controlPoint2.at(0).y, curveVertices.at(0).x, curveVertices.at(0).y);
        polyLineFromPoints.setClosed(true);
    }
}
