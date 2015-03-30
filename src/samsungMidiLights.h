#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxOBJModel.h"
#include "ofxGui.h"
#include "ofxDmx.h"

#include "LaunchPad.h"

//#define USE_DMX
#define DRAW_MODEL
#define USE_LAUNCHPAD


typedef enum {
    LED,
    SHARPY,
    GOBO,
} light_t;

struct Light {
    string name;
    bool on;
    float value;
    ofVec2f pos;
    ofParameter<int> midiPitch;
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
            type = LED;
            
            paramGroup.setName(s);
            paramGroup.add(midiPitch.set("midiPitch", p, 36, 54));
            paramGroup.add(col.set("colour",100,ofColor(0,0),255));
            paramGroup.add(dmxChannel.set("dmxChannel", y*6+x+100, 0, 512));

            
            if (addGui) {
                panel.setDefaultWidth(150);
                panel.setup(paramGroup);
                panel.setPosition(pos * ofVec2f(panel.getWidth()+25, panel.getHeight()+25));
//                panel.loadFromFile("settings.xml");
            }
    }
};

//struct Sharpy : public Pad {
//    float value2;
//    Sharpy(string s, int p, int x, int y): Pad(s, p, x, y){
//        value = 0;
//        value2 = 0;
//    }
//};

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
//    map<int, Pad> padKeys;
//    map<int, Light*> lights;
    vector<Light*> lights;
    vector<string> groupNames;
//    vector<Sharpy*> sharpys;

    ofxOBJModel model;

    

    
    vector<ofAVFoundationPlayer*> launchPadMovies;
    
#ifdef USE_LAUNCHPAD
    LaunchPad lp1, lp2;
    LaunchPadEnvironment *lpe;
#endif

    vector<ofPixels*> pixelPerms;


    void resetView();
};


