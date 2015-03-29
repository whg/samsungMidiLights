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
    
    ofxPanel panel;
    ofParameterGroup paramGroup;
//    ofParameter<int> red;
//    ofParameter<int> green;
//    ofParameter<int> blue;
    ofParameter<ofColor> col;
    ofParameter<int> dmxChannel;

    
    Pad(): name(""), on(false) {}
    
    Pad(string s, int p, int x, int y):
        name(s),
        on(false),
        pos(x, y),
        value(0) {

            paramGroup.setName(s);
//            paramGroup.add(red.set("red", 0, 0, 255));
//            paramGroup.add(green.set("blue", 0, 0, 255));
            paramGroup.add(midiPitch.set("midiPitch", p, 36, 54));
            paramGroup.add(col.set("colour",100,ofColor(0,0),255));
            paramGroup.add(dmxChannel.set("dmxChannel", y*6+x+100, 0, 512));
            panel.setDefaultWidth(150);
            panel.setup(paramGroup);
            panel.setPosition(pos * ofVec2f(panel.getWidth()+25, panel.getHeight()+25));
            panel.loadFromFile("settings.xml");
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
    vector<Pad*> pads;
    
    vector<Sharpy*> sharpys;

    ofxOBJModel model;

    LaunchPad lp1, lp2;
    

    
    vector<ofAVFoundationPlayer*> launchPadMovies;
    
    LaunchPadEnvironment *lpe;
    
    vector<ofPixels*> pixelPerms;

};


