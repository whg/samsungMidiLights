//
//  LaunchPad.h
//  samsungMidiLights
//
//  Created by Will Gallia on 24/03/2015.
//
//

#pragma once

#include "ofMain.h"
#include "ofxLaunchpad.h"

class LaunchPad : public ofxLaunchpad {
    
public:
    LaunchPad() {
        ofxLaunchpad::setup(1);
    }
    
    void draw(ofEventArgs & args) {
        ofxLaunchpad::draw(0, 0);
    }
    
    
};
