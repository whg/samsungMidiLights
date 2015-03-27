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

static int portCounter = 1;

class LaunchPad : public ofxLaunchpad {
    
public:
    LaunchPad() {
        ofxLaunchpad::setup(portCounter++);
    }
    
    void draw(ofEventArgs & args) {
        ofxLaunchpad::draw(0, 0);
    }
    
    
};

//class LaunchPadAnimation {
//protected:
//public:
//    float counter;
//
//    virtual void draw() {}
//    void reset() {
//        counter = 0;
//    }
//};
//
//class LPCircularAnimation : public LaunchPadAnimation {
//public:
//    void draw() {
//        ofClear(0, 255);
//        float r = ofMap(sin(counter), -1, 1, 0, 6);
//        ofFill();
//        ofSetColor(ofColor::green);
//        ofDrawCircle(4, 4, r);
//        ofNoFill();
//        ofSetColor(ofColor::red);
//        ofDrawCircle(4, 4, r);
//        counter+= 0.1;
//    }
//};