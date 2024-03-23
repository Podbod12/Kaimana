// Customize Kaimana settings here

#ifndef __kaimana_local_h__
#define __kaimana_local_h__

#define __PROG_TYPES_COMPAT__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "kaimana.h"



#define  _LED_ORDER_FULL

// Use this for J4's on your buttons
#define LED_PER_BUTTON 4
// Use this for J2's on your buttons
//#define LED_PER_BUTTON 2

//Use this if using J2's for leverless buttons
//#define LED_PER_JOYSTICK_DIRECTION 2
//Use this if using J4's for leverless buttons or the new Arcade Paradise 16 LED stick controller
#define LED_PER_JOYSTICK_DIRECTION 4

//If this is true then when you press a diagonal on the joystick it will light 4 lights in the diagonal instead of lighting both cardinal directions (for a total of 8 leds)
#define USING_JOYSTICK_LED_PCB_AND_WANT_PROPER_DIAGONALS true

//Set Led order here. The gap between each button/direction should match the numbers you just set above.
#ifdef _LED_ORDER_FULL
  #define  LED_P4  12
  #define  LED_P3  8
  #define  LED_P2  4
  #define  LED_P1  0
  #define  LED_K1  28
  #define  LED_K2  24
  #define  LED_K3  20
  #define  LED_K4  16
  #define  LED_HOME  0xFF
  #define  LED_START  0xFF
  #define  LED_SELECT  0xFF
  #define  LED_UP    44
  #define  LED_DOWN  36
  #define  LED_LEFT  32
  #define  LED_RIGHT 40 
  // maximum number of LEDs attached to Kaimana board
  #define  LED_COUNT   48
#endif

#define  LED_ENTRIES         12 // should match the unique entries above (and thus array size below)
const unsigned char ledList[LED_ENTRIES] = { LED_P4, LED_P3, LED_P2, LED_P1, LED_K1, LED_K2, LED_K3, LED_K4, LED_UP, LED_DOWN, LED_LEFT, LED_RIGHT};

// general definitions for delays and other customizable features
// specific to ParadiseArcadeShop.com Kaimana board (PS360+LED)
#define  BOOT_COLOR_DELAY         200    // value in miliseconds
#define  FAST_COLOR_DELAY         150    // value in miliseconds
#define  BOOT_COMPLETE_DELAY      500    // value in miliseconds
#define  IDLE_TIMEOUT_SECONDS       1    // value in seconds - normally 60 or 30 seconds but set very short for testing
#define  IDLE_ANIMATION_DELAY       5    // value in miliseconds - use smaller value for faster idle animation playback - Recommended you dont go below 5 or kaimana/leds cant update fast enough and will glitch
#define  MIN_LED_UPDATE_DELAY       5    // value in miliseconds - Gives kaimana/leds time to update correctly - Recommended you dont go below 5 or kaimana/leds cant update fast enough and will glitch
#define  T_DELAY      			  250    

// definitions of RGB values use by random color generator: setLEDRandomColor(int)
#define  COLOR_RANDOM_1    127,220,000    // lime green
#define  COLOR_RANDOM_2    127,000,220    // purple
#define  COLOR_RANDOM_3    000,220,220    // blue
#define  COLOR_RANDOM_4    000,255,127    // cyan
#define  COLOR_RANDOM_5    000,127,255    // cobalt blue
#define  COLOR_RANDOM_6    255,000,000    // red
#define  COLOR_RANDOM_7    220,127,000    // orange
#define  COLOR_RANDOM_8    220,000,127    // magenta
#define  COLOR_RANDOM_9	   177,037,075	
#define  NUM_RANDOM_COLORS 9
const RGB_t randomColors[NUM_RANDOM_COLORS] = { COLOR_RANDOM_1, COLOR_RANDOM_2, COLOR_RANDOM_3, COLOR_RANDOM_4, COLOR_RANDOM_5, COLOR_RANDOM_6, COLOR_RANDOM_7, COLOR_RANDOM_8, COLOR_RANDOM_9 };

// definitions for combo switch patterns
//
// These are some of the most common special move inputs. Feel free to add your own. Or add new shortcut versions as required.
// To add special moves to a character see Ryu.cpp as an example

#define COMBO_INPUT_TIME_WINDOW 200 //Maximum time between combo movements that arent a charge
#define COMBO_TRIGGER_INPUT_TIME_WINDOW 125 //Maximum time between combined button inputs eg PP, PPP, KK or PK etc
#define CHARGE_COMBO_INPUT_TIME_WINDOW 750 //How long the first direction of a charge move needs to be held - sf6 is mostly 40-45 frames = 670-750ms
#define COMBO_INPUT_COUNT_FOR_ONE_OUT_OF_TWO 7 //if the combo input is 7 or more moves then the input system will allow you to succeed if you dont miss 2 in a row window. eg LDRU would be fine for a SPD

#define QUARTERCIRCLE_INPUT_COUNT 3
const EInputTypes COMBO_QUARTERCIRCLE_RIGHT[QUARTERCIRCLE_INPUT_COUNT] = { EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right };
const EInputTypes COMBO_QUARTERCIRCLE_LEFT[QUARTERCIRCLE_INPUT_COUNT] = { EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left };

#define HALFCIRCLE_INPUT_COUNT 5
const EInputTypes COMBO_HALFCIRCLE_RIGHT[HALFCIRCLE_INPUT_COUNT] = { EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right };
const EInputTypes COMBO_HALFCIRCLE_LEFT[HALFCIRCLE_INPUT_COUNT] = { EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left };

#define DP_INPUT_COUNT 3
const EInputTypes COMBO_DP_RIGHT[DP_INPUT_COUNT] = { EIT_Input_Right, EIT_Input_Down, EIT_Input_DownRight };
const EInputTypes COMBO_DP_LEFT[DP_INPUT_COUNT] = { EIT_Input_Left, EIT_Input_Down, EIT_Input_DownLeft };

#define DOUBLE_QUARTERCIRCLE_INPUT_COUNT 6
const EInputTypes COMBO_DOUBLE_QUARTERCIRCLE_RIGHT[DOUBLE_QUARTERCIRCLE_INPUT_COUNT] = { EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right };
const EInputTypes COMBO_DOUBLE_QUARTERCIRCLE_LEFT[DOUBLE_QUARTERCIRCLE_INPUT_COUNT] = { EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left };

#define CHARGE_INPUT_COUNT 2
const EInputTypes COMBO_CHARGE_RIGHT_LEFT[CHARGE_INPUT_COUNT] = { EIT_Input_Right, EIT_Input_Left };
const EInputTypes COMBO_CHARGE_LEFT_RIGHT[CHARGE_INPUT_COUNT] = { EIT_Input_Left, EIT_Input_Right };

const EInputTypes COMBO_CHARGE_DOWN_UP[CHARGE_INPUT_COUNT] = { EIT_Input_Down, EIT_Input_Up };

#define CHARGE_SUPER_INPUT_COUNT 4
const EInputTypes COMBO_CHARGE_SUPER_RIGHT_LEFT[CHARGE_SUPER_INPUT_COUNT] = { EIT_Input_Right, EIT_Input_Left, EIT_Input_Right, EIT_Input_Left };
const EInputTypes COMBO_CHARGE_SUPER_LEFT_RIGHT[CHARGE_SUPER_INPUT_COUNT] = { EIT_Input_Left, EIT_Input_Right, EIT_Input_Left, EIT_Input_Right };

#define TWOSEVENTY_INPUT_COUNT 7 //triggers a 360 in street fighter
const EInputTypes COMBO_270_LEFT_CLOCKWISE[TWOSEVENTY_INPUT_COUNT] =      { EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down };
const EInputTypes COMBO_270_UP_CLOCKWISE[TWOSEVENTY_INPUT_COUNT] =        { EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left };
const EInputTypes COMBO_270_RIGHT_CLOCKWISE[TWOSEVENTY_INPUT_COUNT] =     { EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up };
const EInputTypes COMBO_270_DOWN_CLOCKWISE[TWOSEVENTY_INPUT_COUNT] =      { EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right };
const EInputTypes COMBO_270_LEFT_ANTICLOCKWISE[TWOSEVENTY_INPUT_COUNT] =  { EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up };
const EInputTypes COMBO_270_UP_ANTICLOCKWISE[TWOSEVENTY_INPUT_COUNT] =    { EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right };
const EInputTypes COMBO_270_RIGHT_ANTICLOCKWISE[TWOSEVENTY_INPUT_COUNT] = { EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down };
const EInputTypes COMBO_270_DOWN_ANTICLOCKWISE[TWOSEVENTY_INPUT_COUNT] =  { EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left };

#define SIXTHIRTY_INPUT_COUNT 15 //triggers a 720 in street fighter
const EInputTypes COMBO_630_LEFT_CLOCKWISE[SIXTHIRTY_INPUT_COUNT] =      { EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down };
const EInputTypes COMBO_630_UP_CLOCKWISE[SIXTHIRTY_INPUT_COUNT] =        { EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left };
const EInputTypes COMBO_630_RIGHT_CLOCKWISE[SIXTHIRTY_INPUT_COUNT] =     { EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up };
const EInputTypes COMBO_630_DOWN_CLOCKWISE[SIXTHIRTY_INPUT_COUNT] =      { EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right };
const EInputTypes COMBO_630_LEFT_ANTICLOCKWISE[SIXTHIRTY_INPUT_COUNT] =  { EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up };
const EInputTypes COMBO_630_UP_ANTICLOCKWISE[SIXTHIRTY_INPUT_COUNT] =    { EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right };
const EInputTypes COMBO_630_RIGHT_ANTICLOCKWISE[SIXTHIRTY_INPUT_COUNT] = { EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down };
const EInputTypes COMBO_630_DOWN_ANTICLOCKWISE[SIXTHIRTY_INPUT_COUNT] =  { EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left };

//Most trigger arrays are generated locally in the character but these are worth having global for ease of use
#define TRIPLEATTACK_TRIGGER_COUNT 3
const EInputTypes TRIGGER_PPP[TRIPLEATTACK_TRIGGER_COUNT] = { EIT_Input_P3, EIT_Input_P2, EIT_Input_P1 };
const EInputTypes TRIGGER_KKK[TRIPLEATTACK_TRIGGER_COUNT] = { EIT_Input_K3, EIT_Input_K2, EIT_Input_K1 };

// data points for color cycling
//
const uint8_t colorCycleData[] PROGMEM = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,

    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,

    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,

    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,

    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,

    0,   2,   4,   6,   8,  10,  12,  14,  16,  18,  20,  22,  24,  26,  28,  30,
   32,  35,  38,  41,  44,  47,  50,  53,  56,  59,  62,  65,  68,  71,  74,  77,
   81,  85,  89,  93,  97, 101, 105, 109, 113, 117, 121, 125, 129, 133, 137, 141,
  148, 155, 162, 169, 176, 183, 190, 197, 204, 211, 218, 225, 232, 239, 246, 253,

  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,

  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,

  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,

  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,

  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,

  253, 246, 239, 232, 225, 218, 211, 204, 197, 190, 183, 176, 169, 162, 155, 148,
  141, 137, 133, 129, 125, 121, 117, 113, 109, 105, 101,  97,  93,  89,  85,  81,
   77,  74,  71,  68,  65,  62,  59,  56,  53,  50,  47,  44,  41,  38,  35,  32,
   30,  28,  26,  24,  22,  20,  18,  16,  14,  12,  10,   8,   6,   4,   2,   0
};



#endif
