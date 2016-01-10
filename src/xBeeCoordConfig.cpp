//
//  xBeeCoordConfig.cpp
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

#include "xBeeCoordConfig.h"
#include "Poco/DateTime.h"

using Poco::DateTime;

//! Create the control object
xBeeCoordConfig::xBeeCoordConfig() {
}

//! Destructor
xBeeCoordConfig::~xBeeCoordConfig() {
}

void xBeeCoordConfig::outputMessage(const char * newMessage, bool withEndl) {
    if (FS_DEBUG_ACTIVITY) {
        if (newMessage) {
            cout << logTimestamp() << newMessage;
        }
        if (withEndl) cout << endl;
    }
}  

void xBeeCoordConfig::outputInteger(int newInt, bool withEndl) {
    if (FS_DEBUG_ACTIVITY) {
        if (newInt < 16) cout << hex << "0" << newInt << " " << dec;
        else cout << hex << newInt << " " << dec;
        if (withEndl) cout << endl;
    }
}

//! Timestamp for the log
string xBeeCoordConfig::logTimestamp() {
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
