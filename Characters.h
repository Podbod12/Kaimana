//  Characters.h
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
//  Kaimana characters for combo animations. First released by ParadiseArcadeShop.com November, 2023
//
//  Created:  November, 2023    Paul 'pod' Denning
//  Revised:  Mar     07, 2024    Paul 'pod' Denning -- Added static colour option for idle mode. Added fixed colour option for pressed mode. Added hold Idle colour instead of instant black for non-pressed. Can be tailored to be different for each character.
//  Revised:  Mar     22, 2024    Paul 'pod' Denning -- Added fade outs for button presses
//

#ifndef __Characters_h__
#define __Characters_h__

#define __PROG_TYPES_COMPAT__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "kaimana_custom.h"
#include "animations.h"
#include "kaimana.h"

extern Kaimana kaimana;

#define NUM_CHARACTERS 8

//define the base character class for combos
class Character
{
  private:

  public:
    virtual bool testForCharacterCombos( ) const { return false; };

    RGB_t getRGB(int r, int g, int b) const
    {
      RGB_t returnRGB;
      returnRGB.r = r;
      returnRGB.g = g;
      returnRGB.b = b;
      return returnRGB;
    }

    virtual EIdleType getIdleAnimationType() const { return EIT_RainbowCircling; } //Default implementation, ranbow rotation through all leds.
    virtual RGB_t getIdleAnimationStaticColour(int ledIndex) const { return getRGB(BLACK); }; //ignored unless idle anim type is StaticColour, override this is each character class if required. if you dont override it will use this BLACK setting
    virtual RGB_t getIdleAnimationPulseColour(int ledIndex) const { return getRGB(WHITE); }; //ignored unless idle anim type is one of the pulse types, override this is each character class if required. if you dont override it will use this WHITE setting

    virtual int holdPressedButtonColourTimeInMS( ) const { return 0; };  //Make sure this plus...
    virtual int fadePressedButtonColourTimeInMS( ) const { return 0; };  //...this is less than the time to restart the idle if its not disabled in getIdleAnimationType() with EIT_Disabled or it'll get stomped ny it (IDLE_TIMEOUT_SECONDS * 1000)

    virtual RGB_t notPressedStaticColour(int ledIndex) const { return getRGB(BLACK); }; //default implementation turns buttons off when out of the idle animation
    virtual RGB_t pressedStaticColour(int ledIndex) const //default implementation changes the pressed button to a random colour
    {
      int randomVal = random(0,NUM_RANDOM_COLORS);
      return randomColors[randomVal];
    };
};

//  Characters are normally made with the character editor (editor/character-editor.html) and stored in characters/, one file each.
//  The editor writes GeneratedCharacters.cpp which turns them into DataCharacter profiles (see CharacterData.h), and
//  AllCharacters is defined there too.
//
//  If you want a character the editor cant describe (eg custom C++ logic in its special move tests) you can write one by hand.
//  Hand written characters live in src/<Game>/, one .h declaring the class and one .cpp with its testForCharacterCombos().
//  They must be under src/ as the Arduino IDE only compiles the sketch folder and src/. CharacterArchive/ has the original hand
//  written versions of the bundled characters (eg CharacterArchive/StreetFighter6/Ryu.h and Ryu.cpp) to copy as a starting point.
//  The editor finds every class under src/ (and in this file) that derives from Character and lists it as "C++: Name" for the
//  profile slots. They are only compiled into the firmware if you pick one for a slot, otherwise they cost no memory.

#endif
