#include "samsungMidiLights.h"

//--------------------------------------------------------------
void samsungMidiLights::setup() {

    ofSetFrameRate(12);
    
//	ofSetVerticalSync(true);
	ofBackground(0);
    
    dmx.connect("/dev/tty.usbserial-EN110089");
    
    showGui = false;
//	ofSetLogLevel(OF_LOG_VERBOSE);
	
    model.load("/Users/whg/Desktop/2015_03_25_FOR_SIMULATION_STRIPPED_edit.obj");
    model.scale(5);
	
	midiIn.openPort("midiOne Bus 1");	// by name
	
	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, false, false);
	midiIn.addListener(this);
	midiIn.setVerbose(true);


    // start notes at 36
    stringstream ss;
    for (int i = 1; i <= 4; i++) {
        for (int j = 1; j <= 4; j++) {
            
            ss.str("");
            ss << char('a' + (i-1));
            ss << j;
            
            int midiNote = 36 + ((i-1)*4) + (j-1);
            padKeys[midiNote] = Pad(ss.str(), midiNote, i-1, j-1);

            pads.push_back(Pad(ss.str(), midiNote, j-1, i-1));

            cout << midiNote << ": " << ss.str() << endl;

//            midiPitches.insert(pair<string, padParam>(ss.str(), padParam(ss.str(), midiNote, 1, 100)));
        }
    }
    
    
    parameters.setName("parameters");
    
    for(int i = 0; i < pads.size(); i++) {
        parameters.add(pads[i].midiPitch);
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
                float r = ofRandom(8);
                if (r > 7) c = ofColor::red;
                else if (r > 6) c = ofColor::yellow;
                else if (r > 5) c = ofColor::green;
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
    
//    cam.setOrientation(ofQuaternion(-0.706509172, 0.0908926799, -0.687438547, 0.141468167));

//    cam.setPosition(945.860656, 392.853912, -92.8425445);
//    cam.setPosition(182.467392, 259.548157, -108.678383);
//    cam.setOrientation(ofQuaternion(-0.707637131, 0.0145274857, -0.706423163, 0.0087180743));
    
//    cam.setGlobalOrientation(ofQuaternion(-0.714855433, -0.00786958821, -0.703222095, 0.0272255018));
//    cam.setGlobalPosition(606.578186, 544.768005, -99.3945159);

    cam.setPosition(324.845123, 398.42157, -106.140259);
    cam.setOrientation(ofQuaternion(-0.694695234, -0.122944221, -0.694024741, 0.14353995));
//    cam.setPosition(606.578186, 544.768005, -99.3945159);
//    cam.setPosition(606.578186, 544.768005, -99.3945159);
}

float theta = 0;

void samsungMidiLights::exit() {
    for (int i = 0; i < pixelPerms.size(); i++) {
        delete pixelPerms[i];
    }
    
    // clean up
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
    
//    lp2.set(*pixelPerms[pc]);
    
    ofPixels opixels(*pixelPerms[pc]);
    opixels.mirror(false, true);
    lp1.set(opixels);
    
}

//--------------------------------------------------------------
void samsungMidiLights::draw() {
	ofSetColor(0);
	
//

    
//    cam.setPosition(ofGetFrameNum(), 0, 0);


    
    cam.begin();

    ofNoFill();
    ofSetHexColor(0xffffff);
    
//    glDisable(GL_CULL_FACE);
    ofVec3f v = cam.getZAxis();
    ofRotate(1.5, v.x, v.y, v.z);
    ofScale(0.25, 0.25, 0.25);
    model.draw(true);
    
    ofSetHexColor(0x000000);
    
//    ofxOBJGroup *g;
//    
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
//
//    
//    for (map<int, Pad>::iterator it =  padKeys.begin(); it != padKeys.end(); ++it) {
//        g = model.getGroup(it->second.name);
//        if (g == NULL) {
//            continue;
//        }
//        if (it->second.on) {
//            ofSetHexColor(0xffaabb);
//        }
//        else ofSetHexColor(0xffffff);
//        g->draw();
//    }
//    
    
    
    cam.end();


    if (showGui) {
        gui.draw();
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
        Pad p = pads[i];
        
        if (p.on) {
            ofSetColor(255);
        } else ofSetColor(0);
        
        ofDrawRectangle(p.pos.x * d, p.pos.y * d, d, d);
    }
}
