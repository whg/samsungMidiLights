#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxOBJModel.h"
#include "ofxGui.h"

#include "LaunchPad.h"

typedef ofParameter<int> padParam;

struct Pad {
    string name;
    bool on;
    Pad(): name(""), on(false) {}
    Pad(string s): name(s), on(false) {}
};


class samsungMidiLights : public ofBaseApp, public ofxMidiListener {
	
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	void keyReleased(int key);
	
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	
	void newMidiMessage(ofxMidiMessage& eventArgs);
	
	
	ofxMidiIn midiIn;
	ofxMidiMessage midiMessage;
    
    
    ofEasyCam cam;
    map<int, Pad> padKeys;

    ofxOBJModel model;

    LaunchPad lp1, lp2;
    
    ofParameterGroup parameters;
    map<string, padParam> midiPitches;
    ofParameter<ofColor> color;
    ofxPanel gui;

};


