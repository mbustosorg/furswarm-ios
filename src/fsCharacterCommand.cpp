//
//  fsCharacterCommand.cpp
//
//  $Date$
//  $Rev$
//  $Author$
//
//  Please see fsCharacterCommand.h for documentation
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
#include "fsCharacterCommand.h"
#include "furSwarmPatternConst.h"

//! Initialize the command object
fsCharacterCommand::fsCharacterCommand(int shape, int buttonX, int buttonY, int buttonHeight, int buttonWidth, int buttonColor, 
                                       const char *buttonLabel, uint8_t identifier, fsPatternParameters *parameters) : furSwarmCommand(shape, buttonX, buttonY, buttonHeight, buttonWidth, buttonColor, buttonLabel, identifier, parameters){
}

//! Message payload for this command
fsPayload fsCharacterCommand::payloadFor(uint8_t character) {
    int payloadLength = 6;
    uint8_t *commandPayload = new uint8_t[payloadLength];    
    commandPayload[0] = FS_ID_CHARACTER;
    commandPayload[1] = patternParameters->intensity->value();
    commandPayload[2] = patternParameters->red->value();
    commandPayload[3] = patternParameters->green->value();
    commandPayload[4] = patternParameters->blue->value();
    commandPayload[5] = character;
    if (lastPayload.payload) {
//        if (lastPayload.samePayload(commandPayload)) {
//            commandPayload[1] = 0; // Intensity
//            commandPayload[2] = 0; // Red
//            commandPayload[3] = 0; // Green
//            commandPayload[4] = 0; // Blue           
//            commandPayload[5] = 0; // Character
//            return fsPayload(commandPayload, payloadLength);
//        } else {
            lastPayload = fsPayload(commandPayload, payloadLength);
            return lastPayload;
//        }
    } else {
        lastPayload = fsPayload(commandPayload, payloadLength);
        return lastPayload;
    }
}

