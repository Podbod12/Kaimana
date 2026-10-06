//  Gief.h
//
//  Copyright 2023 Paradise Arcade Shop, ParadiseArcadeShop.com
//  All rights reserved.  Use is subject to license terms.
//
//  Code is provided for entertainment purposes and use with the Kaimana controller.
//  Code may be copied, modified, resused with this Copyright notice.
//  No commercial use without written permission from Paradise Arcade Shop.
//
//  Paradise Arcade Shop Kaimana LED Driver Board
//  Initial Release October 15, 2013
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//
//  Hand written Gief character profile. Its special moves are in Gief.cpp
//  Archived template, not compiled from here. To use it copy it (and its .cpp) into src/StreetFighter6/, then pick "C++: Gief" for a slot in the editor.
//
//  Created:  November, 2023    Paul 'pod' Denning
//

#ifndef __Gief_h__
#define __Gief_h__

#include "../../animations.h" // must come before Characters.h as they include each other
#include "../../Characters.h"

class Gief : public Character
{
  private:
 
  public:
    virtual bool testForCharacterCombos( ) const override;
    
    //Example for YELLOW idle mode. Out of idle will be RED if not pressed and GREEN if pressed
    //virtual EIdleType getIdleAnimationType() const override { return EIT_StaticColour; }
    //virtual RGB_t getIdleAnimationStaticColour(int ledIndex) const override { return getRGB(YELLOW); }; //Gi colour

    //virtual RGB_t notPressedStaticColour(int ledIndex) const override { return getRGB(RED); }; //Pants colour
    //virtual RGB_t pressedStaticColour(int ledIndex) const override { return getRGB(GREEN); }; //Hand colour :D
};

#endif
