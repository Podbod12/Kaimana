//  CharacterData.h
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
//  Data driven characters. Instead of each character being hand written code, every character is a set of tables stored in
//  program memory (PROGMEM) that DataCharacter reads at runtime. The tables themselves live in GeneratedCharacters.cpp which is
//  written by the character editor (editor/character-editor.html) from the files in characters/. You should not need to edit this file
//  to add or change characters. Use the editor instead.
//
//  NOTE : If you change any of the structures or enums in here then the generator in editor/codegen.js must be updated to match
//
//  Created:  Sep, 2026    Paul 'pod' Denning
//

#ifndef __CharacterData_h__
#define __CharacterData_h__

#define __PROG_TYPES_COMPAT__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "kaimana.h"
#include "kaimana_custom.h"
#include "animations.h" // must come before Characters.h as they include each other
#include "Characters.h"

//Colour tables have one entry per button in this order
// P1, P2, P3, P4, K1, K2, K3, K4, Up, Down, Left, Right, Select, Start, Home
#define GEN_COLOUR_SLOTS    15

//Palette index that means "pick one of the random colours" rather than a fixed colour
#define GEN_COLOUR_RANDOM   0xFF

//Maximum number of buttons that must be pressed together to finish a move (eg Raging demon needs 2)
#define GEN_MAX_TRIGGERS    4

//Top bit of GenTest::motion marks it as a charge move
#define GEN_CHARGE_FLAG     0x80

//Which colour table of a character
enum EGenColourSet:uint8_t
{
  GCS_IdleStatic,
  GCS_IdlePulse,
  GCS_NotPressed,
  GCS_Pressed,
  GCS_Count,
};

//Animation step types. Parameters used by each (unused ones are 0)
enum EGenAnimType:uint8_t
{
  GAT_Wave,                    // p0 = EWaveType | (EWaveSpeed << 4), p1 = loops, colour
  GAT_FlashColour,             // p1 = time in ms, colour
  GAT_CircleRGB,               // p1 = loops
  GAT_CircleOneColour,         // p1 = loops, colour
  GAT_FlashAllSpeedIncreasing, // colour
  GAT_KnightRider,             // p0 = top row (1) or bottom row (0), p1 = loops, colour
  GAT_Randomise,               // p0 = number of flashes, p1 = time lit in ms, p2 = delay between in ms, colour
  GAT_Pause,                   // p1 = time in ms
};

//A sequence of inputs (eg quarter circle right) stored as a slice of GEN_MOTION_DATA
typedef struct __attribute__ ((__packed__)) {
  uint16_t offset;
  uint8_t length;
} GenMotion;

//The button(s) that finish a move
typedef struct __attribute__ ((__packed__)) {
  uint8_t count;
  EInputTypes inputs[GEN_MAX_TRIGGERS];
} GenTriggerSet;

//One way of performing a move. A move passes if any of its tests pass
typedef struct __attribute__ ((__packed__)) {
  uint8_t motion;      // index into GEN_MOTIONS, top bit is GEN_CHARGE_FLAG
  uint8_t triggerSet;  // index into GEN_TRIGGER_SETS
} GenTest;

//One step of the animation played when a move passes
typedef struct __attribute__ ((__packed__)) {
  uint8_t typeAndRepeat; // low 4 bits EGenAnimType, high 4 bits (number of times to play - 1)
  uint8_t p0;
  uint16_t p1;
  uint16_t p2;
  uint8_t colour;        // index into GEN_PALETTE or GEN_COLOUR_RANDOM
} GenAnim;

//A special move. Tests and animations are slices of GEN_TESTS and GEN_ANIMS
typedef struct __attribute__ ((__packed__)) {
  uint16_t firstTest;
  uint8_t testCount;
  uint16_t firstAnim;
  uint8_t animCount;
} GenMove;

//A whole character profile. Moves are a slice of GEN_MOVES in priority order (first is tested first)
typedef struct __attribute__ ((__packed__)) {
  uint8_t idleType;                // EIdleType
  uint8_t colourSets[GCS_Count];   // index into GEN_COLOUR_SETS for each EGenColourSet
  uint16_t holdMs;
  uint16_t fadeMs;
  uint16_t firstMove;
  uint8_t moveCount;
} GenCharacter;

//Tables defined in GeneratedCharacters.cpp
extern const RGB_t GEN_PALETTE[] PROGMEM;
extern const uint8_t GEN_COLOUR_SETS[][GEN_COLOUR_SLOTS] PROGMEM;
extern const EInputTypes GEN_MOTION_DATA[] PROGMEM;
extern const GenMotion GEN_MOTIONS[] PROGMEM;
extern const GenTriggerSet GEN_TRIGGER_SETS[] PROGMEM;
extern const GenTest GEN_TESTS[] PROGMEM;
extern const GenAnim GEN_ANIMS[] PROGMEM;
extern const GenMove GEN_MOVES[] PROGMEM;
extern const GenCharacter GEN_CHARACTERS[] PROGMEM;

//The 8 selectable profiles. Also defined in GeneratedCharacters.cpp
extern const Character* AllCharacters[NUM_CHARACTERS];

//A character whose settings all come from the generated tables
class DataCharacter : public Character
{
  private:
    const GenCharacter* _def;

    RGB_t colourFor(EGenColourSet colourSet, int ledIndex) const;

  public:
    DataCharacter(const GenCharacter* def) : _def(def) {}

    virtual bool testForCharacterCombos( ) const override;

    virtual EIdleType getIdleAnimationType() const override;
    virtual RGB_t getIdleAnimationStaticColour(int ledIndex) const override { return colourFor(GCS_IdleStatic, ledIndex); }
    virtual RGB_t getIdleAnimationPulseColour(int ledIndex) const override { return colourFor(GCS_IdlePulse, ledIndex); }

    virtual int holdPressedButtonColourTimeInMS( ) const override;
    virtual int fadePressedButtonColourTimeInMS( ) const override;

    virtual RGB_t notPressedStaticColour(int ledIndex) const override { return colourFor(GCS_NotPressed, ledIndex); }
    virtual RGB_t pressedStaticColour(int ledIndex) const override { return colourFor(GCS_Pressed, ledIndex); }
};

#endif
