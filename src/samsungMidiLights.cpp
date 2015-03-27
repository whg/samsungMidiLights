#include "samsungMidiLights.h"

#define SHARPY_TILT 12
#define SHARPY_STROBE 2
#define SHARPY_DIMMER 3
#define SHARPY_STEP 16

//--------------------------------------------------------------
void samsungMidiLights::setup() {

    ofSetFrameRate(25);
    
//	ofSetVerticalSync(true);
	ofBackground(60);
    
    
    showGui = false;
//	ofSetLogLevel(OF_LOG_VERBOSE);
	
    model.load("/Users/whg/Desktop/2015_03_25_FOR_SIMULATION_STRIPPED_edit2.obj");
    model.scale(5);

//    vector<string> names = model.getGroupNames();
//    for(int i = 0; i < names.size(); i++) cout << names[i] << endl;

	midiIn.openPort("midiOne Bus 1");	// by name
	
	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, false, false);
	midiIn.addListener(this);
	midiIn.setVerbose(true);


    // start notes at 36
    stringstream ss;
    int k = 1;
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 6; j++) {
            
            ss.str("");
            ss << "panel" << k++;
//            ss << char('a' + (i-1));
//            ss << j;
            
            int midiNote = 34 + k; //((i-1)*4) + (j-1);
//            padKeys[midiNote] = Pad(ss.str(), midiNote, i-1, j-1);

            pads.push_back(Pad(ss.str(), midiNote, j-1, i-1));

            cout << midiNote << ": " << ss.str() << endl;

//            midiPitches.insert(pair<string, padParam>(ss.str(), padParam(ss.str(), midiNote, 1, 100)));
        }
    }
    
    
    parameters.setName("parameters");
    
    for(int i = 0; i < pads.size(); i++) {
        parameters.add(pads[i].midiPitch);
    }
    
    
    for (int i = 1, k = 0; i <= 6; i++) {
        for (int j = 1; j <= 2; j++, k++) {
            ss.str("");
            ss << "sharpy" << i << "_" << j;
            sharpys.push_back(Sharpy(ss.str(), 60 + k, i, j));
        }
    }

    
    
//    for (map<string, padParam>::iterator it = midiPitches.begin(); it != midiPitches.end(); ++it) {
//        parameters.add(it->second);
//        cout << it->first << endl;
//    }

    gui.setup(parameters);
    
    
    for (int i = 0; i < 16; i++) {
        ofPixels *p = new ofPixels();
        p->allocate(8, 8, 3);
        ofColor c;
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                float r = ofRandom(5);
                if (r > 4) c = ofColor::red;
                else if (r > 3) c = ofColor::yellow;
                else if (r > 2) c = ofColor::green;
                else c = ofColor::black;
                p->setColor(j, k, c);
            }
        }
        pixelPerms.push_back(p);
    }


    cam.setupPerspective();
    float f = cam.getFarClip();
    float n = cam.getNearClip();
    
    cam.setTranslationKey('t');
    cam.setGlobalOrientation(ofQuaternion(-0.703205466, 0.0471465364,  0.708562314, 0.0352183878));
    cam.setGlobalPosition(-1734.89771, 747.906738, -122.971588);


    dmx.connect("/dev/tty.usbserial-EN110089", sharpys.size() * SHARPY_STEP);


}

float theta = 0;

void samsungMidiLights::exit() {
    for (int i = 0; i < pixelPerms.size(); i++) {
        delete pixelPerms[i];
    }
    midiIn.closePort();
    midiIn.removeListener(this);
}


//--------------------------------------------------------------
void samsungMidiLights::update() {
    



//    lp1.begin();
//    ofClear(0, 255);
//    float r = ofMap(sin(ofGetElapsedTimef() * 4), -1, 1, 0, 6);
//    ofFill();
//    ofSetColor(ofColor::green);
//	ofDrawCircle(4, 4, r);
//	ofNoFill();
//	ofSetColor(ofColor::red);
//	ofDrawCircle(4, 4, r);
//    
//    lp1.end();
//    
//
//    
//    lp2.begin();
//    ofClear(0, 255);
//    ofSetColor(ofColor::red);
//
//    float t = 0.1;
//    ofPolyline l;
//
//    ofBeginShape();
//    for (int i = 0; i < 9; i++) {
//        l.addVertex(i, ofMap(sin(theta+i*mouseX/10.0), -1, 1, 0, 6));
//
//    }
//    ofEndShape();
////    theta+= t;
//    ofSetColor(ofColor::red);
//    ofSetLineWidth(1);
//    l.draw();
//    
//    
//    ofTranslate(0, 1);
//    ofSetColor(ofColor::yellow);
//    ofSetLineWidth(1);
//    l.draw();
//    
//    ofTranslate(0, 1);
//    ofSetColor(ofColor::green);
//    ofSetLineWidth(2);
//    l.draw();
//    
//    lp2.end();

    static int pc = 0;
    for (int i = 0; i < pads.size(); i++) {
        if (pads[i].on) {
            pc = i;
            break;
        }
    }
    
    lp2.set(*pixelPerms[pc]);
    
    ofPixels opixels(*pixelPerms[pc]);
    opixels.mirror(false, true);
    lp1.set(opixels);
    
    
//    for (vector<Pad>::iterator it =  pads.begin(); it != pads.end(); ++it) {
//        Pad p = *it;
//        g = model.getGroup(p.name);
////        cout << "name :"  << p.name << endl;
//
//        if (g == NULL) {
//            continue;
//        }
//        if (p.on) {
//            ofSetHexColor(0xffaabb);
//        }
//        else ofSetHexColor(0xffffff);
//        g->draw();
//
//
//        if (it == pads.begin()) {
//
//            dmx.setLevel(10, 125);
//            dmx.setLevel(16, 255);
//            dmx.setLevel(12, 125);
//
//            dmx.setLevel(3, 60);
//            dmx.setLevel(2, p.on ? 255 : 0);
//
//        }
//
//    }
    
    
    for (int i = 0; i < sharpys.size(); i++) {
        
        dmx.setLevel(i*SHARPY_STEP + SHARPY_STROBE, sharpys[i].on * 255);
        dmx.setLevel(i*SHARPY_STEP + SHARPY_DIMMER, 100); //sharpys[i].on * 255);
        dmx.setLevel(i*SHARPY_STEP + SHARPY_TILT, sharpys[i].value * 2);
        
        
    }
}

//--------------------------------------------------------------
void samsungMidiLights::draw() {
	ofSetColor(0);

    
    
    cam.begin();

    ofNoFill();
    ofSetHexColor(0xffffff);
    

    ofVec3f v = cam.getZAxis();
    
    ofRotate(1.5, v.x, v.y, v.z);
    ofScale(-0.25, 0.25, 0.25);
    model.draw(true);
    
    ofSetHexColor(0x000000);
    
//

//
//

    ofxOBJGroup *g;
    
//    for (map<int, Pad>::iterator it =  padKeys.begin(); it != padKeys.end(); ++it) {
    for (int i = 0; i < pads.size(); i++) {
//        cout << it->second.name << ": " << it->second.on << endl;
//        cout << pads[i].name
        g = model.getGroup(pads[i].name);
        if (g == NULL) {
            continue;
        }
        if (pads[i].on) {
            ofSetColor(pads[i].value*2);
            ofFill();
        }
        else ofSetHexColor(0xffffff);
        g->draw(pads[i].on);

    }

    ofPushStyle();
    ofSetLineWidth(5);

    for (int i = 0; i < sharpys.size(); i++) {
        g = model.getGroup(sharpys[i].name);
        if (g == NULL) {
            continue;
        }
        if (sharpys[i].on) {
//            ofSetColor(sharpys[i].value*2);
            ofFill();
            
            ofPushMatrix();
            
            int n = g->faces.size() - 1;
            ofTranslate((g->faces[n].vertices[0]));
            float rot = ofMap(sharpys[i].value, 0, 127, -180, -100);
            ofRotateX(rot * (((i/2) % 2 == 1) ? -1 : 1));
            
            ofSetLineWidth(5);
            ofDrawLine(0, 0, 0, 2500);
            
            ofPopMatrix();
            
        }
        
        
    }

//    stringstream ss;
//    for (int i = 1; i <= 6; i++) {
//        for (int j = 1; j <= 2; j++) {
//            ss.str("");
//            ss << "sharpy" << i << "_" << j;
//            
//            g = model.getGroup(ss.str());
//            ofPushMatrix();
//
//            int n = g->faces.size() - 1;
//            ofTranslate((g->faces[n].vertices[0]));
////            cout << ((i % 2 == 1) ? -1 : 1) << endl;
//            ofRotateZ(mouseX);// * ((i % 2 == 1) ? -1 : 1));
//            ofRotateX(mouseY  * ((i % 2 == 1) ? -1 : 1));
//            
//            ofSetLineWidth(5);
//            ofDrawLine(0, 0, 0, 2500);
//            
//            ofPopMatrix();
//        }
//    }
    
//    g = model.getGroup("sharpy1_1");
//    
//    int n = g->faces.size() - 1;
//    ofTranslate((g->faces[n].vertices[0]));
//    ofRotateY(mouseX);
//    ofRotateX(mouseY);
//    ofTranslate(-(g->faces[n].vertices[0]));
//    ofDrawCylinder(0, 0, 20, 500);

//    ofSetLineWidth(5);
//    ofDrawLine(0, 0, 0, 500);
    
//    ofSetColor(255, 0, 0);
//    ofDrawCircle(0, 0, 200);
    
    ofSetHexColor(0x00ff00);
//    g->draw(true);
    
    
    
    ofPopMatrix();
    ofPopStyle();
    
    cam.end();


    if (showGui) {
        gui.draw();
    }
    

    for (int i = 0; i < pads.size(); i++) {
        pads[i].colourPanel.draw();
    }


    dmx.update();

}

//--------------------------------------------------------------
void samsungMidiLights::newMidiMessage(ofxMidiMessage& msg) {

	// make a copy of the latest message
	midiMessage = msg;
    stringstream ss;
    ss << msg.channel << ": " << msg.value << "," << msg.pitch << "," << msg.velocity << "," << msg.control;
    cout << ss.str() << endl;
    
//    if (msg.status == MIDI_NOTE_ON) {
//        padKeys[msg.pitch].on = true;
//    }
//    else if (msg.status == MIDI_NOTE_OFF) {
//        padKeys[msg.pitch].on = false;
//    }
//

    for (int i = 0; i < pads.size(); i++) {
        Pad *p = &pads[i];
        if (p->midiPitch == msg.pitch) {
            if (msg.status == MIDI_NOTE_ON) {
                p->on = true;
                p->value = msg.velocity;
            }
            else if (msg.status == MIDI_NOTE_OFF) {
                p->on = false;
                p->value = 0;
            }
        }
    }
    
    for (int i = 0; i < sharpys.size(); i++) {
        Sharpy *p = &sharpys[i];
        if (p->midiPitch == msg.pitch) {
            if (msg.status == MIDI_NOTE_ON) {
                p->on = true;
                p->value = msg.velocity;
            }
            else if (msg.status == MIDI_NOTE_OFF) {
                p->on = false;
            }
        }
    }
    
}

//--------------------------------------------------------------
void samsungMidiLights::keyPressed(int key) {

	switch(key) {
		case 'l':
			midiIn.listPorts();
			break;
            
        case ' ':
            showGui = !showGui;
            break;
        case 'a':
            cout << cam.getOrientationQuat() << endl;
            break;
	}
    
}

//--------------------------------------------------------------
void samsungMidiLights::keyReleased(int key) {
}

//--------------------------------------------------------------
void samsungMidiLights::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void samsungMidiLights::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void samsungMidiLights::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void samsungMidiLights::mouseReleased() {
}

void samsungMidiLights::drawMPC(ofEventArgs &args) {
    
    ofBackgroundHex(0x000000);
    
    ofTranslate(0, 200);
    ofScale(1, -1);
    
    int d = 50;
    for (int i = 0; i < pads.size(); i++) {
        Pad *p = &pads[i];
        
        if (p->on) {
            ofSetColor(255);
        } else ofSetColor(0);
        
        ofDrawRectangle(p->pos.x * d, p->pos.y * d, d, d);
    }
}
