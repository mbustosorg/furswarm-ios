//
//  fsPayload.cpp
//
//  $Date$
//  $Rev$
//  $Author$
//
//  Please see fsPayload.h for documentation
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
#include "fsPayload.h"

//! Initialize the payload object
fsPayload::fsPayload(uint8_t *newPayload, int newPayloadLength) {
    payload = newPayload;
    payloadLength = newPayloadLength;
    timestamp = ofGetElapsedTimeMillis();
}

//! Initialize to empty
fsPayload::fsPayload() {
    payload = NULL;
    payloadLength = 0;
}

fsPayload::~fsPayload() {
}

//! Clear the payload
void fsPayload::clear() {
    payload = NULL;
    payloadLength = 0;
}

//! Update `timestamp' with current time
void fsPayload::updateTimestamp() {
    timestamp = ofGetElapsedTimeMillis();
}

//! Is `newPayload' the same as current `payload'?
bool fsPayload::samePayload(uint8_t *newPayload) {
    bool result = payload[1] == newPayload[1] && payload[2] == newPayload[2] && payload[3] == newPayload[3] && 
    payload[4] == newPayload[4] && payload[5] == newPayload[5] && payload[6] == newPayload[6];
    result = result && ((ofGetElapsedTimeMillis() - timestamp) > timeThresholdForSame);
    return result;
}
