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
//        ofxLaunchpad::setup(portCounter++);
    }
    
    void draw(ofEventArgs & args) {
        ofxLaunchpad::draw(0, 0);
    }
    
    
};

class LaunchPadAnimation : public ofAVFoundationPlayer {
protected:
public:
    float counter;
    ofFbo fbo;
    ofPixels pix;
    
    LaunchPadAnimation() {
        fbo.allocate(8, 8);
        counter = 0;
    }
    
    virtual void draw() {}
    
    virtual void update() {
        counter+= 0.75;
    }
    
    virtual bool isDead() {}
    
    virtual ofPixelsRef getPixels() {
        fbo.begin();
        ofPushMatrix();
        ofPushStyle();
        
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        
//        ofSetLineWidth(2);
        draw();
        
        ofPopMatrix();
        ofPopStyle();
        fbo.end();
        fbo.readToPixels(pix);
        
        return pix;
    }
    
    void reset() {
        counter = 0;

    }
    
};

class LPCircularAnimation : public LaunchPadAnimation {
public:
   
    
    void draw() {
   
        ofClear(0, 255);
        float r = ofMap(sin(counter), -1, 1, 0, 6);
//        ofFill();
//        ofSetColor(ofColor::green);
//        ofDrawCircle(4, 4, r);
        ofNoFill();
        ofSetColor(ofColor::red);
        ofDrawCircle(4, 4, r);
        
        
    }
};

class LPSineAnimation : public LaunchPadAnimation {
    ofPolyline l;
public:
    void draw() {
        ofClear(0, 255);
 
        float t = 0.1;
        l.clear();
        for (int i = 0; i < 9; i++) {
            l.addVertex(i, ofMap(sin(counter+i), -1, 1, 0, 6));

        }

        ofSetColor(ofColor::red);
        ofSetLineWidth(1);
        l.draw();


        ofTranslate(0, 1);
        ofSetColor(ofColor::yellow);
        ofSetLineWidth(1);
        l.draw();
        
        ofTranslate(0, 1);
        ofSetColor(ofColor::green);
        ofSetLineWidth(1);
        l.draw();
        
    }
};

static bool bisDead(shared_ptr<LaunchPadAnimation> a) {
    return a->isDead();
}

class LaunchPadEnvironment : public LaunchPadAnimation {
protected:
    vector< shared_ptr<LaunchPadAnimation> > animations;
public:
    
    void draw() {
        
        ofClear(0, 255);

        
        for(vector<shared_ptr<LaunchPadAnimation> >::iterator it = animations.begin(); it != animations.end(); ++it) {
            (*it)->draw();
            (*it)->update();
        }
        
        ofRemove(animations, bisDead);
        
//        cout << animations.size() << endl;
    }
    
    
    void add(LaunchPadAnimation *a) {
        animations.push_back(shared_ptr<LaunchPadAnimation>(a));
    }
};

class LPChevron : public LaunchPadAnimation {
    float pos;
public:
    LPChevron() {
        pos = 2;
    }
    
    void draw() {
        ofFill();
        ofSetColor(ofColor::red);
        ofDrawLine(pos, 4, pos-5, -1);
        ofDrawLine(pos, 4, pos-5, 9);
    }
    
    void update() {
        pos+= 1;
    }
    
    bool isDead() {
        return pos > 10;
    }
};

class LPVLine : public LaunchPadAnimation {
protected:
    float pos;
public:
    LPVLine() {
        pos = 0;
    }
    
    void draw() {
        ofFill();
        ofSetColor(ofColor::red);
        ofDrawLine(pos, 0, pos, 8);
    }
    
    void update() {
        pos+= 1;
    }
    
    bool isDead() {
        return pos > 10;
    }
};

class LPHLine : public LPVLine {
public:
    void draw() {
        ofFill();
        ofSetColor(ofColor::red);
        ofDrawLine(0, pos, 8, pos);
    }
};

class LPClockLine : public LaunchPadAnimation {
protected:
    float angle;
public:
    LPClockLine() {
        angle = 0;
    }
    void draw() {
        ofPushMatrix();
        ofFill();
        ofSetColor(ofColor::red);
        ofTranslate(0, 8);
        ofRotate(angle);
        ofDrawLine(0, 0, 0, -19);
        ofPopMatrix();
    }
    
    void update() {
        angle+= 10;
    }
    
    bool isDead() {
        return angle > 120;
    }
};

class LPAntiClockLine : public LPClockLine {
public:
    void draw() {
        ofPushMatrix();
        ofFill();
        ofSetColor(ofColor::red);
        ofRotate(-angle);
        ofDrawLine(0, 0, 0, 19);
        ofPopMatrix();
    }
};

class LPDot : public LPVLine {
public:
    void draw() {
        ofFill();
        ofSetColor(ofColor::red);
        ofDrawCircle(pos, 4, 2);
    }
};

class LPSmallHLines : public LPVLine {
public:
    void draw() {
        ofFill();
        ofSetColor(ofColor::red);
        for (int i = 1; i < 8; i+= 2) {
            ofDrawLine(pos, i, pos+2, i);
        }
        
    }
};

class LPDLineP : public LaunchPadAnimation {
protected:
    float pos;
public:
    LPDLineP() {
        pos = 1;
    }
    
    void draw() {
        ofFill();
        ofSetColor(ofColor::red);
        ofDrawLine(pos, 0, pos-8, 8);
    }
    
    void update() {
        pos+= 1;
    }
    
    bool isDead() {
        return pos > 16;
    }
};

class LPDLineN : public LPDLineP {
public:
    void draw() {
        ofFill();
        ofSetColor(ofColor::red);
        ofDrawLine(pos-8, 0, pos, 8);
    }
};


class LPCircle : public LaunchPadAnimation {
    float size;
public:
    LPCircle() {
        size = 0;
    }
    
    void draw() {
        ofNoFill();
        ofSetColor(ofColor::green);
        ofDrawCircle(4, 4, size);
    }
    
    void update() {
        size+= 1;
    }
    
    bool isDead() {
        return size > 6;
    }
};




LaunchPadAnimation animations[] = {
    
};

inline LaunchPadAnimation* animationCreator(int v) {
    switch(v) {
        case 0:
            return new LPChevron;
        case 1:
            return new LPVLine;
        case 2:
            return new LPDLineP;
        case 3:
            return new LPDLineN;
        case 4:
            return new LPCircle;
        case 5:
            return new LPDot;
        case 6:
            return new LPSmallHLines;
        
    }
}






