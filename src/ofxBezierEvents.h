//
//  ofxBezierEvents.h
//  example-ui
//
//  Created by Fred Rodrigues on 23/11/2023.
//

#pragma once

#include "ofMain.h"

class TriggerUpdateEventArgs : public ofEventArgs {
    // Define any arguments you want to pass with the event
    // For a simple trigger, you might not need to pass any data
};

extern ofEvent<TriggerUpdateEventArgs> triggerUpdateEvent;
