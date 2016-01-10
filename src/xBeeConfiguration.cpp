//
//  xBeeConfiguration.cpp
//
//  $Date$
//  $Rev$
//  $Author$
//
//  Please see xBeeControl.h for documentation
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

#include "xBeeConfiguration.h"

//! Create the control object
xBeeConfiguration::xBeeConfiguration() {
}

//! Destructor
xBeeConfiguration::~xBeeConfiguration() {
}

//! Create the control object
void xBeeConfiguration::setRadio(XBee* anXbeeRadio) {
    xbeeRadio = anXbeeRadio;
}

//! Are we connected to the radio?
bool xBeeConfiguration::connected() {
    xbeeRadio->readPacket(500);
    uint8_t atCommand[]={'V','R'};
    AtCommandRequest atRequest = AtCommandRequest(atCommand, NULL, 0);
    outputMessage("connected", true);
    return sendAndGetAck(atRequest);
}

//! Current radio PAN identifier
uint32_t xBeeConfiguration::panID() {
    uint8_t atCommand[]={'I','D'};
    uint32_t currentValue = 0;

    AtCommandRequest atRequest = AtCommandRequest(atCommand, NULL, 0);
    xbeeRadio->send(atRequest);
    
    xbeeRadio->readPacket(1000);
    if (xbeeRadio->getResponse().isAvailable()) {
        int apiId = xbeeRadio->getResponse().getApiId();
        if (apiId == AT_COMMAND_RESPONSE) {
            AtCommandResponse atResponse = AtCommandResponse();
            xbeeRadio->getResponse().getAtCommandResponse(atResponse);
            outputMessage ("PANID is 0x ", false);
            if (atResponse.isOk()) {
                for (int i = 0; i < (int) atResponse.getValueLength(); i++) {
                    outputInteger((int) atResponse.getValue()[i], false);
                }
                outputMessage(NULL, true);
                if (atResponse.getValueLength() == 8) {
                    memcpy (&currentValue, atResponse.getValue() + 4, sizeof(uint32_t));
                }
            } 
            else {
                outputMessage("panID request returned error code: ", false);
                outputInteger((int) atResponse.getStatus(), true);
            }
        }
    } else {        
        outputMessage("panID failed", true);
    }
    return currentValue;
}

//! Current BAUD rate
uint32_t xBeeConfiguration::baudRate() {
    uint8_t atCommand[]={'B','D'};
    uint8_t currentValue = 0;
    
    AtCommandRequest atRequest = AtCommandRequest(atCommand, NULL, 0);
    xbeeRadio->send(atRequest);
    
    xbeeRadio->readPacket(1000);
    if (xbeeRadio->getResponse().isAvailable()) {
        int apiId = xbeeRadio->getResponse().getApiId();
        if (apiId == AT_COMMAND_RESPONSE) {
            AtCommandResponse atResponse = AtCommandResponse();
            xbeeRadio->getResponse().getAtCommandResponse(atResponse);
            if (atResponse.isOk()) {
                outputMessage("BAUD rate is - 0x ", false);
                currentValue = atResponse.getValue()[atResponse.getValueLength() - 1];
                if (currentValue <= numberOfBaudRates) {
                    currentValue = baudRates[currentValue];
                    outputInteger (currentValue, true);
                }
                outputMessage(NULL, true);
            } 
            else {
                outputMessage("baudRate request returned error code: ", false);
                outputInteger((int) atResponse.getStatus(), true);
            }
        }
    } else {
        outputMessage("baudRate failed", true);
    }
    return currentValue;
}

//! Persist the current configuration
bool xBeeConfiguration::persistConfiguration() {    
    uint8_t atCommand[]={'W','R'};
    AtCommandRequest atRequest = AtCommandRequest(atCommand, NULL, 0);
    outputMessage("persistConfiguration", true);
    return sendAndGetAck(atRequest);
}

//! Set the PANID
bool xBeeConfiguration::setPanID(uint32_t newPanID) {    
    uint8_t atCommand[]={'I','D'};
    AtCommandRequest atRequest = AtCommandRequest(atCommand, (uint8_t *)&newPanID, sizeof(uint32_t));
    outputMessage("setPanID", true);
    return sendAndGetAck(atRequest);
}

//! Set the BaudRate
bool xBeeConfiguration::setBaudRate(uint32_t desiredRate) {    
    uint8_t atCommand[]={'B','D'};
    uint8_t index = indexForBaudRate(desiredRate);
    AtCommandRequest atRequest = AtCommandRequest(atCommand, &index, sizeof(uint8_t));
    outputMessage("setBaudRate", true);
    return sendAndGetAck(atRequest);
}

//! Send `atRequest' and return status of request
bool xBeeConfiguration::sendAndGetAck(AtCommandRequest atRequest) {
    xbeeRadio->send(atRequest);
    xbeeRadio->readPacket(c_sendAndGetAckWait);
    if (xbeeRadio->getResponse().isAvailable()) {
        int apiId = xbeeRadio->getResponse().getApiId();
        if (apiId == AT_COMMAND_RESPONSE) {
            AtCommandResponse atResponse = AtCommandResponse();
            xbeeRadio->getResponse().getAtCommandResponse(atResponse);
            if (atResponse.isOk()) {
                if (atResponse.getValueLength() > 0) {
                    outputMessage("\tok - 0x ", false);
                    for (int i = 0; i < (int) atResponse.getValueLength(); i++) {
                        outputInteger((int) atResponse.getValue()[i], false);
                    }
                    outputMessage(NULL, true);
                } else {
                    outputMessage("\tok", true);
                }
                return true;
            } else {
                outputMessage("\trequest returned error code: ", false);
                outputInteger((int) atResponse.getStatus(), true);
            }
        } else {
            outputMessage("\trequest returned error wrong apiId: ", false);
            outputInteger(apiId, true);
        }
    } else {
        outputMessage("\trequest failed", true);
    }
    return false;
}

//! XBee index for desired baud rate
uint8_t xBeeConfiguration::indexForBaudRate(uint32_t desiredRate) {
    uint8_t indexFound = -1;
    for (uint8_t i = 0; i < numberOfBaudRates; i++) {
        if (baudRates[i] == desiredRate) {
            indexFound = i;
        }
    }
    return indexFound;
}

//! Print `newMessage'
void xBeeConfiguration::outputMessage(const char * newMessage, bool withEndl) {
    // Do nothing
}

//! Print `newInt'
void xBeeConfiguration::outputInteger(int newInt, bool withEndl) {
    // Do nothing
}
