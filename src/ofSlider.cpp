//
//  ofSlider.cpp
//
//  $Date$
//  $Rev$
//  $Author$
//
//  Please see ofSlider.h for documentation
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
#include "ofSlider.h"

//! Initialize the slider object
ofSlider::ofSlider(int sliderX, int sliderY, int sliderHeight, int sliderWidth, int sliderColor) {
    x = sliderX;
    y = sliderY;
    height = sliderHeight;
    width = sliderWidth;
    color = sliderColor;
    buttonX = x;
	font.load("DIN.otf", 12);
}

//! Draw the slider
void ofSlider::draw() {
    ofSetHexColor(color);
    ofDrawRectangle(x, y, width, height);
    setPosition(buttonX);
}

//! Currently selected slider value
uint8_t ofSlider::value() {
    float offset = (buttonX - x);
    float denom = width;
    return (uint8_t) offset / denom * 255;
}

//! Set the value of the slider
void ofSlider::setValue(int value) {
    if (value >= 0 && value <= 255) {
        setPosition(x + (int) ((double) value / 255.0 * width));
    }
}

//! Reset `lastValue' to value to help with throttling
void ofSlider::resetLastValue() {
    lastValue = value();
}

//! Add `deltaY' to `y'
void ofSlider::changeY(int deltaY) {
    y += deltaY;
}

//! Set the position to `pos'
void ofSlider::setPosition(int pos) {
	char tempStr[1024];
    int newPos = pos;
    
    if (pos < x) {
        newPos = x;
    } else if (pos > x + width) {
        newPos = x + width;
    }
    buttonX = newPos;
    ofSetHexColor(0x000000);
    ofDrawEllipse(buttonX, buttonY(), BUTTON_BG_SIZE, BUTTON_BG_SIZE * BUTTON_ELLIPSE_FACTOR);
    if (value() == 0 || value() == 255) {
        ofSetHexColor(0x707070);            
    } else {
        ofSetHexColor(0xFFFFFF);
    }
    ofDrawEllipse(buttonX, buttonY(), BUTTON_FG_SIZE, BUTTON_FG_SIZE * BUTTON_ELLIPSE_FACTOR);
    sprintf(tempStr, "%i", value());
    ofSetHexColor(0x808080);
    int stringWidth = font.stringWidth(tempStr);
    font.drawString(tempStr, x + width + 40 - stringWidth, buttonY() + height / 2);
}

//! Is the point (pointX, pointY) on the slider? 
bool ofSlider::pointValidForSlider(int pointX, int pointY) {
    int buttonRadius = BUTTON_BG_SIZE / 2;
    return pointX >= x - buttonRadius && pointX <= x + width + buttonRadius && pointY >= y - buttonRadius && pointY <= y + buttonRadius;
}

//! The y value for the slider button
int ofSlider::buttonY() {
    return y + (height / 2);
}

