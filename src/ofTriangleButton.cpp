//
//  ofTriangleButton.cpp
//
//  $Date$
//  $Rev$
//  $Author$
//
//  Please see ofTriangleButton.h for documentation
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
#include "ofTriangleButton.h"

//! Initialize the button object
ofTriangleButton::ofTriangleButton(int buttonX, int buttonY, int buttonHeight, int buttonWidth, int buttonColor, const char *buttonLabel, int buttonCorner) : ofButton (buttonX, buttonY, buttonHeight, buttonWidth, buttonColor, buttonLabel) {
    corner = buttonCorner;
	setFontSize(12);
}

//! The y value for the button
int ofTriangleButton::buttonY() {
    return y;
}

//! Is the point (pointX, pointY) contained in the ellipse? 
bool ofTriangleButton::pointValid(int pointX, int pointY) {
    if (corner == 1) {
        return pointX >= x && pointX <= x + width && pointY >= y && pointY <= y + height;
    } else if (corner == 2) {
        return pointX >= x - width && pointX <= x && pointY >= y && pointY <= y + height;
    } else if (corner == 3) {
        return pointX >= x - width && pointX <= x && pointY >= y - height && pointY <= y;
    } else if (corner == 4) {
        return pointX >= x && pointX <= x + height && pointY >= y - height && pointY <= y;
    }
    return false;
}

//! Draw the button
void ofTriangleButton::draw() {
    if (pressed) {
        ofSetHexColor(color * PRESS_COLOR_FACTOR);
    } else {
        ofSetHexColor(color);
    }
    if (corner == 1) {
        ofDrawTriangle(x + width, y, x, y, x, y + height);
    } else if (corner == 2) {
        ofDrawTriangle(x - width, y, x, y, x, y + height);
    } else if (corner == 3) {
        ofDrawTriangle(x - width, y, x, y, x, y - height);
    } else if (corner == 4) {
        ofDrawTriangle(x + width, y, x, y, x, y - height);
    }
    ofSetHexColor(0x808080);
    font.drawString(label, x - 15 - labelWidth / 2, y + 17);    
}

