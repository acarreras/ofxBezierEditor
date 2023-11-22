#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    
    ofGLWindowSettings settings;
    settings.setSize(1024, 768);
    settings.windowMode = OF_WINDOW;
    settings.setGLVersion(4, 1);
    
    auto window = ofCreateWindow(settings);

    ofRunApp(window, std::make_shared<ofApp>());
    ofRunMainLoop();

}
