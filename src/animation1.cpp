//
//  animation1.cpp
//
//  $Date$
//  $Rev$
//  $Author$
//
//  Please see animation1.h for documentation
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
#include "animation1.h"
#include "ofUtils.h"

//! Create the animation object
animation1::animation1() {
    animationStepSetting* step;
    for (int i = 0; i < 25; ++i) {
        step = new animationStepSetting(FS_ID_CYLON, i * 20, 200, 100 - i * 10, 100, i * 10, 400);
        animationSteps.push_back(step);
    }
    step = new animationStepSetting(FS_ID_CYLON, 250, 200, 0, 0, 255, 3000);
    animationSteps.push_back(step);
    for (int i = 0; i < 25; ++i) {
        if (i <= 10) {
            step = new animationStepSetting(FS_ID_CYLON_PONG, 100, 0, 100 - i * 10, 100, i * 10, 400);
        } else {
            step = new animationStepSetting(FS_ID_CYLON_PONG, 100, 0, 0, 100, i * 10, 400);            
        }
        animationSteps.push_back(step);
    }
    step = new animationStepSetting(FS_ID_PRISM, 100, 0, 100, 0, 100, 1500);
    animationSteps.push_back(step);
    
    animationId = 1;
}
