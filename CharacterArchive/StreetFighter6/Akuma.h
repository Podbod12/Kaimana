//  Akuma.h
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
//  Hand written Akuma character profile. Its special moves are in Akuma.cpp
//  Archived template, not compiled from here. To use it copy it (and its .cpp) into src/StreetFighter6/, then pick "C++: Akuma" for a slot in the editor.
//
//  Created:  November, 2023    Paul 'pod' Denning
//

#ifndef __Akuma_h__
#define __Akuma_h__

#include "../../animations.h" // must come before Characters.h as they include each other
#include "../../Characters.h"

class Akuma : public Character
{
  private:
 
  public:
    virtual bool testForCharacterCombos( ) const override;

    //These are the settings I like. Comment in and/or if you like.
    //virtual EIdleType getIdleAnimationType() const override { return EIT_Disabled; }
   
    //virtual int holdPressedButtonColourTimeInMS( ) const { return 500; };  //Make sure this plus...
    //virtual int fadePressedButtonColourTimeInMS( ) const { return 500; };  //...this is less than the time to restart the idle if turnNonHeldButtonsOff is true or it'll get stomped (IDLE_TIMEOUT_SECONDS * 1000)

    //virtual RGB_t notPressedStaticColour(int ledIndex) const override { return getRGB(PURPLE); }; //gi
    //virtual RGB_t pressedStaticColour(int ledIndex) const override { return getRGB(RED); }; //hair
};

#endif
