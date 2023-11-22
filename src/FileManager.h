//
//  FileManager.hpp
//  example-ui
//
//  Created by Fred Rodrigues on 21/11/2023.
//

#pragma once
#include "ofMain.h"
#include "BezierCurve.h"

class FileManager {
public:
    FileManager();
    ~FileManager();

    void saveCurve(const BezierCurve& curve, const string& filename);
    void loadCurve(BezierCurve& curve, const string& filename);
};
