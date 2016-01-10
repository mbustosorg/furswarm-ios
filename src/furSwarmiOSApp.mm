//
//  furSwarmiOSApp.mm
//
//  $Date$
//  $Rev$
//  $Author$
//
//  Copyright (c) 2012, Mauricio Bustos
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, 
//  with or without modification, are permitted provided 
//  that the following conditions are met:
//
//  - Redistributions must not be for profit.  This software in source or binary form, 
//      with or without modification, cannot be included in anyproduct that is then sold.  
//      Please contact Mauricio Bustos m@bustos.org if you are interested in a for-profit license.
//  - Redistributions of source code or in binary form must reproduce the above copyright notice, 
//      this list of conditions and the following disclaimer in the documentation 
//      and/or other materials provided with the distribution.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
//  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
//  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
//  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
//  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#include <iostream>

#include "furSwarmiOSApp.h"

furSwarmiOSApp::furSwarmiOSApp() {
    theSwarm = new furSwarm();
}

//--------------------------------------------------------------
void furSwarmiOSApp::setup(){	
	// register touch events
	//ofRegisterTouchEvents(this);
	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
	
	//If you want a landscape oreintation 
	//iPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
    
	//ofSetFrameRate(120);
    ofSetCircleResolution(20);
	ofBackground(FS_APP_BACKGROUND_R, FS_APP_BACKGROUND_G, FS_APP_BACKGROUND_B);
    
    theSwarm->setup();
}

//--------------------------------------------------------------
void furSwarmiOSApp::update(){
    theSwarm->update();
}

//--------------------------------------------------------------
void furSwarmiOSApp::draw(){
	theSwarm->draw();
}

//--------------------------------------------------------------
void furSwarmiOSApp::exit(){
    
}

//--------------------------------------------------------------
void furSwarmiOSApp::touchDown(ofTouchEventArgs &touch){
    theSwarm->touchDown(touch);
}

//--------------------------------------------------------------
void furSwarmiOSApp::touchMoved(ofTouchEventArgs &touch){
    theSwarm->touchMoved(touch);
}

//--------------------------------------------------------------
void furSwarmiOSApp::touchUp(ofTouchEventArgs &touch){
    theSwarm->touchUp(touch);
}

//--------------------------------------------------------------
void furSwarmiOSApp::touchDoubleTap(ofTouchEventArgs &touch){
    
}

//--------------------------------------------------------------
void furSwarmiOSApp::lostFocus(){
    
}

//--------------------------------------------------------------
void furSwarmiOSApp::gotFocus(){
    
}

//--------------------------------------------------------------
void furSwarmiOSApp::gotMemoryWarning(){
    
}

//--------------------------------------------------------------
void furSwarmiOSApp::deviceOrientationChanged(int newOrientation){
    
}


//--------------------------------------------------------------
void furSwarmiOSApp::touchCancelled(ofTouchEventArgs& args){
    
}

