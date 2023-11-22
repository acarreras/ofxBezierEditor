//
//  ofxBezierEditorSettings.hpp
//  example-ui
//
//  Created by Fred Rodrigues on 22/11/2023.
//

#pragma once
#include "ofMain.h"

class ofxBezierEditorSettings {
public:
    ofxBezierEditorSettings();
    ~ofxBezierEditorSettings();

    bool getUseRibbonMesh() { return bUseRibbonMesh; };
    void setUseRibbonMesh(bool b) { bUseRibbonMesh = b; };
    void setMeshLengthPrecisionMultiplier(int m) { meshLengthPrecisionMultiplier = m; updateAllFromVertices();};
    int getMeshLengthPrecisionMultiplier() { return meshLengthPrecisionMultiplier; };
    float getRibbonWidth() { return ribbonWidth; };
    void setRibbonWidth(float w) { ribbonWidth = w; updateAllFromVertices();};
    

    void setTubeResolution(int r) { tubeResolution = r; updateAllFromVertices();};
    int getTubeResolution() { return tubeResolution; };
    void setTubeRadius(float r) { tubeRadius = r; updateAllFromVertices();};
    float getTubeRadius() { return tubeRadius; };
    
    bool getHasRoundCaps() { return roundCap; };
    void setHasRoundCaps(bool b) { roundCap = b; updateAllFromVertices();};
    
    bool getFillBezier() { return bfillBezier; };
    void setFillBezier(bool b) { bfillBezier = b; updateAllFromVertices();};
    void setClosed(bool b) { bIsClosed = b; updateAllFromVertices(); };
    bool getClosed() { return bIsClosed; };

private:
    // File Operations
    string jsonFileName;
    void loadPoints(string filename);
    void savePoints(string filename);
    
    int radiusControlPoints;
    int radiusVertex;
    
    // Colors
    ofColor vertexColour;           // Bright yellow for default vertices
    ofColor vertexHoverColor;       // Semi-transparent yellow when hovering
    ofColor vertexLabelColour;      // Slightly less intense yellow for labels
    ofColor vertexSelectedColour;   // Bright green for selected vertices
    ofColor vertexDraggedColour;    // Slightly different green for dragged vertices
    
    ofColor ctrPtColour;            // Bright purple for default control points
    ofColor ctrPtHoverColor;        // Semi-transparent purple when hovering
    ofColor ctrPtLabelColour;       // Slightly less intense purple for labels
    ofColor ctrPtSelectedColour;    // Bright blue for selected control points
    ofColor ctrPtDraggedColour;     // Slightly different blue for dragged control points
    ofColor colorFill;
    ofColor colorStroke;
    
    ofJson JSONBezier;

    bool beditBezier;
    bool bIsClosed;
    bool bUseRibbonMesh = false;
    bool bUseTubeMesh = false;
    bool bshowBoundingBox;
    int meshLengthPrecisionMultiplier = 1;
    float ribbonWidth = 10.0;
    int tubeResolution = 10;
    float tubeRadius = 10.0;
    bool roundCap = true;

    
};

