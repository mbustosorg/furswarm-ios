//
//  animationCommand.cpp
//
//  $Date$
//  $Rev$
//  $Author$
//
//  Please see animationCommand.h for documentation
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
#include "animationCommand.h"
#include "ofUtils.h"

//! Create the animation object
animationCommand::animationCommand() {
    startTime = 0;
    currentStep = NULL;
    animationComplete = false;
    animationIndex = 0;
}

//! Destructor
animationCommand::~animationCommand() {
    for (int i = 0; i < animationSteps.size(); ++i) {
        delete animationSteps[i];
    }
}

//! Move forward in the animation
void animationCommand::animate() {
    if (!animationComplete) {
        if (startTime == 0) {
            startTime = ofGetElapsedTimeMillis();
            setStep(animationSteps[animationIndex]);
        } else if (animationIndex + 1 < animationSteps.size()) {
            if ((animationSteps[animationIndex]->stepLength + startTime) < ofGetElapsedTimeMillis()) {
                ++animationIndex;
                setStep(animationSteps[animationIndex]);
            }
        } else {
            if ((animationSteps[animationIndex]->stepLength + startTime) < ofGetElapsedTimeMillis()) {
                animationComplete = true;
            }        
        }
    }
}

//! Setup what step needs to be sent out
void animationCommand::setStep(animationStepSetting* step) {
    currentStep = step;
    startTime = ofGetElapsedTimeMillis();
}

//! Set the fact that the command request has been sent out
void animationCommand::setStepProcessed() {
    currentStep = NULL;
}