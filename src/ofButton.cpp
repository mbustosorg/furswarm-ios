//
//  ofButton.cpp
//
//  $Date$
//  $Rev$
//  $Author$
//
//  Please see ofButton.h for documentation
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
#include "ofButton.h"

//! Initialize the button object
ofButton::ofButton(int buttonX, int buttonY, int buttonHeight, int buttonWidth, int buttonColor, const char *buttonLabel) {
    x = buttonX;
    y = buttonY;
    height = buttonHeight;
    width = buttonWidth;
    color = buttonColor;
	font.load("DIN.otf", 8);
    setLabel(buttonLabel);
    labelWidth = font.stringWidth(label);
    labelHeight = font.stringHeight(label);
    pressed = false;
}

//! Set the current label
void ofButton::setLabel(const char *newLabel) {
    if (strcmp(label, newLabel) != 0) {
        strcpy(label, newLabel);    
        labelWidth = font.stringWidth(label);
        labelHeight = font.stringHeight(label);        
    }
}

//! Set the font size
void ofButton::setFontSize(int fontSize) {
	font.load("DIN.otf", fontSize);
    labelWidth = font.stringWidth(label);
    labelHeight = font.stringHeight(label);
}

//! Draw the button
void ofButton::draw() {
    if (pressed) {
        ofSetHexColor(color * PRESS_COLOR_FACTOR);
    } else {
        ofSetHexColor(color);
    }
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofDrawRectangle(x, y, width, height);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetHexColor(0x000000);
    font.drawString(label, x - labelWidth / 2, buttonY() - labelHeight / 2);
}

//! Is the point (pointX, pointY) on the button? 
bool ofButton::pointValid(int pointX, int pointY) {
    return pointX >= x - width / 2 && pointX <= x + width / 2 && pointY >= y - height / 2 && pointY <= y + height / 2;
}

//! Set the button color 
void ofButton::setColor(int newColor) {
    color = newColor;
}

//! Add `deltaY' to `y'
void ofButton::changeY(int deltaY) {
    y += deltaY;
}

//! The y value for the button
int ofButton::buttonY() {
    return y + (height / 2);
}

//! Button has been pressed
void ofButton::press() {
    pressed = true;
}

//! Button has been released
void ofButton::release() {
    pressed = false;
}

