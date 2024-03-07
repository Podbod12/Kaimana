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
    virtual void testForCharacterCombos( ) const {};

    RGB_t getRGB(int r, int g, int b) const
    {
      RGB_t returnRGB;
      returnRGB.r = r;
      returnRGB.g = g;
      returnRGB.b = b;
      return returnRGB;
    }

    virtual bool useStaticColourInIdle( ) const { return false; };
    virtual bool useStaticColourWhenPressed( ) const { return false; };
    virtual bool turnNonHeldButtonsOff( ) const { return true; };  //This is only used if you've set UseStaticColourInIdle to true. other wise it will always turn them off.
   
    virtual RGB_t idleStaticColour( ) const = 0; //used if useStaticColourInIdle is true
    virtual RGB_t pressedStaticColour( ) const = 0; //used if useStaticColourWhenPressed is true
};

//define the actual Characters
//The classic 8
class Ryu : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) const override;

//Example for White at all times but individual button will turn Red when pressed
//    virtual bool useStaticColourInIdle( ) const override { return true; };
//    virtual bool useStaticColourWhenPressed( ) const override { return true; };
//    virtual bool turnNonHeldButtonsOff( ) const override { return false; };  //This is only used if you've set UseStaticColourInIdle to true. other wise it will always turn them off.
   
    virtual RGB_t idleStaticColour( ) const override { return getRGB(WHITE); }; //Gi colour
    virtual RGB_t pressedStaticColour( ) const override { return getRGB(RED); }; //Headband colour
};

class Ken : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) const override;

//Example for Red when idling but individual button will turn Yellow when pressed and other buttons/stick lights will go black
//    virtual bool useStaticColourInIdle( ) const override { return true; };
//    virtual bool useStaticColourWhenPressed( ) const override { return true; };
//    virtual bool turnNonHeldButtonsOff( ) const override { return true; };  //This is only used if you've set UseStaticColourInIdle to true. other wise it will always turn them off.
   
    virtual RGB_t idleStaticColour( ) const override { return getRGB(RED); }; //Gi colour
    virtual RGB_t pressedStaticColour( ) const override { return getRGB(YELLOW); }; //Hair colour :D
};

class Chun : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) const override;
    
    virtual RGB_t idleStaticColour( ) const override { return getRGB(BLUE); }; //Gi colour
    virtual RGB_t pressedStaticColour( ) const override { return getRGB(WHITE); }; //boot colour
};

class Guile : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) const override;
    
    virtual RGB_t idleStaticColour( ) const override { return getRGB(GREEN); }; //Gi colour
    virtual RGB_t pressedStaticColour( ) const override { return getRGB(YELLOW); }; //Hair colour
};

class Gief : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) const override;
    
    virtual RGB_t idleStaticColour( ) const override { return getRGB(RED); }; //Pants colour
    virtual RGB_t pressedStaticColour( ) const override { return getRGB(GREEN); }; //Hand colour :D
};

class Dhalsim : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) const override;
    
    virtual RGB_t idleStaticColour( ) const override { return getRGB(GOLD); }; //Shorts colour
    virtual RGB_t pressedStaticColour( ) const override { return getRGB(WHITE); }; //skulls colour
};

class Honda : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) const override;
    
    virtual RGB_t idleStaticColour( ) const override { return getRGB(BLUE); }; //Gi colour
    virtual RGB_t pressedStaticColour( ) const override { return getRGB(RED); }; //facepaint colour
};

class Blanka : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) const override;
    
    virtual RGB_t idleStaticColour( ) const override { return getRGB(GREEN); }; //Skin colour
    virtual RGB_t pressedStaticColour( ) const override { return getRGB(ORANGE); }; //Shorts/bangle colour
};

//Some other sf6 favs
class DeeJay : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) const override;
    
    virtual RGB_t idleStaticColour( ) const override { return getRGB(ORANGE); }; //Original gi colour
    virtual RGB_t pressedStaticColour( ) const override { return getRGB(GREEN); }; //New gi colour
};

class Marisa : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) const override;
    
    virtual RGB_t idleStaticColour( ) const override { return getRGB(GOLD); }; //Gi trim colour
    virtual RGB_t pressedStaticColour( ) const override { return getRGB(RED); }; //hair colour
};

#endif
