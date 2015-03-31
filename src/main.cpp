#include "ofMain.h"
#include "samsungMidiLights.h"
#include "ofAppGLFWWindow.h"
#include "LaunchPad.h"
//#include "MPC.h"

int main(){

    ofGLFWWindowSettings settings;
    
    settings.width = 1275;
    settings.height = 780;
    settings.position.x = 200;
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

    shared_ptr<samsungMidiLights> mainApp(new samsungMidiLights);
    


//    mainApp->setupGui();
//    ofAddListener(guiWindow->events().draw, mainApp.get(), &ofApp::drawGui);
    
    settings.width = 256;
    settings.height = settings.width;
    settings.position.x = 0;
    settings.resizable = false;
    shared_ptr<ofAppBaseWindow> launchPad1Window = ofCreateWindow(settings);
//    shared_ptr<LaunchPad> launchPad1(new LaunchPad);
//    ofAddListener(launchPad1Window->events().draw, launchPad1.get(), &LaunchPad::draw);

#ifdef USE_LAUNCHPAD
    ofAddListener(launchPad1Window->events().draw, &mainApp->lp1, &LaunchPad::draw);
#endif

//    settings.position.x = 1440-settings.width;
//    settings.position.y = 0;
//    settings.resizable = true;
//    shared_ptr<ofAppBaseWindow> launchPad2Window = ofCreateWindow(settings);
////    shared_ptr<LaunchPad> launchPad2(new LaunchPad);
//
//    ofAddListener(launchPad2Window->events().draw, &mainApp->lp2, &LaunchPad::draw);


    settings.width = 200;
    settings.height = 200;
    settings.position.x = 0;
    settings.position.y = 450;
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> mpcWindow = ofCreateWindow(settings);
    ofAddListener(mpcWindow->events().draw, mainApp.get(), &samsungMidiLights::drawMPC);


    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();


//	ofSetupOpenGL(640, 480, OF_WINDOW);
//	ofRunApp(new samsungMidiLights());
}
