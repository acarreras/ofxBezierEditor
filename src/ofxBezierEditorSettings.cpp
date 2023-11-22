//
//  ofxBezierEditorSettings.cpp
//  example-ui
//
//  Created by Fred Rodrigues on 22/11/2023.
//

#include "ofxBezierEditorSettings.h"

ofxBezierEditorSettings::ofxBezierEditorSettings(std::vector<draggableVertex>& curveVertices,
                                                 std::vector<draggableVertex>& controlPoint1,
                                                 std::vector<draggableVertex>& controlPoint2)
: curveVertices(curveVertices), controlPoint1(controlPoint1), controlPoint2(controlPoint2) {
    
    
}

ofxBezierEditorSettings::~ofxBezierEditorSettings() {
    // Cleanup
}

//--------------------------------------------------------------
void ofxBezierEditorSettings::loadPoints(string filename){
    ofLogVerbose("ofxBezierEditor") << "loadPoints: " << filename;
    jsonFileName = filename;
    
    ofFile jsonFile(jsonFileName);
    if (jsonFile.exists()) {
        jsonFile >> JSONBezier;
        // Read data from JSON
        bUseRibbonMesh = JSONBezier["bezier"]["useRibbonMesh"].get<bool>();
        ofLogVerbose("ofxBezierEditor") << "bUseRibbonMesh: " << bUseRibbonMesh;
        ribbonWidth = JSONBezier["bezier"]["ribbonWidth"].get<float>();
        ofLogVerbose("ofxBezierEditor") << "ribbonWidth: " << ribbonWidth;
        meshLengthPrecisionMultiplier = JSONBezier["bezier"]["meshLengthPrecision"].get<int>();
        ofLogVerbose("ofxBezierEditor") << "meshLengthPrecisionMultiplier: " << meshLengthPrecisionMultiplier;
        
        bUseTubeMesh = JSONBezier["bezier"]["useTubeMesh"].get<bool>();
        ofLogVerbose("ofxBezierEditor") << "bUseTubeMesh: " << bUseTubeMesh;
        tubeRadius = JSONBezier["bezier"]["tubeRadius"].get<float>();
        ofLogVerbose("ofxBezierEditor") << "tubeRadius: " << tubeRadius;
        tubeResolution = JSONBezier["bezier"]["tubeResolution"].get<int>();
        ofLogVerbose("ofxBezierEditor") << "tubeResolution: " << tubeResolution;
        
        
        bIsClosed = JSONBezier["bezier"]["closed"].get<bool>();
        ofLogVerbose("ofxBezierEditor") << "bIsClosed: " << bIsClosed;
        
        //polyLineFromPoints.setClosed(bIsClosed);
        
        
        bfillBezier = JSONBezier["bezier"]["fill"].get<bool>();
        colorFill.set(
                      JSONBezier["bezier"]["colorFill"]["r"].get<int>(),
                      JSONBezier["bezier"]["colorFill"]["g"].get<int>(),
                      JSONBezier["bezier"]["colorFill"]["b"].get<int>(),
                      JSONBezier["bezier"]["colorFill"]["a"].get<int>()
                      );
        
        colorStroke.set(
                        JSONBezier["bezier"]["colorStroke"]["r"].get<int>(),
                        JSONBezier["bezier"]["colorStroke"]["g"].get<int>(),
                        JSONBezier["bezier"]["colorStroke"]["b"].get<int>(),
                        JSONBezier["bezier"]["colorStroke"]["a"].get<int>()
                        );
        
        
        curveVertices.clear();
        for (const auto& vertexJson : JSONBezier["bezier"]["vertices"]) {
            draggableVertex vtx;
            vtx.pos.x = vertexJson["x"].get<float>();
            vtx.pos.y = vertexJson["y"].get<float>();
            vtx.bOver = false;
            vtx.bBeingDragged = false;
            vtx.bBeingSelected = false;
            curveVertices.push_back(vtx);
        }
        
        // Read control points from JSON
        controlPoint1.clear();
        for (const auto& cpJson : JSONBezier["bezier"]["cp1"]) {
            draggableVertex cp;
            cp.pos.x = cpJson["x"].get<float>();
            cp.pos.y = cpJson["y"].get<float>();
            cp.bOver = false;
            cp.bBeingDragged = false;
            controlPoint1.push_back(cp);
        }
        
        controlPoint2.clear();
        for (const auto& cpJson : JSONBezier["bezier"]["cp2"]) {
            draggableVertex cp;
            cp.pos.x = cpJson["x"].get<float>();
            cp.pos.y = cpJson["y"].get<float>();
            cp.bOver = false;
            cp.bBeingDragged = false;
            controlPoint2.push_back(cp);
        }
        
        
        
    } else {
        ofLogVerbose() << "ofxBezierEditor::loadPoints(): File does not exist.";
    }
    
    triggerUpdate();
}

//--------------------------------------------------------------
void ofxBezierEditorSettings::savePoints(string filename){
    JSONBezier.clear();
    
    // Create the JSON structure
    JSONBezier["bezier"]["fill"] = bfillBezier;
    JSONBezier["bezier"]["closed"] = bIsClosed;
    
    JSONBezier["bezier"]["useRibbonMesh"] = bUseRibbonMesh;
    JSONBezier["bezier"]["meshLengthPrecision"] =  meshLengthPrecisionMultiplier;
    JSONBezier["bezier"]["ribbonWidth"] = ribbonWidth;
    
    JSONBezier["bezier"]["useTubeMesh"] = bUseTubeMesh;
    JSONBezier["bezier"]["tubeRadius"] = tubeRadius;
    JSONBezier["bezier"]["tubeResolution"] = tubeResolution;
    
    JSONBezier["bezier"]["colorFill"]["r"] = colorFill.r;
    JSONBezier["bezier"]["colorFill"]["g"] = colorFill.g;
    JSONBezier["bezier"]["colorFill"]["b"] = colorFill.b;
    JSONBezier["bezier"]["colorFill"]["a"] = colorFill.a;
    
    JSONBezier["bezier"]["colorStroke"]["r"] = colorStroke.r;
    JSONBezier["bezier"]["colorStroke"]["g"] = colorStroke.g;
    JSONBezier["bezier"]["colorStroke"]["b"] = colorStroke.b;
    JSONBezier["bezier"]["colorStroke"]["a"] = colorStroke.a;
    
    for (int i = 0; i < curveVertices.size(); i++) {
        JSONBezier["bezier"]["vertices"][i]["x"] = curveVertices.at(i).pos.x ;
        JSONBezier["bezier"]["vertices"][i]["y"] = curveVertices.at(i).pos.y ;
    }
    
    for (int i = 0; i < controlPoint1.size(); i++) {
        JSONBezier["bezier"]["cp1"][i]["x"] = controlPoint1.at(i).pos.x ;
        JSONBezier["bezier"]["cp1"][i]["y"] = controlPoint1.at(i).pos.y ;
    }
    
    for (int i = 0; i < controlPoint2.size(); i++) {
        JSONBezier["bezier"]["cp2"][i]["x"] = controlPoint2.at(i).pos.x ;
        JSONBezier["bezier"]["cp2"][i]["y"] = controlPoint2.at(i).pos.y ;
    }
    
    // Save JSON to a file
    ofSavePrettyJson(filename, JSONBezier);
    
}
void ofxBezierEditorSettings::triggerUpdate(){
    TriggerUpdateEventArgs args;
    // Set any necessary properties of args
    ofNotifyEvent(triggerUpdateEvent, args, this);
}
