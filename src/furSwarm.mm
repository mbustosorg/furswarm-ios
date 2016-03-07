//
//  furSwarm.mm
//
//  $Date$
//  $Rev$
//  $Author$
//
//  Please see furSwarm.h for documentation
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

#include "furSwarm.h"
#include "furSwarmMessage.h"
#include "furSwarmConst.h"
#include "furSwarmPatternConst.h"
#include "Poco/DateTime.h"
#include "TargetConditionals.h"
#include "ofTriangleButton.h"
#include <AudioToolbox/AudioToolbox.h>

using Poco::DateTime;

furSwarm::furSwarm() {
    deltaYSlide = 0;
    lastMemberCount = 0;
    currentCommand = NULL;
    sliding = false;
    currentAnimation = NULL;
    fpsTimestampFrame = ofGetFrameNum();
    fpsTimestamp = ofGetElapsedTimeMillis();
    if (!FS_DEBUG_ACTIVITY) {
        ofSetLogLevel(OF_LOG_SILENT);
    }
	coreLocation = new ofxiPhoneCoreLocation();
	hasGPS = coreLocation->startLocation();
    hasGPS = false;
}

furSwarm::~furSwarm() {
    // Delete active members
    // Delete sliders
}

//! Setup the serial connection and some of the basic graphics settings
void furSwarm::setup(){	 

    //  Graphics initializations
    ofEnableAlphaBlending();
	font.load("DIN.otf", 12);
	smallFont.load("DIN.otf", 8);
	fontSliderLabel.load("DIN.otf", 16);
	missingXbeeLabel.load("DIN.otf", 32);

    //  3GS resolution is 320 x 480
    //  Build up the RGB and intensity sliders
    auxSlider = new ofSlider(95, 380, FS_SLIDER_HEIGHT, FS_SLIDER_WIDTH, 0x808080);
    activeSliders.push_front(auxSlider);
    intensitySlider = new ofSlider(95, 240, FS_SLIDER_HEIGHT, FS_SLIDER_WIDTH, 0xFFFFFF);
    intensitySlider->setValue(128);
    activeSliders.push_front(intensitySlider);
    blueSlider = new ofSlider(95, 275, FS_SLIDER_HEIGHT, FS_SLIDER_WIDTH, 0x0000FF);
    activeSliders.push_front(blueSlider);
    greenSlider = new ofSlider(95, 310, FS_SLIDER_HEIGHT, FS_SLIDER_WIDTH, 0x00FF00);
    activeSliders.push_front(greenSlider);
    redSlider = new ofSlider(95, 345, FS_SLIDER_HEIGHT, FS_SLIDER_WIDTH, 0xFF0000);
    activeSliders.push_front(redSlider);
    messageDelaySlider = new ofSlider(95, 410, FS_SLIDER_HEIGHT, FS_SLIDER_WIDTH, 0xFFFFFF);
    messageDelaySlider->setValue(0);
    secondModSlider = new ofSlider(95, 445, FS_SLIDER_HEIGHT, FS_SLIDER_WIDTH, 0xFFFFFF);
    secondModSlider->setValue(0);
    
    // Build gradient arrow
    gradient.moveTo(-100, -5);
    gradient.lineTo(100, -5);
    gradient.lineTo(100, -10);
    gradient.lineTo(130, 0);
    gradient.lineTo(100, 10);
    gradient.lineTo(100, 5);
    gradient.lineTo(-100, 5);
    gradient.lineTo(-100, -5);
    gradient.setFillHexColor(0x008080);
    
    furSwarmCommand* newCommand;
    patternParameters = new fsPatternParameters(intensitySlider, redSlider, greenSlider, blueSlider, auxSlider);
    //  Build up the reporting area
    heartbeatHistory = ";;";
    newCommand = new furSwarmCommand(FS_ELLIPSE_BUTTON, MAIN_BUTTON_X, MAIN_BUTTON_Y, 
                                     MAIN_BUTTON_RADIUS * 2, MAIN_BUTTON_RADIUS * 2, FS_COLOR_FLASH, FS_NAME_FLASH, FS_ID_FLASH, patternParameters);
    page1Commands.push_front(newCommand);
    // Context switching buttons
    memberDetailButton = new ofTriangleButton(320, 0, 40, 40, 0x00FF00, "", 2);
    nextPageButton = new ofTriangleButton(0, 0, 40, 40, 0x00FF00, "", 1);
    
    // Page 1 commands
    // Row 1
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_1, FS_ROW_1, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_HEART, FS_NAME_HEART, FS_ID_HEART, patternParameters);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_2, FS_ROW_1, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_SPIRAL, FS_NAME_SPIRAL, FS_ID_SPIRAL, patternParameters);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);    
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_3, FS_ROW_1, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_SOUND_ACTIVATE, FS_NAME_SOUND_ACTIVATE, FS_ID_SOUND_ACTIVATE,patternParameters);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);
    newCommand = new fsCharacterCommand(FS_RECT_BUTTON, FS_COL_4, FS_ROW_1, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_WORD, FS_NAME_WORD, FS_ID_WORD, patternParameters);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);
    // Row 2
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_1, FS_ROW_2, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_DROP, FS_NAME_DROP, FS_ID_DROP, patternParameters);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_2, FS_ROW_2, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_MATRIX, FS_NAME_MATRIX, FS_ID_MATRIX, patternParameters);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_3, FS_ROW_2, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_RAINBOW_CHASE, FS_NAME_RAINBOW_CHASE, FS_ID_RAINBOW_CHASE, patternParameters);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_4, FS_ROW_2, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_BREATHE_EVOLVE, FS_NAME_BREATHE_EVOLVE, FS_ID_BREATHE_EVOLVE, patternParameters);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);    
    // Row 3
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_1, FS_ROW_3, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_PRISM, FS_NAME_PRISM, FS_ID_PRISM, patternParameters);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);
    newCommand = new fsCharacterCommand(FS_RECT_BUTTON, FS_COL_2, FS_ROW_3, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_ORGANIC, FS_NAME_ORGANIC, FS_ID_ORGANIC, patternParameters);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);
    newCommand = new fsCharacterCommand(FS_RECT_BUTTON, FS_COL_3, FS_ROW_3, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_GRASS_WAVE, FS_NAME_GRASS_WAVE, FS_ID_GRASS_WAVE, patternParameters);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);
    newCommand = new fsCharacterCommand(FS_RECT_BUTTON, FS_COL_4, FS_ROW_3, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_CHARACTER, FS_NAME_CHARACTER, FS_ID_CHARACTER, patternParameters);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);
    // Row 4
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_1, FS_ROW_4, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_PRISM_DISTANCE, FS_NAME_PRISM_DISTANCE, FS_ID_PRISM_DISTANCE, patternParameters);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_2, FS_ROW_4, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_TILT, FS_NAME_TILT, FS_ID_TILT, patternParameters);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_3, FS_ROW_4, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_SPARKLER, FS_NAME_SPARKLER, FS_ID_SPARKLER, patternParameters);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_4, FS_ROW_4, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_FOREST_RUN, FS_NAME_FOREST_RUN, FS_ID_FOREST_RUN, patternParameters);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);
    // Row 5
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_1, FS_ROW_5, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_STARFIELD, FS_NAME_STARFIELD, FS_ID_STARFIELD, patternParameters);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);    
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_2, FS_ROW_5, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_RADIO_TOWER, FS_NAME_RADIO_TOWER, FS_ID_RADIO_TOWER, patternParameters);
    newCommand->setCanSetIntensity(true);   
    page1Commands.push_front(newCommand);
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_3, FS_ROW_5, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_SPECTRUM_ANALYZER, FS_NAME_SPECTRUM_ANALYZER, FS_ID_SPECTRUM_ANALYZER, patternParameters);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_4, FS_ROW_5, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_BREATHE, FS_NAME_BREATHE, FS_ID_BREATHE, patternParameters);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);
    // Row 6
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_1, FS_ROW_6, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_SPARKLE, FS_NAME_SPARKLE, FS_ID_SPARKLE, patternParameters);
    page1Commands.push_front(newCommand);
    // Row 7
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_1, FS_ROW_7, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_DESCEND, FS_NAME_DESCEND, FS_ID_DESCEND, patternParameters);
    page1Commands.push_front(newCommand);
    // Row 8
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_1, FS_ROW_8, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_FULL_COLOR, FS_NAME_FULL_COLOR, FS_ID_FULL_COLOR, patternParameters);
    newCommand->setCanSetColor(true);
    page1Commands.push_front(newCommand);    
    // Row 9
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_1, FS_ROW_9, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_OFF, FS_NAME_OFF, FS_ID_OFF, patternParameters);
    page1Commands.push_front(newCommand);
    // Row 10
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_1, FS_ROW_10, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_BOUNCING_BALL, FS_NAME_BOUNCING_BALL, FS_ID_BOUNCING_BALL, patternParameters);
    newCommand->setCanSetColor(true);
    page1Commands.push_front(newCommand);
    // Row 11
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_1, FS_ROW_11, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_CYLON, FS_NAME_CYLON, FS_ID_CYLON, patternParameters);
    newCommand->setCanSetColor(true);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_2, FS_ROW_11, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_CYLON_PONG, FS_NAME_CYLON_PONG, FS_ID_CYLON_PONG, patternParameters);
    newCommand->setCanSetColor(true);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);    
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_3, FS_ROW_11, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_CYLON_VERTICAL, FS_NAME_CYLON_VERTICAL, FS_ID_CYLON_VERTICAL, patternParameters);
    newCommand->setCanSetColor(true);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);    
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_4, FS_ROW_11, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_RANDOM_FLASH, FS_NAME_RANDOM_FLASH, FS_ID_RANDOM_FLASH, patternParameters);
    newCommand->setCanSetColor(true);
    newCommand->setCanSetIntensity(true);
    page1Commands.push_front(newCommand);

    // Page 2 commands
    // Row 1
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_1, FS_ROW_1, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_SEARCHING_EYE, FS_NAME_SEARCHING_EYE, FS_ID_SEARCHING_EYE, patternParameters);
    newCommand->setCanSetColor(true);
    newCommand->setCanSetIntensity(true);
    page2Commands.push_front(newCommand);
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_2, FS_ROW_1, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_BUBBLE_WAVE, FS_NAME_BUBBLE_WAVE, FS_ID_BUBBLE_WAVE, patternParameters);
    newCommand->setCanSetColor(true);
    newCommand->setCanSetIntensity(true);
    page2Commands.push_front(newCommand);
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_3, FS_ROW_1, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_BROKEN, FS_NAME_BROKEN, FS_ID_BROKEN, patternParameters);
    newCommand->setCanSetIntensity(true);
    page2Commands.push_front(newCommand);
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_4, FS_ROW_1, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_GIANT_SPECTRUM, FS_NAME_GIANT_SPECTRUM, FS_ID_GIANT_SPECTRUM, patternParameters);
    newCommand->setCanSetIntensity(true);
    page2Commands.push_front(newCommand);
    // Row 2
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_1, FS_ROW_2, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_PONG, FS_NAME_PONG, FS_ID_PONG, patternParameters);
    newCommand->setCanSetColor(true);
    newCommand->setCanSetIntensity(true);
    page2Commands.push_front(newCommand);
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_2, FS_ROW_2, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_FLAME, FS_NAME_FLAME, FS_ID_FLAME, patternParameters);
    newCommand->setCanSetColor(true);
    newCommand->setCanSetIntensity(true);
    page2Commands.push_front(newCommand);
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_3, FS_ROW_2, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_CANDLE, FS_NAME_CANDLE, FS_ID_CANDLE, patternParameters);
    newCommand->setCanSetColor(true);
    newCommand->setCanSetIntensity(true);
    page2Commands.push_front(newCommand);
    // Row 3
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_1, FS_ROW_3, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_POOF_1, FS_NAME_POOF_1, FS_ID_POOF_1, patternParameters);
    page2Commands.push_front(newCommand);
    newCommand = new furSwarmCommand(FS_RECT_BUTTON, FS_COL_2, FS_ROW_3, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_POOF_2, FS_NAME_POOF_2, FS_ID_POOF_2, patternParameters);
    page2Commands.push_front(newCommand);
    // Row 7
    resetNetworkButton = new ofButton(FS_COL_1, FS_ROW_7, FS_CB_HEIGHT, FS_CB_WIDTH, FS_COLOR_BROKEN, "network");
    // Row 8
    receiptModeButton = new ofToggleButton(FS_COL_1, FS_ROW_8, FS_CB_HEIGHT, FS_CB_WIDTH, 0xFF0000, "receipt");
    receiptModeButton->press();
    // Row 9
    selectiveModeButton = new ofToggleButton(FS_COL_1, FS_ROW_9, FS_CB_HEIGHT, FS_CB_WIDTH, 0xFF0000, "select");
    // Row 10
    daytimeModeButton = new ofToggleButton(FS_COL_1, FS_ROW_10, FS_CB_HEIGHT, FS_CB_WIDTH, 0xFF0000, "daytime");
    daytimeModeButton->press();

    activeCommands = &page1Commands;

    textKeyboard = new keyboard(true);
    if (XBEE_THREADED) {
        xBee.startThread(true);
    }
}

//! Process any incoming data from the network
void furSwarm::update(){
	if (!XBEE_THREADED) {
        xBee.processIncoming();        
    }
    processResponses();
    animate();
}

//! Animate any currently selected animation
void furSwarm::animate() {
    if (currentAnimation) {
        currentAnimation->animate();
        if (currentAnimation->currentStep) {
            furSwarmCommand *animationCommand = commandByID(currentAnimation->currentStep->identifier);
            intensitySlider->setValue(currentAnimation->currentStep->intensity);
            redSlider->setValue(currentAnimation->currentStep->red);
            greenSlider->setValue(currentAnimation->currentStep->green);
            blueSlider->setValue(currentAnimation->currentStep->blue);
            auxSlider->setValue(currentAnimation->currentStep->aux);
            sendCommand(animationCommand);
            for (list<furSwarmCommand*>::iterator commandIter = activeCommands->begin(); commandIter != activeCommands->end(); ++commandIter)
            {
                if ((*commandIter)->commandIdentifier != currentAnimation->currentStep->identifier) {
                    (*commandIter)->setActivelySelected(false);
                }
            }        
            currentAnimation->setStepProcessed();
        }
    } else if (commandByID(FS_ID_WORD)->activelySelected) {
        if (textKeyboard->needsProcessing()) {
            string stringToBroadcast = textKeyboard->selectedText();
            broadcastString ((fsCharacterCommand *) commandByID(FS_ID_WORD), stringToBroadcast);        
            textKeyboard->setProcessed(true);
        }
    }
}

//! Update the graphical elements using the view objects
/*!
    Primarily this will involve drawing the swarm member details
    and any control elements.
*/
void furSwarm::draw(){
    drawAltControls();
    drawMembers();
    drawControls();
}

ofVec2f gradientVec;
bool gradientLessThan (swarmMemberDisplay* a, swarmMemberDisplay* b) {
    return ofVec2f(a->position[0], a->position[1]).dot(gradientVec) <
    ofVec2f(b->position[0], b->position[1]).dot(gradientVec);
}

//! Draw member representations
void furSwarm::drawMembers() {
    vector<swarmMemberDisplay*> currentCollection;
    SwarmCollection::iterator end = activeMembers.end();
    for (SwarmCollection::iterator it = activeMembers.begin(); it != end; ++it) {
        swarmMemberDisplay *currentDisplay = &(*it).second;
        currentCollection.push_back(currentDisplay);
    }
    
    std::sort (currentCollection.begin(), currentCollection.end(), gradientLessThan);
    for (int i = 0; i < currentCollection.size(); i++) {
        swarmMemberDisplay *currentDisplay = currentCollection[i];
        currentDisplay->display();
        ofSetHexColor(0xFFFFFF);
        stringstream ssIndex;
        ssIndex << i;
        smallFont.drawString(ssIndex.str().c_str(), currentDisplay->position[0] - 3, currentDisplay->position[1] + 3);
    }
}

void furSwarm::slideCommands(list<furSwarmCommand*>* commands, int slideStep) {
    list<furSwarmCommand*>::iterator commandIter;
    for (commandIter = commands->begin(); commandIter != commands->end(); ++commandIter)
    {
        (*commandIter)->commandButton->changeY(slideStep);
    }
}

//! Draw control representations (including buttons and sliders)
void furSwarm::drawControls() {
    float tempSlideStep;
    if (sliding && deltaYSlide < MAX_Y_SLIDE) {
        //  Slide controls off screen
        tempSlideStep = MAX(pow((float)(MAX_Y_SLIDE - (float) deltaYSlide) / MAX_Y_SLIDE, (float)0.45) * 16.0, 1.0);
    } else if (!sliding && deltaYSlide > 0) {
        //  Sliding back to default position
        tempSlideStep = MIN(-pow((float)1.0 - (MAX_Y_SLIDE - (float) deltaYSlide) / MAX_Y_SLIDE, (float)0.45) * 16.0, -1);
    } else {
        tempSlideStep = 0;
    }
#if (TARGET_IPHONE_SIMULATOR || !TARGET_OS_IPHONE)
    slideStep = (int) tempSlideStep;
#else
    slideStep = (int) tempSlideStep * 4.0;
#endif
    deltaYSlide += slideStep;
    list<furSwarmCommand*>::iterator commandIter;
    bool soundActivateSelected = false;
    bool bouncingBallSelected = false;
    
    slideCommands(&page1Commands, slideStep);
    slideCommands(&page2Commands, slideStep);
    
    for (commandIter = activeCommands->begin(); commandIter != activeCommands->end(); ++commandIter)
    {
        if ((*commandIter)->activelySelected) {
            if (selectiveModeButton->pressed) {
                ofSetHexColor(0xff0000);
            } else {
                ofSetHexColor(0xd9d9d9);
            }
            int buttonOffset = 10;
            ofDrawRectangle((*commandIter)->commandButton->x - ((*commandIter)->commandButton->width + buttonOffset) / 2,
                            (*commandIter)->commandButton->y - ((*commandIter)->commandButton->height + buttonOffset) / 2,
                            (*commandIter)->commandButton->width + buttonOffset,
                            (*commandIter)->commandButton->height + buttonOffset);
            if ((*commandIter)->commandIdentifier == FS_ID_SOUND_ACTIVATE) {
                soundActivateSelected = true;
            } else if ((*commandIter)->commandIdentifier == FS_ID_SPECTRUM_ANALYZER) {
                    soundActivateSelected = true;
            } else if ((*commandIter)->commandIdentifier == FS_ID_BOUNCING_BALL) {
                bouncingBallSelected = true;
            }
        }
        (*commandIter)->draw();
    }
    list<ofSlider*>::iterator sliderIter;
    for (sliderIter = activeSliders.begin(); sliderIter != activeSliders.end(); ++sliderIter)
    {
        (*sliderIter)->changeY(slideStep);
        (*sliderIter)->draw();
    }
    
    if (soundActivateSelected) {
        ofSetHexColor(0xb0b0b0);
        fontSliderLabel.drawString("threshold", intensitySlider->x, intensitySlider->y);
        fontSliderLabel.drawString("peaky", auxSlider->x, auxSlider->y);
    } else if (bouncingBallSelected) {
        ofSetHexColor(0xb0b0b0);
        fontSliderLabel.drawString("gravity", intensitySlider->x, intensitySlider->y);
        fontSliderLabel.drawString("damping", auxSlider->x, auxSlider->y);
    }
    textKeyboard->changeY(slideStep);
    if (textKeyboard->active) {
        textKeyboard->draw();
    }
    if (!xBee.connected) {
        ofSetHexColor(0xFF0000);
        missingXbeeLabel.drawString("RADIO MISSING", 5, 200);            
    }
}

//! Draw auxilliary controls
void furSwarm::drawAltControls() {
    int frameTimestamp = ofGetElapsedTimeMillis();
    int droppedCount = 0;
    int failedMessageCount = 0;
    SwarmCollection::iterator it = activeMembers.begin();
    while(it != activeMembers.end())
    {
        swarmMemberDisplay *currentDisplay = &(*it).second;
        failedMessageCount += currentDisplay->member->messageFailures;
        if (!currentDisplay->member->stillInNetwork(frameTimestamp)) {
            //activeMembers.erase(it++);
            ++droppedCount;
            ++it;
        } else {
            ++it;
        }
    }
    if (lastMemberCount != (int) activeMembers.size() - droppedCount) {
        AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);        
    }
    lastMemberCount = (int) activeMembers.size() - droppedCount;
    stringstream ssCnt;
    ssCnt << lastMemberCount;
    memberDetailButton->setLabel(ssCnt.str().c_str());
    memberDetailButton->draw();
    nextPageButton->draw();
    if (activeCommands == &page2Commands) {
        resetNetworkButton->draw();
        receiptModeButton->draw();
        selectiveModeButton->draw();
        daytimeModeButton->draw();
    }
    // Message queue count display
    if (!sliding && deltaYSlide == 0) {
        ofSetHexColor(0x606060);
        stringstream ssQueue;
        ssQueue << xBee.broadcastQueueCount();
        font.drawString(ssQueue.str().c_str(), 20, 475);        
    }
    // Send failure count
    if (!sliding && deltaYSlide == 0) {
        stringstream ssFail;
        ofSetHexColor(0x606060);
        ssFail << "[" << failedMessageCount << "/" << xBee.apiFailures << "/" << xBee.timeoutFailures << "/" << xBee.sendFailures << "]/" << xBee.sentCount;
        font.drawString(ssFail.str().c_str(), 55, 475);
    }
    // Display seconds count
    if (!sliding && deltaYSlide == 0) {
        stringstream ssSeconds;
        if (ofGetSeconds() % 5 == 0) {
            ofSetHexColor(0xAFAF00);
        } else {
            ofSetHexColor(0x606060);
        }
        ssSeconds << ofGetSeconds();
        font.drawString(ssSeconds.str().c_str(), 185, 475);
    }
    // Frame rate display
    if (!sliding && deltaYSlide == 0) {
        stringstream ssFPS;
        ofSetHexColor(0x606060);
        ssFPS << (int) ((double) (ofGetFrameNum() - fpsTimestampFrame) / (double) (frameTimestamp - fpsTimestamp) * 1000.0) << " fps";
        if (FS_DEBUG_ACTIVITY) {
            ssFPS << "-DB";
        }
        font.drawString(ssFPS.str().c_str(), 225, 475);    
        if (fpsTimestamp - frameTimestamp > 10000.0) {
            fpsTimestampFrame = ofGetFrameNum();
            fpsTimestamp = frameTimestamp;
        }
    }
    if (sliding && deltaYSlide >= MAX_Y_SLIDE && !commandByID(FS_ID_WORD)->activelySelected) {
        // Serial port display
        ofSetHexColor(0xf0f000);
        if(hasGPS){
            stringstream ssLatLong;
            ssLatLong << "[lat:" << coreLocation->getLatitude() << "][lon:" << coreLocation->getLongitude() << "]";
            smallFont.drawString(ssLatLong.str().c_str(), 20, 445);
        }
        stringstream ssPort;
        ssPort << xBee.portName << " [" << XBEE_BAUD_RATE << "][0x" << hex << XBEE_PAN << "]" << dec;
        smallFont.drawString(ssPort.str().c_str(), 20, 460);
        ofSetHexColor(0x606060);
        stringstream ssFail;
        ssFail << "[failed/api/timeOut/send]/sent";
        smallFont.drawString(ssFail.str().c_str(), 20, 475);
        // Draw auxiliary components
        messageDelaySlider->draw();
        secondModSlider->draw();
        gradient.draw(320 / 2, 480 / 2);
        SwarmCollection::iterator end = activeMembers.end();
        mapGPS();
        for (SwarmCollection::iterator it = activeMembers.begin(); it != end; ++it) {
            swarmMemberDisplay *currentDisplay = &(*it).second;
            if (!currentDisplay->hasLatLong()) {
                currentDisplay->moveToDistance();
            }
        }
    }    
}

//! Update the position mapping for those that have latitude & longitude available
void furSwarm::mapGPS() {
    float minLatitude = 99999.0, maxLatitude = -99999.0, minLongitude = 99999.0, maxLongitude = -99999.0;
    
    if(hasGPS){
        minLatitude = coreLocation->getLatitude();
        maxLatitude = coreLocation->getLatitude();
        minLongitude = coreLocation->getLongitude();
        maxLongitude = coreLocation->getLongitude();
    } else {
        minLatitude = 99999.0;
        maxLatitude = -99999.0;
        minLongitude = 99999.0;
        maxLongitude = -99999.0;
	}

    SwarmCollection::iterator end = activeMembers.end();
    for (SwarmCollection::iterator it = activeMembers.begin(); it != end; ++it) {
        swarmMemberDisplay *currentDisplay = &(*it).second;
        if (currentDisplay->hasLatLong()) {
            minLatitude = min (minLatitude, currentDisplay->member->latitude);
            maxLatitude = max (maxLatitude, currentDisplay->member->latitude);
            minLongitude = min (minLongitude, currentDisplay->member->longitude);
            maxLongitude = max (maxLongitude, currentDisplay->member->longitude);
        }
    }
    float latRange = maxLatitude - minLatitude;
    float lonRange = maxLongitude - minLongitude;
    if (latRange == 0) latRange = 1.0;
    if (lonRange == 0) lonRange = 1.0;
    float aspectRatio = 480.0 / 320.0;
    if (latRange / lonRange > aspectRatio) {
        minLatitude = minLatitude - 0.1 * aspectRatio * latRange;
        maxLatitude = maxLatitude + 0.1 * aspectRatio * latRange;
        minLongitude = minLongitude - 0.1 * 1.0 / aspectRatio * (lonRange + 1.0 / aspectRatio * latRange);
        maxLongitude = maxLongitude + 0.1 * 1.0 / aspectRatio * (lonRange + 1.0 / aspectRatio * latRange);
    } else {
        minLatitude = minLatitude - 0.1 * aspectRatio * (latRange + aspectRatio * lonRange);
        maxLatitude = maxLatitude + 0.1 * aspectRatio * (latRange + aspectRatio * lonRange);
        minLongitude = minLongitude - 0.1 * 1.0 / aspectRatio * lonRange;
        maxLongitude = maxLongitude + 0.1 * 1.0 / aspectRatio * lonRange;
    }
    latRange = maxLatitude - minLatitude;
    lonRange = maxLongitude - minLongitude;

    int minDistance = 99999999;
    for (SwarmCollection::iterator it = activeMembers.begin(); it != end; ++it) {
        swarmMemberDisplay *currentDisplay = &(*it).second;
        if (currentDisplay->hasLatLong()) {
            float x = 320.0 * (float)(currentDisplay->member->longitude - minLongitude) / lonRange / 1.5;
            float y = 480.0 * (float)(currentDisplay->member->latitude - minLatitude) / latRange / 1.5;
            currentDisplay->moveTo(x, y);
            if (hasGPS) {
                minDistance = min(minDistance, feetToTarget(currentDisplay->member));
            }
        }
    }
    if (hasGPS) {
        ofSetColor(255, 255, 255);
        float x = 320.0 * (float)(coreLocation->getLongitude() - minLongitude) / lonRange / 1.5;
        float y = 480.0 * (float)(coreLocation->getLatitude() - minLatitude) / latRange / 1.5;
        ofDrawCircle(x, y, 5);
        stringstream ssDist;
        ssDist << minDistance << " ft";
        smallFont.drawString(ssDist.str().c_str(), x + 10, y);
    }
}

//! Distance from current position to a member in feet
int furSwarm::feetToTarget(swarmMember* member) {
    float R = 6371; // km
    float feet = 3280.8;
    float dLat = ((float)coreLocation->getLatitude() - member->latitude) * PI / 180.0;
    float dLon = ((float)coreLocation->getLongitude() - member->longitude) * PI / 180.0;
    float lat1 = member->latitude * PI / 180.0;
    float lat2 = (float)coreLocation->getLatitude() * PI / 180.0;
    
    float a = sin(dLat/2) * sin(dLat/2) +
        sin(dLon/2) * sin(dLon/2) * cos(lat1) * cos(lat2);
    float c = 2 * atan2(sqrt(a), sqrt(1-a));
    return (int) R * c * feet;
}

//! Process an RXResponse from the XBee
void furSwarm::processResponses() {    
    if (XBEE_THREADED) {
        xBee.lock();
    }
    swarmMember* newMemberToUpdate = NULL;
    list<broadcastMessage*> commandMessagesToSend;

    if (xBee.receiveQueue.size() > 0) {
        for (receiveMessage message = xBee.receiveQueue.front(); xBee.receiveQueue.size() > 0; message = xBee.receiveQueue.front()) 
        {
            if (FS_DEBUG_ACTIVITY) {
                cout << logTimestamp() << "RX Response received" << endl;
            }
            if (true) {
                // Process member heartbeat message
                SwarmCollection::iterator memberIterator;
                memberIterator = activeMembers.find(message.address16);
                swarmMember* currentMember;
                furSwarmMessage* memberHeartbeat = new furSwarmMessage(message.payload, message.payloadLength);
                if (memberIterator == activeMembers.end()) {
                    uint16_t address16 = message.address16;
                    currentMember = new swarmMember(address16, message.address64);
                    currentMember->setVersionId(memberHeartbeat->versionId);
                    swarmMemberDisplay* currentDisplay = new swarmMemberDisplay(currentMember, MAIN_BUTTON_X, MAIN_BUTTON_Y, MEMBER_RADIUS);
                    activeMembers.insert(std::pair<uint16_t, swarmMemberDisplay>(address16, *currentDisplay));
                    if (FS_DEBUG_ACTIVITY) {
                        cout << logTimestamp() << "New swarmMember - " << (unsigned short) address16 << endl;
                    }
                    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
                } else {
                    currentMember = memberIterator->second.member;
                }
                if (currentCommand && !selectiveModeButton->pressed) {
                    if (!currentMember->stillInNetwork(ofGetElapsedTimeMillis())) {
                        newMemberToUpdate = currentMember;
                    } else if (memberHeartbeat->messageTypeId == 1 && memberHeartbeat->currentPattern != currentCommand->commandIdentifier) {
                        if (commandByID(FS_ID_WORD)->activelySelected) {
                            if (memberHeartbeat->currentPattern != FS_ID_CHARACTER) {
                                newMemberToUpdate = currentMember;
                            }
                        } else if (commandByID(FS_ID_PRISM_DISTANCE)->activelySelected) {
                            if (memberHeartbeat->currentPattern != FS_ID_PRISM) {
                                newMemberToUpdate = currentMember;
                            }
                        } else if (ofGetElapsedTimeMillis() > commandDelayThreshold) {
                            newMemberToUpdate = currentMember;
                        }
                    }                    
                }
                int dataLength = message.payloadLength;
                if (dataLength > 0 && memberHeartbeat->messageTypeId == 1 && !selectiveModeButton->pressed) { // Long heartbeat type
                    currentMember->setFrameCount(memberHeartbeat->frameCount);
                    currentMember->setCurrentPattern(memberHeartbeat->currentPattern);
                    currentMember->setBatteryVoltage(memberHeartbeat->batteryVoltage); 
                    currentMember->setMessageFailures(memberHeartbeat->failedMessageCount);
                    currentMember->setMemberType(memberHeartbeat->memberType);
                    currentMember->setLastTimestamp(ofGetElapsedTimeMillis());
                    currentMember->setFrameRate(memberHeartbeat->frameRate);
                    if (memberHeartbeat->latitude && memberHeartbeat->longitude) {
                        currentMember->setLatLong((float)memberHeartbeat->latitude / 100000.0, (float)memberHeartbeat->longitude / 100000.0);
                    }
                    stringstream ss;
                    ss << dec << currentMember->index << ":" << dec << (int) message.rssi << ";" << heartbeatHistory.substr(0, heartbeatHistory.find_last_of(";"));
                    heartbeatHistory = ss.str();
                    // Make sure we have a valid RSSI to update with
                    if (message.rssi != 0) {
                        if (commandByID(FS_ID_PRISM_DISTANCE)->activelySelected) {
                            uint8_t payload[] = {FS_ID_PRISM, 128, currentMember->lastRssiObservationProportion(), 0, 0, auxSlider->value()};
                            broadcastMessage *beMessageToSend = new broadcastMessage (currentMember->address64, currentMember->address16, 
                                                                                      payload, 6, 0, receiptModeButton->pressed);
                            commandMessagesToSend.push_front(beMessageToSend);
                        }
                        currentMember->extendRssi(message.rssi);
                    }
                }
                if (!currentMember->stillInNetwork(ofGetElapsedTimeMillis())) {
                    currentMember->setJoinTime(ofGetElapsedTimeMillis());
                }
                if (FS_DEBUG_ACTIVITY) {
                    cout << "\tVersionId is " << currentMember->versionId << endl;
                    cout << "\tPayload value length is " << dec << (int) message.payloadLength << endl;
                    cout << "\tPayload value:";
                    int dataItem;
                    for (int i = 0; i < dataLength; ++i) {
                        dataItem = (int) message.payload[i];
                        if (dataItem < 16) {
                            cout << " 0x0" << hex << (int) message.payload[i];
                        } else {
                            cout << " 0x" << hex << (int) message.payload[i];
                        }
                    }
                    cout << dec << endl;
                }
                delete memberHeartbeat;
            } else {
                // Process member neighborhood message
            }
            xBee.receiveQueue.pop();
        }
    }
    if (XBEE_THREADED) {
        xBee.unlock();
    }
    if (newMemberToUpdate) {
        fsPayload payload = currentCommand->payload(true);
        broadcastMessage *beMessageToSend = new broadcastMessage (newMemberToUpdate->address64, newMemberToUpdate->address16, 
                                                                  payload.payload, payload.payloadLength, 0, receiptModeButton->pressed);
        xBee.pushBroadcast(beMessageToSend);                        
        if (FS_DEBUG_ACTIVITY) {
            cout << logTimestamp() << " Current Command Sent: " << newMemberToUpdate->address16 << endl;
        }
    }
    if (commandMessagesToSend.size() > 0) {
        for (list<broadcastMessage*>::iterator messagesIter = commandMessagesToSend.begin(); 
             messagesIter != commandMessagesToSend.end(); 
             ++messagesIter)
        {
            xBee.pushBroadcast(*messagesIter);                        
        }        
    }
}

//! Perform a node discovery action and compile responses
void furSwarm::nodeDiscover() {
    fsNodeDiscover currentND = fsNodeDiscover(3, &xBee);
}

//! Event handler for the mousePressed callback
void furSwarm::touchDown(ofTouchEventArgs & touch) {
//void furSwarm::mousePressed(int x, int y, int button){
    //nodeDiscover();
    int x = touch.x;
    int y = touch.y;
    textKeyboard->pressIfValid(x, y);
    if (FS_DEBUG_INTERACTION) {
        cout << logTimestamp() << "Mouse Pressed" << endl;
    }
    if (memberDetailButton->pointValid(x, y)) {
        sliding = not sliding;
        memberDetailButton->press();
        textKeyboard->setActive(false);
    } else if (nextPageButton->pointValid(x, y)) {
        nextPageButton->press();
        if (activeCommands == &page1Commands) {
            activeCommands = &page2Commands;
        } else {
            activeCommands = &page1Commands;
        }
    } else if (activeCommands == &page2Commands) {
        if (selectiveModeButton->pointValid(x, y)) {
            selectiveModeButton->press();
        } else if (resetNetworkButton->pointValid(x, y)) {
            resetNetworkButton->press();
            xBee.resetNetwork();
        } else if (daytimeModeButton->pointValid(x, y)) {
            daytimeModeButton->press();
        } else if (receiptModeButton->pointValid(x, y)) {
            receiptModeButton->press();
        }
    } else if (sliding && !messageDelaySlider->pointValidForSlider(x, y) && !secondModSlider->pointValidForSlider(x, y)) {
        rotateGradient(x, y);
    }
    SwarmCollection::iterator end = activeMembers.end(); 
    int i = 0;
    for (SwarmCollection::iterator it = activeMembers.begin(); it != end; ++it) {
        swarmMemberDisplay *currentDisplay = &(*it).second;
        if (sliding) {
            ++i;
            //currentDisplay->moveTo(30, 30 * i);
            if (!currentDisplay->hasLatLong()) {
                currentDisplay->moveToDistance();
            }
            if (currentDisplay->pointValid(x, y)) {
                currentDisplay->toggle();
            }
        } else {
            currentDisplay->moveToOrbit();
        }
    }    
    bool commandSelected = false;
    for (list<furSwarmCommand*>::iterator commandIter = activeCommands->begin(); 
         commandIter != activeCommands->end(); 
         ++commandIter)
    {
        if ((*commandIter)->pointValid(x, y)) {
            int animationId = 0;
            if (currentAnimation) {
                animationId = currentAnimation->animationId;
                delete currentAnimation;
                currentAnimation = NULL;
            }
            if ((*commandIter)->commandIdentifier == FS_ID_ANIMATE_1) {
                if (animationId == 3 || animationId == 0) {
                    currentAnimation = new animation1();
                } else if (animationId == 1) {
                    currentAnimation = new animation2();                    
                } else if (animationId == 2) {
                    currentAnimation = new animation3(); 
                }
                commandSelected = true;
            } else if ((*commandIter)->commandIdentifier == FS_ID_WORD) {
                textKeyboard->setActive (true);
                sliding = true;
                commandSelected = true;
            } else if ((*commandIter)->commandIdentifier == FS_ID_SOUND_ACTIVATE) {
                intensitySlider->setValue(128);
                auxSlider->setValue(128);
                sendCommand(*commandIter);
                commandSelected = true;                
            } else {
                sendCommand(*commandIter);
                commandSelected = true;
            };
        }
    }
    if (commandSelected) {
        // Deactivate all other commands
        deactivateCommands(&page1Commands, x, y);
        deactivateCommands(&page2Commands, x, y);
    }
    if (FS_DEBUG_ACTIVITY)
    {
        cout << logTimestamp() << "framerate is " << (double) ofGetFrameNum() / (double) ofGetElapsedTimeMillis() * 1000.0 << " fps" << endl;
    }
}

//! Deactivate `commands' if not valid for `x' & `y'
void furSwarm::deactivateCommands (list<furSwarmCommand*>* commands, int x, int y) {
    for (list<furSwarmCommand*>::iterator commandIter = commands->begin(); commandIter != commands->end(); ++commandIter)
    {
        if (commands != activeCommands || !(*commandIter)->pointValid(x, y)) {
            (*commandIter)->setActivelySelected(false);
        }
    }
}

//! Command for `identifier'
furSwarmCommand* furSwarm::commandByID(int identifier) {
    for (list<furSwarmCommand*>::iterator commandIter = page1Commands.begin();
         commandIter != page1Commands.end();
         ++commandIter)
    {
        if ((*commandIter)->commandIdentifier == identifier) {
            return (*commandIter);
        };
    }
    for (list<furSwarmCommand*>::iterator commandIter = page2Commands.begin();
         commandIter != page2Commands.end();
         ++commandIter)
    {
        if ((*commandIter)->commandIdentifier == identifier) {
            return (*commandIter);
        };
    }
    return NULL;
}

//! Send the command, respecting any timing details for the command
void furSwarm::sendCommand(furSwarmCommand *fsCommand) {
    if (fsCommand->commandIdentifier == FS_ID_CHARACTER) {
        if (FS_DEBUG_ACTIVITY) {
            cout << logTimestamp() << fsCommand->label() << " Request" << endl;
        }
        fsPayload payload = ((fsCharacterCommand *) fsCommand)->payloadFor(0);
        broadcastPayload(payload, true);
    } else if (currentAnimation) {
        // Don't slow down animation changes for network delay
        sendCommandRequest(fsCommand, false);
    } else {
        sendCommandRequest(fsCommand, true);
    }
    currentCommand = fsCommand;
    fsCommand->press();
}

//! Broadcast a string for character display to available members, order determined by network entry
void furSwarm::broadcastString(fsCharacterCommand* characterCommand, string stringToBroadcast) {
    broadcastMessage *messageToSend;
    SwarmCollection::iterator end = activeMembers.end();
    int i = 0;
    int characterIndex = 0;
    int commandDelay = 0;
    for (SwarmCollection::iterator it = activeMembers.begin(); it != end; ++it) {
        swarmMemberDisplay *currentDisplay = &(*it).second;
        if (currentDisplay->member->stillInNetwork(ofGetElapsedTimeMillis()) && currentDisplay->member->memberType != FS_TYPE_HAT) {
            if (i < stringToBroadcast.length()) {
                characterIndex = (int) stringToBroadcast[i] - 64;
                if (FS_DEBUG_ACTIVITY) {
                    cout << logTimestamp() << "Sending " << stringToBroadcast[i] << endl;
                }
            } else {
                characterIndex = 255;
                if (FS_DEBUG_ACTIVITY) {
                    cout << logTimestamp() << "Sending -Blank-" << endl;
                }
            }
            fsPayload payload = characterCommand->payloadFor(characterIndex);
            messageToSend = new broadcastMessage (currentDisplay->member->address64, currentDisplay->member->address16, 
                                                  payload.payload, payload.payloadLength, commandDelay, false);
            xBee.pushBroadcast(messageToSend);        
            ++i;
        }
    }    
}

//! Event handler for the pressMoved callback
void furSwarm::touchMoved(ofTouchEventArgs & touch) {
//void furSwarm::pressMoved(int x, int y, int button){
    int i = 0;
    int x = touch.x;
    int y = touch.y;

    int currentLevel;
    bool broadcast = false;
    int newColor = 0;
    
    list<ofSlider*>::iterator sliderIter;
    for(sliderIter = activeSliders.begin(); sliderIter != activeSliders.end(); ++sliderIter)
    {
        if ((*sliderIter)->pointValidForSlider(x, y)) {
            (*sliderIter)->setPosition(x);
            if (abs((*sliderIter)->lastValue - (*sliderIter)->value()) > BROADCAST_THRESHOLD) {
                (*sliderIter)->resetLastValue();
                broadcast = true;
            }
        }
        if (i < 3) {
            currentLevel = (*sliderIter)->value();
            newColor = (newColor << 8) + currentLevel;
            ++i;
        }
    }    
    if (sliding && deltaYSlide >= MAX_Y_SLIDE) {
        if (messageDelaySlider->pointValidForSlider(x, y)) {
            messageDelaySlider->setPosition(x);
        } else if (secondModSlider->pointValidForSlider(x, y)) {
            secondModSlider->setPosition(x);
        } else {
            rotateGradient (x, y);
        }
    }

    textKeyboard->setRotationSpeed(auxSlider->value());
    if (broadcast) {
        // Throttle back color selection to prevent overloading the network
        selectedColor = newColor;
        sendSelectedColor();
    }
    if (FS_DEBUG_INTERACTION) {
        cout << logTimestamp() << "pressMoved" << endl;
    }
}

//! Rotate the gradient arrow based on `x' and `y' screen location
void furSwarm::rotateGradient (int x, int y) {
    float gradientAngle = 0;
    gradientAngle = atan2 ((480.0 / 2 - y), (320.0 / 2 - x)) * RAD_TO_DEG;
    gradient.rotate(-lastGradientAngle, ofVec3f(0, 0, 1));
    gradient.rotate(gradientAngle, ofVec3f(0, 0, 1));
    lastGradientAngle = gradientAngle;
    gradientVec = ofVec2f((320.0 / 2 - x), (480.0 / 2 - y));
}

//! Send `selectedColor' to swarm members
void furSwarm::sendSelectedColor() {
    list<furSwarmCommand*>::iterator commandIter;
    for(commandIter = activeCommands->begin(); commandIter != activeCommands->end(); ++commandIter)
    {
        if ((*commandIter)->canSetColor) {
            (*commandIter)->setColor(selectedColor);
        }
        if ((*commandIter)->activelySelected) {
            if ((*commandIter)->canSetColor) {
                sendCommandRequest(*commandIter, true);
            } else if ((*commandIter)->canSetIntensity) {
                sendCommandRequest(*commandIter, true);
            }
        }
    }
}

//! Event handler for when a button is released
void furSwarm::touchUp(ofTouchEventArgs & touch) {
    list<furSwarmCommand*>::iterator commandIter;
    for(commandIter = activeCommands->begin(); commandIter != activeCommands->end(); ++commandIter)
    {
        (*commandIter)->release();
    }
    memberDetailButton->release();
    nextPageButton->release();
    textKeyboard->release();
    resetNetworkButton->release();
    if (FS_DEBUG_INTERACTION) {
        cout << logTimestamp() << "Press Released" << endl;
    }
}

//! Broadcast command request
void furSwarm::sendCommandRequest(furSwarmCommand* fsCommand, bool requestConfirmation) {
    if (FS_DEBUG_ACTIVITY) {
        cout << logTimestamp() << fsCommand->label() << " Request" << endl;
    }
    fsPayload payload = fsCommand->payload(false);
    // 03:30 UTC == 20:30 PDT
    // 12:30 UTC == 05:30 PDT
//    bool on = ofGetHours() >= 8 && ofGetHours() <= 5;
//    if (daytimeModeButton->pressed || on) {
        broadcastPayload(payload, requestConfirmation);
//    }
}

//! Broadcast a payload to all swarm members
void furSwarm::broadcastPayload(fsPayload payload, bool requestConfirmation) {
    vector<swarmMemberDisplay*> currentCollection;
    SwarmCollection::iterator end = activeMembers.end();
    for (SwarmCollection::iterator it = activeMembers.begin(); it != end; ++it) {
        swarmMemberDisplay *currentDisplay = &(*it).second;
        if (!selectiveModeButton->pressed || currentDisplay->selected) {
            currentCollection.push_back(currentDisplay);
        }
    }
    
    int commandDelay = 0;
    if (currentCollection.size() > 0) {
        commandDelay = messageDelaySlider->value() / currentCollection.size();
        if (commandDelay > 127) commandDelay = 127;
    }
    if (secondModSlider->value() > 0) {
        if (commandDelay > 7) commandDelay = 7;
        commandDelay = commandDelay << 4;
        commandDelay = commandDelay | 0x80;
        int secondMod = secondModSlider->value();
        if (secondMod > 15) secondMod = 15;
        commandDelay = commandDelay | secondMod;
    }
    commandDelayThreshold = ofGetElapsedTimeMillis() + secondModSlider->value() * 1500;
    
    std::sort (currentCollection.begin(), currentCollection.end(), gradientLessThan);
    broadcastMessage *messageToSend;
    for (int i = 0; i < currentCollection.size(); i++) {
        swarmMemberDisplay *currentDisplay = currentCollection[i];
        //if (currentDisplay->member->memberType == FS_TYPE_TOWER || currentDisplay->member->stillInNetwork(ofGetElapsedTimeMillis())) {
            ofSetHexColor(0x606060);
            stringstream ssIndex;
            ssIndex << i;
            smallFont.drawString(ssIndex.str().c_str(), currentDisplay->position[0], currentDisplay->position[1]);

            if (payload.payload[0] == FS_ID_PONG || payload.payload[0] == FS_ID_GIANT_SPECTRUM) {
                payload.payload[2] = i + 1;
                payload.payload[3] = currentCollection.size();
            }
            uint8_t transitionCommand = 0x80 | payload.payload[0];
            memcpy(&payload.payload[0], &transitionCommand, sizeof(uint8_t));
            messageToSend = new broadcastMessage (currentDisplay->member->address64, currentDisplay->member->address16,
                                                  payload.payload, payload.payloadLength, commandDelay,
                                                  requestConfirmation);
            xBee.pushBroadcast(messageToSend);
            if (FS_DEBUG_INTERACTION) {
                cout << logTimestamp() << currentDisplay->member->address64.getLsb() << endl;
            }
        //}
        commandDelay += messageDelaySlider->value() / currentCollection.size();
    }
}

//! Timestamp for the log
string furSwarm::logTimestamp() {
    DateTime now;
    stringstream ss;
    if (now.hour() < 10) {
        ss << "0";
    }
    ss << now.hour() << ":";
    if (now.minute() < 10) {
        ss << "0";
    }
    ss << now.minute() << ":";
    if (now.second() < 10) {
        ss << "0";
    }
    ss << now.second();
    if (now.millisecond() < 10) {
        ss << ".00";        
    } else if (now.millisecond() < 100) {
        ss << ".0";
    } else {
        ss << ".";
    }
    ss << now.millisecond();
    ss << " - ";
    return ss.str();
}
