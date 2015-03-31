#include "samsungMidiLights.h"
#include <iomanip>

#define SHARPY_PAN 10
#define SHARPY_TILT 12
#define SHARPY_STROBE 2
#define SHARPY_DIMMER 3
#define SHARPY_STEP 16
#define SHARPY_ON 16
#define LIGHT_STEP 4

#define NITRO_STEP 12
#define NITRO_RED 7
#define NITRO_GREEN 9
#define NITRO_BLUE 8
#define NITRO_INTENSITY 6
#define NITRO_MASTER_INTENSITY 1

#define VIPER_INTENSITY 2
#define VIPER_PAN 25
#define VIPER_TILT 27
#define VIPER_CYAN 4

#define OPTIMISE _DRAW 1

Light *strobe1;
Light *strobe2;

int lastReceived = 0;
int midiNotes[] = { 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 58, 48, 49, 50, 51, 52, 53, 56, 57, 62, 63, 64, 59, 66, 74, 76, 34, 35, 47 };


#define NUM_DMX_CHANNELS 406
#define FRAMERATE 30
#define LAUNCHPAD_FRAMERATE 30

ofVec3f deskCenter;
float stheta = 0, vtheta = 0;
vector<int> lptoplay;
//--------------------------------------------------------------
void samsungMidiLights::setup() {

    ofSetFrameRate(FRAMERATE);

//	ofSetVerticalSync(true);
	ofBackground(20);
    
    paramGroup.setName("settings...");
    paramGroup.add(sharpyTiltMax.set("sharpyTiltMax", 10, 0, 100));
    paramGroup.add(sharpyPanMax.set("sharpyPanMax", 10, 0, 100));
    paramGroup.add(viperTiltMax.set("viperTiltMax", 2, 0, 20));
    paramGroup.add(viperPanMax.set("viperPanMax", 2, 0, 20));
    paramGroup.add(viperSpeed.set("viperSpeed", 1, 0, 1));
    paramGroup.add(viperSpace.set("viperSpace", 1, 0, 10));

    panel.setup(paramGroup);
    panel.setPosition(ofPoint(500, 620));
    panel.loadFromFile("settings.xml");
    
    lppg.setName("Launchpad animations");
    stringstream ls;
    for (int i = 0; i < 29; i++) {
        ls.str("");
        ls << "Note" << midiNotes[i];
        lpparams[midiNotes[i]] = new ofParameter<int>(ls.str(), i % 10, 0, 10);
        lppg.add(*lpparams[midiNotes[i]]);
    }
    launchPadPanel.setup(lppg);
    
    showGui = false;
    viperGui = false;
    launchPadGui = false;
//	ofSetLogLevel(OF_LOG_VERBOSE);
	
    model.load("/Users/whg/Desktop/samsung Lights project/2015_03_28_FOR_SIMULATION_STRIPPED_edit3.obj");

//    vector<string> names = model.getGroupNames();
//    for (int i = 0; i < names.size(); i++) cout << names[i] << endl;
//    ofExit();
    
    
    deskCenter = model.getGroup("center")->faces[0].vertices[0];
	
	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, false, false);
	midiIn.addListener(this);
	midiIn.setVerbose(true);
    midiIn.listPorts();
    midiIn.openPort("virtualMIDI Bus 1");	// by name


    // start notes at 36
    int midiNote = 36;
    stringstream ss;
    int k = 1;
    int dmxChannel = 0
    
    ;
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 4; j++) {
            
            ss.str("");
            ss << "panel" << k;
//            int midiNote = 34 + 24 + k; //((i-1)*4) + (j-1);
            Light *l = new Light(ss.str(), midiNote++, true, j+1, i-1);
            l->dmxChannel = dmxChannel;
            dmxChannel+= 12;
            lights.push_back(l);
            k++;
//            cout << midiNote << ": " << ss.str() << endl;
        }
    }
    
    
    dmxChannel = 200;
    for (int i = 1; i <= 12; i++) {
        ss.str("");
        ss << "sharpy" << i;
        int x = 0;
        int y = (i-1) / 4;// + (i-1) % 2;
        if (((i-1) / 2) % 2 == 1) {
            x = 6;
//            y--;
        }
        if ((i-1) % 2 == 1) {
            x++;
        }
        Light *l = new Sharpy(ss.str(), 41, true, x, y);
//        l->type = SHARPY;
        l->dmxChannel = dmxChannel;
        dmxChannel+= 16;
        lights.push_back(l);

    }
    
    dmxChannel = 0;
    for (int i = 1; i <= 11; i++) {
        ss.str("");
        ss << "viper" << i;
        int x = (i-1) % 6;
        int y = 0;
        if (i > 6) {
            y = 1;
            x+= 1;
        }
        Light *l = new Sharpy(ss.str(), 41, true, x, y);
        l->type = VIPER;
        l->dmxChannel = dmxChannel;
        dmxChannel+= 34;
        lights.push_back(l);
        
    }
    
    strobe1 = new Light("strobe1", 36, true, 2, 4);
    strobe1->dmxChannel = 401;
    strobe1->type = STROBE;
    lights.push_back(strobe1);

//    strobe1->type = STROBE;
    strobe2 = new Light("strobe2", 36, true, 3, 4);
    strobe2->dmxChannel = 402;
    strobe2->type = STROBE;
    lights.push_back(strobe2);
    //    strobe2->type = STROBE;
    
    strobe1->panel.setPosition(ofVec2f(200, 500));
    strobe2->panel.setPosition(ofVec2f(400, 500));
    
    cam.setupPerspective();
    float f = cam.getFarClip();
    float n = cam.getNearClip();
    
    cam.setTranslationKey('t');
    resetView();

#ifdef USE_LAUNCHPAD
    lpe = new LaunchPadEnvironment();
#endif


#ifdef USE_DMX
    dmx.connect("/dev/tty.usbserial-EN110089", NUM_DMX_CHANNELS); //sharpys.size() * SHARPY_STEP + lights.size() * LIGHT_STEP);
    dmxViper.connect("/dev/tty.usbserial-EN128343", 34*11);
#endif


}


float theta = 0;

void samsungMidiLights::exit() {
    for (int i = 0; i < pixelPerms.size(); i++) {
        delete pixelPerms[i];
    }
    midiIn.closePort();
    midiIn.removeListener(this);
    
    for (int i = 0; i < lights.size(); i++) {
        Light *p = lights[i];
        
        if (p->type == NITRO) {
            dmx.setLevel(p->dmxChannel + NITRO_MASTER_INTENSITY, 0);
            dmx.setLevel(p->dmxChannel + NITRO_INTENSITY, 0);
        }
        else if (p->type == SHARPY) {
            dmx.setLevel(p->dmxChannel + SHARPY_DIMMER, 0);
        }
    }
    
    for(map<int, ofParameter<int>* >::iterator it = lpparams.begin(); it != lpparams.end(); ++it) {
        delete it->second;
    }
    
#ifdef USE_LAUNCHPAD
    lp1.setAll(ofxLaunchpadColor::OFF_BRIGHTNESS_MODE);
    lp2.setAll(ofxLaunchpadColor::OFF_BRIGHTNESS_MODE);
#endif

    for (int i = 0; i < lights.size(); i++) {
        delete lights[i];
    }
//    for (int i = 0; i < sharpys.size(); i++) {
//        delete sharpys[i];
//    }
    for (int i = 0; i < launchPadMovies.size(); i++) {
        delete launchPadMovies[i];
    }
}

int pc = 0;
//--------------------------------------------------------------
void samsungMidiLights::update() {

    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    //add the launch pad animation
    for (int i = 0; i < lptoplay.size(); i++) {
        lpe->add(animationCreator(lptoplay[i]));
    }
    lptoplay.clear();
    
    if (pc >= FRAMERATE) {
        pc = 0;
#ifdef USE_LAUNCHPAD
        ofPixels pix(lpe->getPixels());
        lp1.set(pix);
        pix.mirror(false, true);
        lp2.set(pix);
#endif
    }
    
    pc+= LAUNCHPAD_FRAMERATE;
    
#ifdef USE_DMX
    
    for (int i = 0; i < lights.size(); i++) {
        Light *p = lights[i];
        
        if (p->type == NITRO) {
            dmx.setLevel(p->dmxChannel + NITRO_MASTER_INTENSITY, 255);
            dmx.setLevel(p->dmxChannel + 12, 255);
            dmx.setLevel(p->dmxChannel + 11, 255);
            dmx.setLevel(p->dmxChannel + 2, 0);
            dmx.setLevel(p->dmxChannel + 3, 0);
            dmx.setLevel(p->dmxChannel + 4, 0);
            dmx.setLevel(p->dmxChannel + NITRO_INTENSITY, p->value*2);
            dmx.setLevel(p->dmxChannel + NITRO_RED, p->col->r*2);
            dmx.setLevel(p->dmxChannel + NITRO_BLUE, p->col->g*2);
            dmx.setLevel(p->dmxChannel + NITRO_GREEN, p->col->b*2);
        }
        else if (p->type == SHARPY) {
            Sharpy *s = (Sharpy*) p;
            dmx.setLevel(p->dmxChannel + SHARPY_DIMMER, p->value);
            dmx.setLevel(p->dmxChannel + SHARPY_TILT, s->tilt + s->t);
            dmx.setLevel(p->dmxChannel + SHARPY_PAN, s->pan + s->p);
            dmx.setLevel(p->dmxChannel + SHARPY_STROBE, 255);
            dmx.setLevel(p->dmxChannel + 16, 255);
        }
        else if (p->type == VIPER) {

            Sharpy *s = (Sharpy*) p;

            dmxViper.setLevel(p->dmxChannel + VIPER_INTENSITY, p->value);
            dmxViper.setLevel(p->dmxChannel + VIPER_TILT, ofClamp(s->tilt + s->t, 0, 255));
            dmxViper.setLevel(p->dmxChannel + VIPER_PAN, ofClamp(s->pan + s->p, 0, 255));
//            cout << int(s->p) << endl;
            dmxViper.setLevel(p->dmxChannel + 1, 25); //shutter
//            dmxViper.setLevel(p->dmxChannel + 2, 255);
            dmxViper.setLevel(p->dmxChannel + 21, 255); //zoom
            dmxViper.setLevel(p->dmxChannel + 12, 10); //zoom
            dmxViper.setLevel(p->dmxChannel + VIPER_CYAN, s->col->b);
    

        }
        
        
    }

    dmx.setLevel(strobe1->dmxChannel, strobe1->value > 0 ? 255 : 0);
    dmx.setLevel(strobe2->dmxChannel, strobe2->value > 0 ? 255 : 0);

    if (strobe2->value > 255) {
        strobe2->value = 0;
    }
    if (strobe1->value > 255) {
        strobe1->value = 0;
    }
    
    dmx.update();
    dmxViper.update();
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
    
    float rotY = ofMap(mouseY, 0, ofGetHeight(), 0, 360);
    float rotX = ofMap(mouseX, 0, ofGetWidth(), -180, -100);
    int sharpyc = 0;
    
//    for (map<int, Pad>::iterator it =  padKeys.begin(); it != padKeys.end(); ++it) {
    for (int i = 0; i < lights.size(); i++) {
        g = model.getGroup(lights[i]->name);
        if (g == NULL) {
            continue;
        }
        if (lights[i]->on) {
            float intensity = ofMap(lights[i]->value, 0, 127, 0, 255);
            ofSetColor(lights[i]->col->r, lights[i]->col->g, lights[i]->col->b, intensity);
            ofFill();
#ifdef OPTIMISE_DRAW
            g->drawMesh(lights[i]->on);
#else
            g->draw(lights[i]->on);
#endif
        }
//        else ofSetHexColor(0xffffff);

        
        if (lights[i]->type == SHARPY || lights[i]->type == VIPER) {
//            ofPushMatrix();
            ofPushStyle();
            int n = g->faces.size() - 1;
//            ofTranslate((g->faces[n].vertices[0]));
//            float rotY = ofMap(mouseY, 0, 900, 0, 360);
//            float flip = lights[i](((i/2) % 2 == 1) ? -1 : 1)
//            ofRotateX(rotX);// rot * ((i % 2 == 0) ? -1 : 1));
//            ofRotateZ(rotY); // * ((i % 2 == 0) ? -1 : 1));
            
//            ofRotateX(144 * ((i % 2 == 0) ? -1 : 1));
//            ofRotateZ(390);
            
            ofSetLineWidth(5);
            ofSetHexColor(0xffffff);
//            ofDrawLine(0, 0, 0, deskCenter.x, deskCenter.y, deskCenter.z);
            ofVec3f end(deskCenter);
            float zr = sin(stheta +sharpyc*0.2);
            end.z-= zr * 100 * ((sharpyc % 2 == 0) ? -1 : 1);
            
            Sharpy *s = (Sharpy*) lights[i];
            if (s->type == SHARPY) s->t = zr * sharpyTiltMax;
            else if (s->type == VIPER) {
             s->t = zr * viperTiltMax;
            }
            
            
            float xr = sin(stheta +sharpyc*0.5);
            end.x-= xr * 200;
            if (s->type == SHARPY) s->p = xr * sharpyPanMax;
            else if (s->type == VIPER) s->p = xr * viperPanMax;
            
            if (s->on && s->type == SHARPY){
                ofDrawLine(g->faces[n].vertices[0], end);
            }
            
            ofPopStyle();
//            ofPopMatrix();
            sharpyc++;
        }

    }
    
    for (int i = 0; i < lights.size(); i++) {
        if (lights[i]->type == VIPER) {
            float zr = sin(vtheta +viperSpace*1.2*i);
            
            Sharpy *s = (Sharpy*) lights[i];
            s->t = zr * viperTiltMax;
            float xr = sin(vtheta +viperSpace*2.5*i);
            s->p = xr * viperPanMax;


        }
    }
    vtheta+= viperSpeed;
    stheta+= 0.05;
    

//    cout << rotX << ", " << rotY << endl;

    ofPushStyle();
    ofSetLineWidth(5);


    
    ofPopMatrix();
    ofPopStyle();
    
    cam.end();
    
#endif

   
    if (showGui) {
        for (int i = 0; i < lights.size(); i++) {
            if (lights[i]->type == SHARPY || lights[i]->type == NITRO) lights[i]->panel.draw();
        }
        panel.draw();
    }
    if (viperGui) {
        for (int i = 0; i < lights.size(); i++) {
            if (lights[i]->type == VIPER || lights[i]->type == STROBE) {
                lights[i]->panel.draw();
            }
        }
    }
    if (launchPadGui) {
        launchPadPanel.draw();
    }
    
    
    ofSetColor(255);
    ofDrawBitmapString(ofToString(lastReceived), 0, 800);

}

//--------------------------------------------------------------
void samsungMidiLights::newMidiMessage(ofxMidiMessage& msg) {

	// make a copy of the latest message
	midiMessage = msg;
//    stringstream ss;
////    ss << "(" << msg.status << ")" << msg.channel << ": " << msg.value << "," << msg.pitch << "," << msg.velocity << "," << msg.control;
////    cout << ss.str() << endl;
//    
//    if (msg.status == MIDI_NOTE_ON) {
//        ss << "(NOTE_ON) " << msg.pitch << ": " << msg.velocity;
//
//    }
//    if (msg.status == MIDI_NOTE_OFF) {
//        ss << "(NOTE_OFF) "<< msg.pitch << ": " << msg.velocity;
//        
//    }
//    else if (msg.status == MIDI_CONTROL_CHANGE) {
//        ss << "(CONTROL) " << msg.control << ": " << msg.value;
//    }
//    
////    ss << msg.channel << ": " << msg.value << "," << msg.pitch << "," << msg.velocity << "," << msg.control;
//    cout << ss.str() << endl;
//

    for (int i = 0; i < lights.size(); i++) {
        Light *p = lights[i];
        if (msg.status == MIDI_CONTROL_CHANGE &&
            (msg.control == p->midiPitch ||
             msg.control == p->nidiPitch ||
             msg.control == p->yidiPitch)) {
             
            p->value = msg.value;
            
            if (msg.value < 1) {
                p->on = false;
            }
            else if (msg.value > 124){
                p->on = true;
                lastReceived = msg.control;
                int v = *lpparams[msg.control];
                if (v != 0) lptoplay.push_back(v-1);

            }
        }
        else if (p->midiPitch == msg.pitch ||
                 p->nidiPitch == msg.pitch ||
                 p->yidiPitch == msg.pitch) {
            
            if (msg.status == MIDI_NOTE_ON) {
                p->on = true;
                p->value = 128;
                lastReceived = msg.pitch;
                            }
            else if (msg.status == MIDI_NOTE_OFF) {
                p->on = false;
//                p->value = 0;
            }
        }
    }
    
//    for (int i = 0; i < sharpys.size(); i++) {
//        Sharpy *p = sharpys[i];
//        if (p->midiPitch == msg.pitch) {
//            if (msg.status == MIDI_NOTE_ON) {
//                p->on = true;
//                p->value = msg.velocity;
//            }
//            else if (msg.status == MIDI_NOTE_OFF) {
//                p->on = false;
//            }
//        }
//    }
    
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
        for (int i  = 0; i < lights.size(); i++) {
            lights[i]->panel.saveToFile("settings.xml");
        }
        launchPadPanel.saveToFile("settings.xml");
        panel.saveToFile("settings.xml");
        cout << "saved all settings" << endl;
    }
    if (key == 'l') {
        for (int i  = 0; i < lights.size(); i++) {
            lights[i]->panel.loadFromFile("settings.xml");
        }
        launchPadPanel.loadFromFile("settings.xml");
        panel.loadFromFile("settings.xml");
        cout << "loaded all settings" << endl;
    }
    else if (key == 'c') {
        for (int i  = 0; i < lights.size(); i++) {
            lights[i]->value = 0;
            lights[i]->on = false;
        }
    }
#ifdef USE_LAUNCHPAD
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
    else if (key >= '0' && key <= '9') {
        lpe->add(animationCreator(key - '0'));
    }
#endif
    else if (key == 'r') {
        resetView();
    }
    else if (key == 'z') {
        for (int i = 0; i < lights.size(); i++) {
            Light *p = lights[i];
            p->value = 0;
        }
    }
    else if (key == 'v') {
        viperGui = !viperGui;
    }
    else if (key == 'p') {
        launchPadGui = !launchPadGui;
    }
    else if (key  == 'o') {
        dmx.setLevel(401, 255);
        dmx.setLevel(402, 255);
        dmx.update();
        dmx.setLevel(401, 0);
        dmx.setLevel(402, 0);
        dmx.update();
        
        cout << "flash" << endl;
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
    for (int i = 0; i < lights.size(); i++) {
        Light *p = lights[i];
        
        if (p->on) {
            ofSetColor(255);
        } else ofSetColor(0);
        
        ofDrawRectangle(p->pos.x * d, p->pos.y * d, d, d);
    }
}


void samsungMidiLights::resetView() {
    cam.setGlobalOrientation(ofQuaternion(-0.703205466, 0.0471465364, 0.708562314, 0.0352183878));
    cam.setGlobalPosition(-1734.89771, 747.906738, -122.971588);

}