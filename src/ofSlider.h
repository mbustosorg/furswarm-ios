//
//  ofSlider.h
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

#ifndef furSwarm_ofSlider_h
#define furSwarm_ofSlider_h

#include "furSwarmConst.h"
#include "ofGraphics.h"
#include "ofTrueTypeFont.h"

const int BUTTON_BG_SIZE = 38; // Size for the button background ellipse
const int BUTTON_FG_SIZE = 32; // Size for the button foreground ellipse
const float BUTTON_ELLIPSE_FACTOR = 0.75; // Factor to use for button ellipse

//! Description: Slider widget using openFrameworks
class ofSlider {
    
public: 
    ofSlider(int sliderX, int sliderY, int sliderHeight, int sliderWidth, int sliderColor);
    void draw();
    void setValue(int value);
    void resetLastValue();
    void setPosition(int x);
    bool pointValidForSlider(int pointX, int pointY);
    void changeY(int deltaY);
    uint8_t value(), lastValue;
    int x, y, height, width, color;
    
private:
    int buttonX;
    ofTrueTypeFont font;
    int buttonY();
    
};

#endif
