//
//  ofxBezierUI.cpp
//  example-ui
//
//  Created by Fred Rodrigues on 22/11/2023.
//

#include "ofxBezierUI.h"

ofxBezierUI::ofxBezierUI(ofxBezierEditorSettings & settings,
						 std::vector <draggableVertex> & curveVertices,
						 std::vector <draggableVertex> & controlPoint1,
						 std::vector <draggableVertex> & controlPoint2)
	: settings(settings), curveVertices(curveVertices), controlPoint1(controlPoint1), controlPoint2(controlPoint2){

	setReactToMouseAndKeyEvents(true);

}

ofxBezierUI::~ofxBezierUI(){
	setReactToMouseAndKeyEvents(false);
}

void ofxBezierUI::mouseMoved(ofMouseEventArgs & args){
	if(settings.beditBezier == true){
		for(int i = 0; i < curveVertices.size(); i++){
			float diffx = args.x - curveVertices.at(i).pos.x;
			float diffy = args.y - curveVertices.at(i).pos.y;
			float dist = sqrt(diffx * diffx + diffy * diffy);
			if(dist < settings.radiusVertex){
				curveVertices.at(i).bOver = true;
			}else{
				curveVertices.at(i).bOver = false;
			}
		}

		for(int i = 0; i < controlPoint1.size(); i++){
			float diffx = args.x - controlPoint1.at(i).pos.x;
			float diffy = args.y - controlPoint1.at(i).pos.y;
			float dist = sqrt(diffx * diffx + diffy * diffy);
			if(dist < settings.radiusControlPoints){
				controlPoint1.at(i).bOver = true;
			}else{
				controlPoint1.at(i).bOver = false;
			}
		}

		for(int i = 0; i < controlPoint2.size(); i++){
			float diffx = args.x - controlPoint2.at(i).pos.x;
			float diffy = args.y - controlPoint2.at(i).pos.y;
			float dist = sqrt(diffx * diffx + diffy * diffy);
			if(dist < settings.radiusControlPoints){
				controlPoint2.at(i).bOver = true;
			}else{
				controlPoint2.at(i).bOver = false;
			}
		}
	}
}

//--------------------------------------------------------------
void ofxBezierUI::mouseDragged(ofMouseEventArgs & args){
	if(settings.beditBezier){
		if(settings.bshowBoundingBox){
			int deltaX = args.x - mouseX;
			int deltaY = args.y - mouseY;

			for(int i = 0; i < curveVertices.size(); i++){
				curveVertices.at(i).pos.x += deltaX;
				curveVertices.at(i).pos.y += deltaY;
			}

			for(int i = 0; i < controlPoint1.size(); i++){
				controlPoint1.at(i).pos.x += deltaX;
				controlPoint1.at(i).pos.y += deltaY;
			}

			for(int i = 0; i < controlPoint2.size(); i++){
				controlPoint2.at(i).pos.x += deltaX;
				controlPoint2.at(i).pos.y += deltaY;
			}

			mouseX = args.x;
			mouseY = args.y;
		}

		for(int i = 0; i < curveVertices.size(); i++){
			if(curveVertices.at(i).bBeingDragged == true){
				curveVertices.at(i).pos.x = args.x;
				curveVertices.at(i).pos.y = args.y;
			}
		}

		for(int i = 0; i < controlPoint1.size(); i++){
			if(controlPoint1.at(i).bBeingDragged == true){
				controlPoint1.at(i).pos.x = args.x;
				controlPoint1.at(i).pos.y = args.y;
			}
		}

		for(int i = 0; i < controlPoint2.size(); i++){
			if(controlPoint2.at(i).bBeingDragged == true){
				controlPoint2.at(i).pos.x = args.x;
				controlPoint2.at(i).pos.y = args.y;
			}
		}
		triggerUpdate();
	}

}

//--------------------------------------------------------------
void ofxBezierUI::mousePressed(ofMouseEventArgs & args){
	if(settings.beditBezier){
		if(args.button == OF_MOUSE_BUTTON_LEFT){
			if(settings.bshowBoundingBox){
				mouseX = args.x;
				mouseY = args.y;
			}else{
				bool bAnyVertexDragged = false;
				// MOVE vertex
				for(int i = 0; i < curveVertices.size(); i++){
					float diffx = args.x  - curveVertices.at(i).pos.x;
					float diffy = args.y  - curveVertices.at(i).pos.y;
					float dist = sqrt(diffx * diffx + diffy * diffy);
					if(dist < settings.radiusVertex){
						curveVertices.at(i).bBeingDragged = true;
						bAnyVertexDragged = true;
					}else{
						curveVertices.at(i).bBeingDragged = false;
					}
				}

				for(int i = 0; i < controlPoint1.size(); i++){
					float diffx = args.x  - controlPoint1.at(i).pos.x;
					float diffy = args.y  - controlPoint1.at(i).pos.y;
					float dist = sqrt(diffx * diffx + diffy * diffy);
					if(dist < settings.radiusControlPoints){
						controlPoint1.at(i).bBeingDragged = true;
						bAnyVertexDragged = true;
					}else{
						controlPoint1.at(i).bBeingDragged = false;
					}
				}

				for(int i = 0; i < controlPoint2.size(); i++){
					float diffx = args.x  - controlPoint2.at(i).pos.x;
					float diffy = args.y  - controlPoint2.at(i).pos.y;
					float dist = sqrt(diffx * diffx + diffy * diffy);
					if(dist < settings.radiusControlPoints){
						controlPoint2.at(i).bBeingDragged = true;
						bAnyVertexDragged = true;
					}else{
						controlPoint2.at(i).bBeingDragged = false;
					}
				}

				// ADD vertex to the end
				if(bAnyVertexDragged == false){
					draggableVertex vtx;
					vtx.pos.x = args.x;
					vtx.pos.y = args.y;
					vtx.bOver = true;
					vtx.bBeingDragged = false;
					vtx.bBeingSelected = false;
					curveVertices.push_back(vtx);

					draggableVertex cp;
					int nEnd = curveVertices.size() - 1;
					cp.pos.x = ofLerp(curveVertices.at(0).pos.x, curveVertices.at(nEnd).pos.x, 0.66);
					cp.pos.y = ofLerp(curveVertices.at(0).pos.y, curveVertices.at(nEnd).pos.y, 0.66);
					cp.bOver = false;
					cp.bBeingDragged = false;
					cp.bBeingSelected = false;
					controlPoint1.push_back(cp);
					cp.pos.x = ofLerp(curveVertices.at(0).pos.x, curveVertices.at(nEnd).pos.x, 0.33);
					cp.pos.y = ofLerp(curveVertices.at(0).pos.y, curveVertices.at(nEnd).pos.y, 0.33);
					controlPoint2.push_back(cp);

					triggerUpdate();

				}
			}
		}
		if(args.button == OF_MOUSE_BUTTON_RIGHT){
			// SELECT several vertex
			bool bAnyVertexSelected = false;
			for(int i = 0; i < curveVertices.size(); i++){
				float diffx = args.x  - curveVertices.at(i).pos.x;
				float diffy = args.y  - curveVertices.at(i).pos.y;
				float dist = sqrt(diffx * diffx + diffy * diffy);
				if(dist < settings.radiusVertex){
					curveVertices.at(i).bBeingSelected = !curveVertices.at(i).bBeingSelected;
					bAnyVertexSelected = true;
				}
			}

			if(bAnyVertexSelected == false){
				int numVertexSelected = 0;
				lastVertexSelected = 0;
				for(int i = 0; i < curveVertices.size(); i++){
					if(curveVertices.at(i).bBeingSelected == true){
						numVertexSelected++;
						lastVertexSelected = i;
					}
				}
				// ADD vertex between two points
				if(numVertexSelected >= 2){
					draggableVertex vtx;
					vtx.pos.x = args.x;
					vtx.pos.y = args.y;
					vtx.bOver = true;
					vtx.bBeingDragged = false;
					vtx.bBeingSelected = false;
					curveVertices.insert(curveVertices.begin() + lastVertexSelected, vtx);

					draggableVertex cp;
					cp.pos.x = ofLerp(curveVertices.at(lastVertexSelected - 1).pos.x, curveVertices.at(lastVertexSelected).pos.x, 0.66);
					cp.pos.y = ofLerp(curveVertices.at(lastVertexSelected - 1).pos.y, curveVertices.at(lastVertexSelected).pos.y, 0.66);
					cp.bOver = false;
					cp.bBeingDragged = false;
					cp.bBeingSelected = false;
					controlPoint1.insert(controlPoint1.begin() + lastVertexSelected, cp);
					cp.pos.x = ofLerp(curveVertices.at(lastVertexSelected).pos.x, curveVertices.at(lastVertexSelected).pos.x, 0.33);
					cp.pos.y = ofLerp(curveVertices.at(lastVertexSelected - 1).pos.y, curveVertices.at(lastVertexSelected).pos.y, 0.33);
					controlPoint2.insert(controlPoint2.begin() + lastVertexSelected, cp);

					triggerUpdate();
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofxBezierUI::mouseReleased(ofMouseEventArgs & args){
	if(settings.beditBezier){
		for(int i = 0; i < curveVertices.size(); i++){
			curveVertices.at(i).bBeingDragged = false;
		}
		for(int i = 0; i < controlPoint1.size(); i++){
			controlPoint1.at(i).bBeingDragged = false;
		}
		for(int i = 0; i < controlPoint2.size(); i++){
			controlPoint2.at(i).bBeingDragged = false;
		}

	}
}
//--------------------------------------------------------------
void ofxBezierUI::mouseScrolled(ofMouseEventArgs & args){
}
//--------------------------------------------------------------
void ofxBezierUI::mouseEntered(ofMouseEventArgs & args){
}
//--------------------------------------------------------------
void ofxBezierUI::mouseExited(ofMouseEventArgs & args){
}
//--------------------------------------------------------------
void ofxBezierUI::keyPressed(ofKeyEventArgs & args){
	if(args.key == 'e'){
		settings.beditBezier = !settings.beditBezier;
	}

	if(settings.beditBezier){
		if(args.key == 's'){
			settings.savePoints(settings.jsonFileName);
		}else if(args.key == 'l'){
			settings.loadPoints(settings.jsonFileName);
			triggerUpdate();
		}else if(args.key == 'f'){
			settings.bfillBezier = !settings.bfillBezier;
			triggerUpdate();
		}else if(args.key == 'b'){
			settings.bshowBoundingBox = !settings.bshowBoundingBox;
			triggerUpdate();
		}else if(args.key == 'c'){
			settings.bIsClosed = !settings.bIsClosed;
			triggerUpdate();
		}else if(args.key == 'n'){
			currentPointToMove++;
			if(currentPointToMove > curveVertices.size() + controlPoint1.size() + controlPoint2.size() - 1){
				currentPointToMove = 0;
			}
		}else if(args.key == 'm'){
			currentPointToMove--;
			if(currentPointToMove < 0){
				currentPointToMove = curveVertices.size() + controlPoint1.size() + controlPoint2.size() - 1;
			}
		}else if(args.key == OF_KEY_UP){
			int range = currentPointToMove / curveVertices.size();
			int mod = currentPointToMove % curveVertices.size();
			if(range == 0){
				curveVertices.at(mod).pos.y--;
			}else if(range == 1){
				controlPoint1.at(mod).pos.y--;
			}else if(range == 2){
				controlPoint2.at(mod).pos.y--;
			}
		}else if(args.key == OF_KEY_DOWN){
			int range = currentPointToMove / curveVertices.size();
			int mod = currentPointToMove % curveVertices.size();
			if(range == 0){
				curveVertices.at(mod).pos.y++;
			}else if(range == 1){
				controlPoint1.at(mod).pos.y++;
			}else if(range == 2){
				controlPoint2.at(mod).pos.y++;
			}
		}else if(args.key == OF_KEY_LEFT){
			int range = currentPointToMove / curveVertices.size();
			int mod = currentPointToMove % curveVertices.size();
			if(range == 0){
				curveVertices.at(mod).pos.x--;
			}else if(range == 1){
				controlPoint1.at(mod).pos.x--;
			}else if(range == 2){
				controlPoint2.at(mod).pos.x--;
			}
		}else if(args.key == OF_KEY_RIGHT){
			int range = currentPointToMove / curveVertices.size();
			int mod = currentPointToMove % curveVertices.size();
			if(range == 0){
				curveVertices.at(mod).pos.x++;
			}else if(range == 1){
				controlPoint1.at(mod).pos.x++;
			}else if(range == 2){
				controlPoint2.at(mod).pos.x++;
			}
		}
		if(args.key == OF_KEY_BACKSPACE){
			// REMOVE last vertex
			curveVertices.pop_back();
			controlPoint1.pop_back();
			controlPoint2.pop_back();

			triggerUpdate();
		}
		if(args.key == OF_KEY_DEL){
			// REMOVE last intermediate vertex added
			curveVertices.erase(curveVertices.begin() + lastVertexSelected);
			controlPoint1.erase(controlPoint1.begin() + lastVertexSelected);
			controlPoint2.erase(controlPoint2.begin() + lastVertexSelected);

			triggerUpdate();
		}
	}
}

void ofxBezierUI::keyReleased(ofKeyEventArgs & args){
}


//--------------------------------------------------------------
void ofxBezierUI::setReactToMouseAndKeyEvents(bool b){
	if(b == true){
		ofRegisterMouseEvents(this);
		ofRegisterKeyEvents(this);
	}else{
		ofUnregisterMouseEvents(this);
		ofUnregisterKeyEvents(this);
	}
}

void ofxBezierUI::registerToEvents(){
	ofRegisterMouseEvents(this);
	ofRegisterKeyEvents(this);
}

void ofxBezierUI::unregisterFromEvents(){
	ofUnregisterMouseEvents(this);
	ofUnregisterKeyEvents(this);
}



void ofxBezierUI::triggerUpdate(){
	TriggerUpdateEventArgs args;
	// Set any necessary properties of args
	ofNotifyEvent(triggerUpdateEvent, args, this);
}
