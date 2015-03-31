#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxOBJModel.h"
#include "ofxGui.h"
#include "ofxDmx.h"

#include "LaunchPad.h"

#define USE_DMX
#define DRAW_MODEL
#define USE_LAUNCHPAD


typedef enum {
    NITRO,
    SHARPY,
    VIPER,
    STROBE,
} light_t;

struct Light {
    string name;
    bool on;
    float value;
    ofVec2f pos;
    ofParameter<int> midiPitch;
    ofParameter<int> nidiPitch;
    ofParameter<int> yidiPitch;
    light_t type;
    
    ofxPanel panel;
    ofParameterGroup paramGroup;
    ofParameter<ofColor> col;
    ofParameter<int> dmxChannel;

    
    Light(): name(""), on(false) {}
    
    Light(string s, int p, bool addGui=false, int x=0, int y=0):
        name(s),
        on(false),
        pos(x, y),
        value(0) {
            type = NITRO;
            
            paramGroup.setName(s);
            paramGroup.add(midiPitch.set("midiPitch", p, 34, 78));
            paramGroup.add(nidiPitch.set("nidiPitch", p, 34, 78));
            paramGroup.add(yidiPitch.set("yidiPitch", p, 34, 78));
            
            paramGroup.add(col.set("colour",100,ofColor(0,0),255));
            paramGroup.add(dmxChannel.set("dmxChannel", y*6+x+100, 0, 512));
            
            if (addGui) {
                panel.setDefaultWidth(150);
                panel.setup(paramGroup);
                panel.setPosition(pos * ofVec2f(panel.getWidth()+10, panel.getHeight()+10));
                panel.loadFromFile("settings.xml");
            }
    }
};

struct Sharpy : public Light {
    float value2;
    ofParameter<int> tilt;
    ofParameter<int> pan;
    float t, p;

    Sharpy(string s, int p, bool addGui=false, int x=0, int y=0): Light(s, p, addGui, x, y) {
        type = SHARPY;
        panel.add(tilt.set("tilt", 10, 0, 255));
        panel.add(pan.set("pan", 10, 0, 255));
        panel.setPosition(pos * ofVec2f(panel.getWidth()+10, panel.getHeight()+10));
        t = p = 0;
        panel.loadFromFile("settings.xml");
        
    }
};

class samsungMidiLights : public ofBaseApp, public ofxMidiListener {
	
    bool showGui, viperGui, launchPadGui;
    
    ofxDmx dmx, dmxViper;
    
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
//    map<int, Pad> padKeys;
//    map<int, Light*> lights;
    vector<Light*> lights;
    vector<string> groupNames;
//    vector<Sharpy*> sharpys;

    ofxOBJModel model;

    ofxPanel panel;
    ofParameterGroup paramGroup;
    ofParameter<int> sharpyTiltMax;
    ofParameter<int> sharpyPanMax;
    ofParameter<int> viperTiltMax;
    ofParameter<int> viperPanMax;
    ofParameter<float> viperSpeed;
    ofParameter<float> viperSpace;
    
    ofxPanel launchPadPanel;
    ofParameterGroup lppg;
    map<int, ofParameter<int>* > lpparams;


    
    vector<ofAVFoundationPlayer*> launchPadMovies;
    
#ifdef USE_LAUNCHPAD
    LaunchPad lp1, lp2;
    LaunchPadEnvironment *lpe;
#endif

    vector<ofPixels*> pixelPerms;


    void resetView();
};


