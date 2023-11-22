//
//  ofxBezierEditorSettings.hpp
//  example-ui
//
//  Created by Fred Rodrigues on 22/11/2023.
//

#pragma once
#include "ofMain.h"
#include "draggableVertex.h"
#include "ofxBezierEvents.h"

class ofxBezierEditorSettings {
public:
    ofxBezierEditorSettings(std::vector<draggableVertex>& curveVertices,
                            std::vector<draggableVertex>& controlPoint1,
                            std::vector<draggableVertex>& controlPoint2);
    ~ofxBezierEditorSettings();

    bool getUseRibbonMesh() { return bUseRibbonMesh; };
    void setUseRibbonMesh(bool b) { bUseRibbonMesh = b; };
    void setMeshLengthPrecisionMultiplier(int m) { meshLengthPrecisionMultiplier = m; };
    int getMeshLengthPrecisionMultiplier() { return meshLengthPrecisionMultiplier; };
    float getRibbonWidth() { return ribbonWidth; };
    void setRibbonWidth(float w) { ribbonWidth; };
    

    void setTubeResolution(int r) { tubeResolution = r; };
    int getTubeResolution() { return tubeResolution; };
    void setTubeRadius(float r) { tubeRadius = r; };
    float getTubeRadius() { return tubeRadius; };
    
    bool getHasRoundCaps() { return roundCap; };
    void setHasRoundCaps(bool b) { roundCap = b; };
    
    bool getFillBezier() { return bfillBezier; };
    void setFillBezier(bool b) { bfillBezier = b; };
    void setClosed(bool b) { bIsClosed = b; };
    bool getClosed() { return bIsClosed; };
    
    int getRadiusControlPoints() { return radiusControlPoints; };
    void setRadiusControlPoints(int r) { radiusControlPoints = r; };
    int getRadiusVertex() { return radiusVertex; };
    void setRadiusVertex(int r) { radiusVertex = r; };
    
    
    void setColorFill(ofColor c);
//    void setColorFill(float r, float g, float b, float a = 255.0);
//    void setColorFillR(float c);
//    void setColorFillG(float c);
//    void setColorFillB(float c);
//    void setColorFillA(float a);
//    
    void setColorStroke(ofColor c);
//    void setColorStroke(float r, float g, float b, float a = 255.0);
//    void setColorStrokeR(float c);
//    void setColorStrokeG(float c);
//    void setColorStrokeB(float c);
//    void setColorStrokeA(float a);
    
    ofColor getColorFill() { return colorFill; };
    ofColor getColorStroke() { return colorStroke; };



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
    ofRectangle boundingBox;

    bool beditBezier = false;
    bool bIsClosed = false;
    bool bfillBezier = false;
    bool bUseRibbonMesh = false;
    bool bUseTubeMesh = false;
    bool bshowBoundingBox = false;
    int meshLengthPrecisionMultiplier = 1;
    float ribbonWidth = 10.0;
    int tubeResolution = 10;
    float tubeRadius = 10.0;
    bool roundCap = true;
    int currentPointToMove = 0;
    int lastVertexSelected = 0;
    
    std::vector<draggableVertex>& curveVertices;
    std::vector<draggableVertex>& controlPoint1;
    std::vector<draggableVertex>& controlPoint2;
    
private:
    
    void triggerUpdate();

};

