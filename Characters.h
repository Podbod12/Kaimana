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

//This is an example profile thats not actually a real character but shows how to customise individual button colours. In this case, they match the original sf2 machine and turn green when pressed
class SF2 : public Character
{
  private:
 
  public:
    virtual EIdleType getIdleAnimationType() const override { return EIT_Disabled; }
  
    virtual RGB_t notPressedStaticColour(int ledIndex) const override
    {
      if(ledIndex == LED_P1 || ledIndex == LED_K1)
        return getRGB(RED);
       if(ledIndex == LED_P2 || ledIndex == LED_K2)
        return getRGB(WHITE);
      if(ledIndex == LED_P3 || ledIndex == LED_K3)
        return getRGB(BLUE);

      //all other buttons and directions
      return getRGB(YELLOW); 
    }; 
    
    virtual RGB_t pressedStaticColour(int ledIndex) const override { return getRGB(GREEN); };
};

//define the actual Characters
//The classic 8
class Ryu : public Character
{
  private:
 
  public:
    virtual bool testForCharacterCombos( ) const override;

    //Example for grey at all times but individual button will turn Red when pressed and will fade back to white when released
    //virtual EIdleType getIdleAnimationType() const override { return EIT_Disabled; }
  
    //virtual int holdPressedButtonColourTimeInMS( ) const override { return 0; };  //Make sure this plus...
    //virtual int fadePressedButtonColourTimeInMS( ) const override { return 500; };  //...this is less than the time to restart the idle if turnNonHeldButtonsOff is true or it'll get stomped (IDLE_TIMEOUT_SECONDS * 1000)

    //virtual RGB_t notPressedStaticColour(int ledIndex) const override { return getRGB(GREY); }; //Gi colour
    //virtual RGB_t pressedStaticColour(int ledIndex) const override { return getRGB(RED); }; //Headband colour
};

class Ken : public Character
{
  private:
 
  public:
    virtual bool testForCharacterCombos( ) const override;

    //Example for pulsing Red when idling but individual button will turn Yellow when pressed and other buttons/stick lights will go black. Will wait for a small amount then fade back to black when released.
    //virtual EIdleType getIdleAnimationType() const override { return EIT_StaticColourPulsing; }
    //virtual RGB_t getIdleAnimationStaticColour(int ledIndex) const override { return getRGB(RED); }; //Gi colour
   
    //virtual int holdPressedButtonColourTimeInMS( ) const { return 500; };  //Make sure this plus...
    //virtual int fadePressedButtonColourTimeInMS( ) const { return 500; };  //...this is less than the time to restart the idle if turnNonHeldButtonsOff is true or it'll get stomped (IDLE_TIMEOUT_SECONDS * 1000)

    //virtual RGB_t notPressedStaticColour(int ledIndex) const override { return getRGB(BLACK); }; //Off
    //virtual RGB_t pressedStaticColour(int ledIndex) const override { return getRGB(YELLOW); }; //Hair colour :D
};

class Chun : public Character
{
  private:
 
  public:
    virtual bool testForCharacterCombos( ) const override;
    
    //Example for BLUE idle mode with a WHITE pulse when idling that reverses back and forth across all buttons. Out of idle will be BLUE if not pressed and WHITE if pressed
    //virtual EIdleType getIdleAnimationType() const override { return EIT_StaticColourPingPongPulse; }
    //virtual RGB_t getIdleAnimationStaticColour(int ledIndex) const override { return getRGB(BLUE); }; //Gi colour
    //virtual RGB_t getIdleAnimationPulseColour(int ledIndex) const override { return getRGB(WHITE); }; 

    //virtual RGB_t notPressedStaticColour(int ledIndex) const override { return getRGB(BLACK); }; //Gi colour
    //virtual RGB_t pressedStaticColour(int ledIndex) const override { return getRGB(WHITE); }; //boot colour
};

class Guile : public Character
{
  private:
 
  public:
    virtual bool testForCharacterCombos( ) const override;
    
    //Example for RED idle mode with 2 rotating WHITE pulses when idling. Out of idle will be GREEN if not pressed and YELLOW if pressed
    //virtual EIdleType getIdleAnimationType() const override { return EIT_StaticColourCircleDualPulse; }
    //virtual RGB_t getIdleAnimationStaticColour(int ledIndex) const override { return getRGB(RED); }; //Gi colour
    //virtual RGB_t getIdleAnimationPulseColour(int ledIndex) const override { return getRGB(WHITE); };
    
    //virtual RGB_t notPressedStaticColour(int ledIndex) const override { return getRGB(GREEN); }; //Gi colour
    //virtual RGB_t pressedStaticColour(int ledIndex) const override { return getRGB(YELLOW); }; //Hair colour
};

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

class Dhalsim : public Character
{
  private:
 
  public:
    virtual bool testForCharacterCombos( ) const override;
    
    //Example for RainbowCircle idle mode. Out of idle will be WHITE if not pressed and GOLD if pressed
    //virtual RGB_t notPressedStaticColour(int ledIndex) const override { return getRGB(GOLD); }; //Shorts colour
    //virtual RGB_t pressedStaticColour(int ledIndex) const override { return getRGB(WHITE); }; //skulls colour
};

class Honda : public Character
{
  private:
 
  public:
    virtual bool testForCharacterCombos( ) const override;
    
    virtual EIdleType getIdleAnimationType() const override { return EIT_RainbowPulsing; }

    //Example for RainbowCircle idle mode. Out of idle will be RED if not pressed and BLUE if pressed
    //virtual RGB_t notPressedStaticColour(int ledIndex) const override { return getRGB(BLUE); }; //Gi colour
    //virtual RGB_t pressedStaticColour(int ledIndex) const override { return getRGB(RED); }; //facepaint colour
};

class Blanka : public Character
{
  private:
 
  public:
    virtual bool testForCharacterCombos( ) const override;
    
    //Example for GREEN idle mode with 1 rotating WHITE pulse when idling. Out of idle will be ORANGE if not pressed and GREEN if pressed
    //virtual EIdleType getIdleAnimationType() const override { return EIT_StaticColourCirclePulse; }
    //virtual RGB_t getIdleAnimationStaticColour(int ledIndex) const override { return getRGB(GREEN); }; //Gi colour
    //virtual RGB_t getIdleAnimationPulseColour(int ledIndex) const override { return getRGB(WHITE); };

    //virtual RGB_t notPressedStaticColour(int ledIndex) const override { return getRGB(GREEN); }; //Skin colour
    //virtual RGB_t pressedStaticColour(int ledIndex) const override { return getRGB(ORANGE); }; //Shorts/bangle colour
};

//Some other sf6 favs
class DeeJay : public Character
{
  private:
 
  public:
    virtual bool testForCharacterCombos( ) const override;
    
    //Example for RainbowCircle idle mode. Out of idle will be GREEN if not pressed and ORANGE if pressed
    //virtual RGB_t notPressedStaticColour(int ledIndex) const override { return getRGB(ORANGE); }; //Original gi colour
    //virtual RGB_t pressedStaticColour(int ledIndex) const override { return getRGB(GREEN); }; //New gi colour
};

class Marisa : public Character
{
  private:
 
  public:
    virtual bool testForCharacterCombos( ) const override;
    
    //Example for RainbowCircle idle mode. Out of idle will be GOLD if not pressed and RED if pressed
    //virtual RGB_t notPressedStaticColour(int ledIndex) const override { return getRGB(GOLD); }; //Gi trim colour
    //virtual RGB_t pressedStaticColour(int ledIndex) const override { return getRGB(RED); }; //hair colour
};

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

class Terry : public Character
{
  private:
 
  public:
    virtual bool testForCharacterCombos( ) const override;
    
    //These are the settings I like. Comment in and/or if you like.
    //virtual EIdleType getIdleAnimationType() const override { return EIT_Disabled; }
   
    //virtual int holdPressedButtonColourTimeInMS( ) const { return 500; };  //Make sure this plus...
    //virtual int fadePressedButtonColourTimeInMS( ) const { return 500; };  //...this is less than the time to restart the idle if turnNonHeldButtonsOff is true or it'll get stomped (IDLE_TIMEOUT_SECONDS * 1000)

    //virtual RGB_t notPressedStaticColour(int ledIndex) const override { return getRGB(RED); }; //jacket
    //virtual RGB_t pressedStaticColour(int ledIndex) const override { return getRGB(YELLOW); }; //hair colour
};

class JP : public Character
{
  private:
 
  public:
    virtual bool testForCharacterCombos( ) const override;
    
    //These are the settings Plagio_96 who requested JP asked for. Comment in or edit as you like.
    /*virtual RGB_t pressedStaticColour(int ledIndex) const override
    {
      if(ledIndex == LED_P4 || ledIndex == LED_K4)
        return getRGB(RED);
       if(ledIndex == LED_P1 || ledIndex == LED_K1 || ledIndex == LED_P2 || ledIndex == LED_K2 || ledIndex == LED_P3 || ledIndex == LED_K3)
        return getRGB(WHITE);

      //all other buttons and directions
      return getRGB(BLUE); 
    };*/
};
#endif
