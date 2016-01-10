//
//  furSwarm.h
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

#pragma once

#include "swarmMember.h"
#include "swarmMemberDisplay.h"
#include "ofSlider.h"
#include "ofButton.h"
#include "ofEllipseButton.h"
#include "ofToggleButton.h"
#include "ofxiOSCoreLocation.h"
#include "furSwarmCommand.h"
#include "fsCharacterCommand.h"
#include "fsPatternParameters.h"
#include "xBeeControl.h"
#include "fsPayload.h"
#include "fsNodeDiscover.h"
#include "keyboard.h"
#include "animation1.h"
#include "animation2.h"
#include "animation3.h"
#include <map>

#define MAX_Y_SLIDE (470)
#define BROADCAST_THRESHOLD (2)

//! Type for collection of swarm members keyed by address16
typedef map<uint16_t, swarmMemberDisplay> SwarmCollection;

//! Project controller class
/*! 
 *  Main openFrameworks class for project.  Provides  
 *  the setup, update, draw functional control for 
 *  the system.
 */
class furSwarm {
	
public:
    furSwarm(); 
    ~furSwarm();
    void setup();
    void update();
    void animate();
    void drawMembers();
    void drawControls();
    void drawAltControls();
    void draw();
    
    // OF Callbacks
    void touchDown(ofTouchEventArgs & touch);
    void touchMoved(ofTouchEventArgs & touch);
    void touchUp(ofTouchEventArgs & touch);

    ofSlider *intensitySlider;
    ofSlider *redSlider;
    ofSlider *greenSlider;
    ofSlider *blueSlider;
    ofSlider *auxSlider;
    ofSlider *messageDelaySlider;
    ofSlider *secondModSlider;

private:
    void deactivateCommands(list<furSwarmCommand*>* commands, int x, int y);
    void slideCommands(list<furSwarmCommand*>* commands, int slideStep);
    string logTimestamp();
    void drawSlider();
    void rotateGradient(int x, int y);
    int feetToTarget(swarmMember* member);
    //bool gradientLessThan (swarmMemberDisplay* a, swarmMemberDisplay* b);

    int selectedColor;
    
	bool hasGPS;
	ofxiOSCoreLocation * coreLocation;
    
    map<uint16_t, swarmMemberDisplay> activeMembers;
    
    // Widgets 
    ofTrueTypeFont font;
    ofTrueTypeFont smallFont;
    ofTrueTypeFont fontSliderLabel;
    ofTrueTypeFont missingXbeeLabel;
    ofPath gradient;
    float lastGradientAngle = 0.0;
    int deltaYSlide;
    int slideStep;
    int lastMemberCount;
    bool sliding;
    ofButton *memberDetailButton;
    ofButton *nextPageButton;
    ofToggleButton *selectiveModeButton;
    ofToggleButton *receiptModeButton;
    ofToggleButton *daytimeModeButton;
    ofButton *resetNetworkButton;
    list<ofSlider*> activeSliders;
    keyboard *textKeyboard;
    int fpsTimestamp, fpsTimestampFrame;
    // Commands
    unsigned long commandDelayThreshold = 0;
    fsPatternParameters *patternParameters;
    furSwarmCommand* currentCommand;
    list<furSwarmCommand*>* activeCommands;
    list<furSwarmCommand*> page1Commands;
    list<furSwarmCommand*> page2Commands;
    animationCommand *currentAnimation;
    void sendCommand(furSwarmCommand* fsCommand);
    furSwarmCommand* commandByID(int identifier);
    void mapGPS();
    
    // XBee Attributes & Commands
    xBeeControl xBee;
    string heartbeatHistory;
    
    void broadcastString(fsCharacterCommand* characterCommand, string stringToBroadcast);
    void sendCommandRequest(furSwarmCommand* fsCommand, bool requestConfirmation);
    void broadcastPayload(fsPayload payload, bool requestConfirmation);
    void processResponses();
    void nodeDiscover();
    void sendSelectedColor();
};

