#pragma once

#include "ofMain.h"
#include "ofxBezierEditor.h"

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

		ofxBezierEditor myBezier;
		bool bdrawBezierInfo;
    
        ofEasyCam cam;
        ofLight light;
        int mode = POLYLINE_MODE;




};
