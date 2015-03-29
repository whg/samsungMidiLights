#include "samsungMidiLights.h"
#include <iomanip>

#define SHARPY_TILT 12
#define SHARPY_STROBE 2
#define SHARPY_DIMMER 3
#define SHARPY_STEP 16
#define SHARPY_ON 16
#define LIGHT_STEP 4

//#define USE_DMX
//#define DRAW_MODEL

//--------------------------------------------------------------
void samsungMidiLights::setup() {

    ofSetFrameRate(25);
    
//	ofSetVerticalSync(true);
	ofBackground(20);
    
    
    showGui = false;
//	ofSetLogLevel(OF_LOG_VERBOSE);
	
    model.load("/Users/whg/Desktop/samsung Lights project/2015_03_25_FOR_SIMULATION_STRIPPED_edit2.obj");
//    model.scale(5);


	midiIn.openPort("virtualMIDI Bus 1");	// by name
	
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
            int midiNote = 34 + 24 + k; //((i-1)*4) + (j-1);
            pads.push_back(new Pad(ss.str(), midiNote, j-1, i-1));

            cout << midiNote << ": " << ss.str() << endl;
        }
    }
    
    

    
//    for (int i = 0; i < 16; i++) {
//        ofPixels *p = new ofPixels();
//        p->allocate(8, 8, 3);
//        ofColor c;
//        for (int j = 0; j < 8; j++) {
//            for (int k = 0; k < 8; k++) {
//                float r = ofRandom(5);
//                if (r > 4) c = ofColor::red;
//                else if (r > 3) c = ofColor::yellow;
//                else if (r > 2) c = ofColor::green;
//                else c = ofColor::black;
//                p->setColor(j, k, c);
//            }
//        }
//        pixelPerms.push_back(p);
//    }
    
    for (int i = 1, k = 0; i <= 6; i++) {
        for (int j = 1; j <= 2; j++, k++) {
            ss.str("");
            ss << "sharpy" << i << "_" << j;
            sharpys.push_back(new Sharpy(ss.str(), 60 + k, i, j));
        }
    }



    cam.setupPerspective();
    float f = cam.getFarClip();
    float n = cam.getNearClip();
    
    cam.setTranslationKey('t');
    cam.setGlobalOrientation(ofQuaternion(-0.703205466, 0.0471465364, 0.708562314, 0.0352183878));
    cam.setGlobalPosition(-1734.89771, 747.906738, -122.971588);


    lpe = new LaunchPadEnvironment();
//    for (int i = 0; i < 48; i++) {
//        ss.str("");
////        ss << "0" << (i+1) << ".mov";
//        ss << setfill('0') << setw(2) << (i+1) << ".mov";
//        cout << ss.str() << endl;
//        
//        ofFile file(ss.str());
//        if (!file.exists()) {
//            continue;
//        }
//        
//        ofAVFoundationPlayer *player = new ofAVFoundationPlayer();
//        player->load(ss.str());
//        player->play();
//        player->setLoopState(OF_LOOP_NORMAL);
//        launchPadMovies.push_back(player);
//    }

//    launchPadMovies.push_back(new LPCircularAnimation);
//    launchPadMovies.push_back(new LPSineAnimation);

    dmx.connect("/dev/tty.usbserial-EN110089", 200); //sharpys.size() * SHARPY_STEP + pads.size() * LIGHT_STEP);


}

float theta = 0;

void samsungMidiLights::exit() {
    for (int i = 0; i < pixelPerms.size(); i++) {
        delete pixelPerms[i];
    }
    midiIn.closePort();
    midiIn.removeListener(this);
    
    lp1.setAll(ofxLaunchpadColor::OFF_BRIGHTNESS_MODE);
    lp2.setAll(ofxLaunchpadColor::OFF_BRIGHTNESS_MODE);
    
    for (int i = 0; i < pads.size(); i++) {
        delete pads[i];
    }
    for (int i = 0; i < sharpys.size(); i++) {
        delete sharpys[i];
    }
    for (int i = 0; i < launchPadMovies.size(); i++) {
        delete launchPadMovies[i];
    }
}

int pc = 0;
//--------------------------------------------------------------
void samsungMidiLights::update() {
    
//    static int pc = 0;
    
    if (ofGetFrameNum() % 2 == 0) {
//        pc = -1;
//        for (int i = 0; i < pads.size(); i++) {
//            if (pads[i]->on) {
//                pc = i;
//                break;
//            }
//        }
//        
//        
//        if (pc == -1) {
//            lp1.setAll(ofxLaunchpadColor::OFF_BRIGHTNESS_MODE);
//            lp2.setAll(ofxLaunchpadColor::OFF_BRIGHTNESS_MODE);
//        }
//        else {
//            
//            int q = pc % launchPadMovies.size();
//            ofAVFoundationPlayer *player = launchPadMovies[q];
////            player->nextFrame();
//            player->update();
//            lp2.set(player->getPixels());
////            lp2.set(*pixelPerms[pc]);
//            
//            ofPixels opixels(player->getPixels());
//            opixels.mirror(false, true);
//            lp1.set(opixels);
//        }

        lp1.set(lpe->getPixels());

    }
    
#ifdef USE_DMX
    
    for (int i = 0; i < pads.size(); i++) {
        Pad *p = pads[i];
        dmx.setLevel(p->dmxChannel, p->value);
        dmx.setLevel(p->dmxChannel + 1, p->col->r);
        dmx.setLevel(p->dmxChannel + 2, p->col->g);
        dmx.setLevel(p->dmxChannel + 3, p->col->b);
    }

    
    for (int i = 0; i < sharpys.size(); i++) {
        
        dmx.setLevel(i*SHARPY_STEP + SHARPY_STROBE, 255);
        dmx.setLevel(i*SHARPY_STEP + SHARPY_DIMMER, sharpys[i]->value * 0.5); //sharpys[i]->on * 255);
        dmx.setLevel(i*SHARPY_STEP + SHARPY_TILT, sharpys[i]->value);
        dmx.setLevel(i*SHARPY_STEP + SHARPY_ON, 255);

        
    }
    
    dmx.update();

#endif
}


//--------------------------------------------------------------
void samsungMidiLights::draw() {

	ofSetColor(0);
    

#ifdef DRAW_MODEL
    cam.begin();

    ofNoFill();
    ofSetHexColor(0xffffff);
    

    ofVec3f v = cam.getZAxis();
    
    ofRotate(1.5, v.x, v.y, v.z);
    ofScale(-1, 1, 1);
    model.draw(true);
    
    ofSetHexColor(0x000000);
    
    ofxOBJGroup *g;
    
//    for (map<int, Pad>::iterator it =  padKeys.begin(); it != padKeys.end(); ++it) {
    for (int i = 0; i < pads.size(); i++) {
//        cout << it->second.name << ": " << it->second.on << endl;
//        cout << pads[i]->name
        g = model.getGroup(pads[i]->name);
        if (g == NULL) {
            continue;
        }
        if (pads[i]->on) {
            float intensity = ofMap(pads[i]->value, 0, 127, 0, 255);
            ofSetColor(pads[i]->col->r, pads[i]->col->g, pads[i]->col->b, intensity);
            ofFill();
        }
        else ofSetHexColor(0xffffff);
        g->draw(pads[i]->on);

    }

    ofPushStyle();
    ofSetLineWidth(5);

    for (int i = 0; i < sharpys.size(); i++) {
        g = model.getGroup(sharpys[i]->name);
        if (g == NULL) {
            continue;
        }
        if (sharpys[i]->on  || true) {
//            ofSetColor(sharpys[i]->value*2);
            ofFill();
            
            ofPushMatrix();
            
            int n = g->faces.size() - 1;
            ofTranslate((g->faces[n].vertices[0]));
            float rot = ofMap(sharpys[i]->value, 0, 127, -180, -100);
            ofRotateX(rot * (((i/2) % 2 == 1) ? -1 : 1));
            
            ofSetLineWidth(5);
            ofDrawLine(0, 0, 0, 2500);
            
            ofPopMatrix();
            
        }
        
        
    }

    
    ofPopMatrix();
    ofPopStyle();
    
    cam.end();
    
#endif

   
    if (showGui) {
        for (int i = 0; i < pads.size(); i++) {
            pads[i]->panel.draw();
        }
    }
    
    

}

//--------------------------------------------------------------
void samsungMidiLights::newMidiMessage(ofxMidiMessage& msg) {

	// make a copy of the latest message
	midiMessage = msg;
    stringstream ss;
    ss << "(" << msg.status << ")" << msg.channel << ": " << msg.value << "," << msg.pitch << "," << msg.velocity << "," << msg.control;
    cout << ss.str() << endl;
    
//    if (msg.status == MIDI_NOTE_ON) {
//        padKeys[msg.pitch].on = true;
//    }
//    else if (msg.status == MIDI_NOTE_OFF) {
//        padKeys[msg.pitch].on = false;
//    }
//

    for (int i = 0; i < pads.size(); i++) {
        Pad *p = pads[i];
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
        Sharpy *p = sharpys[i];
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
    
    if (key == 's') {
        for (int i  = 0; i < pads.size(); i++) {
            pads[i]->panel.saveToFile("settings.xml");
        }
        cout << "saved all settings" << endl;
    }
    else if (key == 'c') {
        for (int i  = 0; i < pads.size(); i++) {
            pads[i]->value = 0;
            pads[i]->on = false;
        }
    }
    else if (key == OF_KEY_UP) {
//        pc++;
        lpe->add(new LPDot);

    }
    else if (key == OF_KEY_DOWN) {
//        pc--;
        lpe->add(new LPCircle);
    }
    else if (key == OF_KEY_RIGHT) {
        lpe->add(new LPAntiClockLine);
    }
    else if (key == OF_KEY_LEFT) {
        lpe->add(new LPClockLine);
    }
    
    
    if (showGui) {
        cam.disableMouseInput();
    }
    else {
        cam.enableMouseInput();
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
        Pad *p = pads[i];
        
        if (p->on) {
            ofSetColor(255);
        } else ofSetColor(0);
        
        ofDrawRectangle(p->pos.x * d, p->pos.y * d, d, d);
    }
}
