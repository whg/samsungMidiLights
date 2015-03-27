#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxOBJModel.h"
#include "ofxGui.h"
#include "ofxDmx.h"

#include "LaunchPad.h"

typedef ofParameter<int> padParam;

struct Pad {
    string name;
    bool on;
    float value;
    ofVec2f pos;
    ofParameter<int> midiPitch;
    
    ofxPanel colourPanel;
    ofParameterGroup paramGroup;
    ofParameter<int> red;
    ofParameter<int> green;
    ofParameter<int> blue;



    
    Pad(): name(""), on(false) {}
    
    Pad(string s, int p, int x, int y):
        name(s),
        on(false),
        pos(x, y),
        midiPitch(s, p, 1, 100),
        value(0) {

            paramGroup.setName(s);
            paramGroup.add(red.set("red", 0, 0, 255));
            paramGroup.add(green.set("blue", 0, 0, 255));
            paramGroup.add(blue.set("green", 0, 0, 255));
            colourPanel.setup(paramGroup);
            
    }
};

struct Sharpy : public Pad {
    float value2;
    Sharpy(string s, int p, int x, int y): Pad(s, p, x, y){
        value = 0;
        value2 = 0;
    }
};

class samsungMidiLights : public ofBaseApp, public ofxMidiListener {
	
    bool showGui;
    
    ofxDmx dmx;
    
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
    
    vector<Sharpy> sharpys;

    ofxOBJModel model;

    LaunchPad lp1, lp2;
    
    ofParameterGroup parameters;
    map<string, padParam> midiPitches;
    ofParameter<ofColor> color;
    ofxPanel gui;
    
    
    vector<ofPixels*> pixelPerms;

};


