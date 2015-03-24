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
    ofVec2f pos;
    ofParameter<int> midiPitch;
    
    Pad(): name(""), on(false) {}
    
    Pad(string s, int p, int x, int y):
        name(s),
        on(false),
        pos(x, y),
        midiPitch(s, p, 1, 100) {

    }
};


class samsungMidiLights : public ofBaseApp, public ofxMidiListener {
	
    bool showGui;
    
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
	
    
    void drawMPC(ofEventArgs &args);
	
	ofxMidiIn midiIn;
	ofxMidiMessage midiMessage;
    
    
    ofEasyCam cam;
    map<int, Pad> padKeys;
    vector<Pad> pads;

    ofxOBJModel model;

    LaunchPad lp1, lp2;
    
    ofParameterGroup parameters;
    map<string, padParam> midiPitches;
    ofParameter<ofColor> color;
    ofxPanel gui;

};


