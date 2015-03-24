#include "samsungMidiLights.h"

//--------------------------------------------------------------
void samsungMidiLights::setup() {

    ofSetFrameRate(30);
    
//	ofSetVerticalSync(true);
	ofBackground(0);
    
    showGui = false;
//	ofSetLogLevel(OF_LOG_VERBOSE);
	
    model.load("/Users/whg/Desktop/o2.obj");
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

}

float theta = 0;

//--------------------------------------------------------------
void samsungMidiLights::update() {

    lp1.begin();
    ofClear(0, 255);
    float r = ofMap(sin(ofGetElapsedTimef() * 4), -1, 1, 0, 6);
    ofFill();
    ofSetColor(ofColor::green);
	ofDrawCircle(4, 4, r);
	ofNoFill();
	ofSetColor(ofColor::red);
	ofDrawCircle(4, 4, r);
    
    lp1.end();
    
    
    lp2.begin();
    ofClear(0, 255);
    ofSetColor(ofColor::red);

    float t = 0.1;
    ofPolyline l;

    ofBeginShape();
    for (int i = 0; i < 9; i++) {
        l.addVertex(i, ofMap(sin(theta+i*0.5), -1, 1, 0, 8));

    }
    ofEndShape();
    theta+= t;
    ofSetLineWidth(3);
    l.draw();
    
    ofSetColor(ofColor::yellow);
    ofSetLineWidth(1);
    l.draw();
    
    lp2.end();
}

//--------------------------------------------------------------
void samsungMidiLights::draw() {
	ofSetColor(0);
	

    cam.begin();
    
    ofSetHexColor(0xffffff);
    model.draw();
    
    ofSetHexColor(0x000000);
    
    ofxOBJGroup *g;
    
    for (vector<Pad>::iterator it =  pads.begin(); it != pads.end(); ++it) {
        Pad p = *it;
        g = model.getGroup(p.name);
//        cout << "name :"  << p.name << endl;

        if (g == NULL) {
            continue;
        }
        if (p.on) {
            ofSetHexColor(0xffaabb);
        }
        else ofSetHexColor(0xffffff);
        g->draw();

    }

    
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
    
}

//--------------------------------------------------------------
void samsungMidiLights::exit() {
	
	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
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
