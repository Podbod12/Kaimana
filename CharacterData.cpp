//  CharacterData.cpp
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
//  Reads the generated character tables (see CharacterData.h) and turns them into button colours, special move tests and animations
//
//  Created:  Sep, 2026    Paul 'pod' Denning
//

#define __PROG_TYPES_COMPAT__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "kaimana.h"
#include "kaimana_custom.h"
#include "animations.h"
#include "CharacterData.h"

//LED for each colour slot. Order must match GEN_COLOUR_SLOTS
static const uint8_t GEN_SLOT_LEDS[GEN_COLOUR_SLOTS] PROGMEM = { LED_P1, LED_P2, LED_P3, LED_P4, LED_K1, LED_K2, LED_K3, LED_K4, LED_UP, LED_DOWN, LED_LEFT, LED_RIGHT, LED_SELECT, LED_START, LED_HOME };

static RGB_t paletteColour(uint8_t paletteIndex)
{
  if(paletteIndex == GEN_COLOUR_RANDOM)
    return randomColors[random(0, NUM_RANDOM_COLORS)];

  RGB_t colour;
  memcpy_P(&colour, &GEN_PALETTE[paletteIndex], sizeof(RGB_t));
  return colour;
}

RGB_t DataCharacter::colourFor(EGenColourSet colourSet, int ledIndex) const
{
  uint8_t slot = 0;
  for(; slot < GEN_COLOUR_SLOTS; ++slot)
  {
    if(pgm_read_byte_near(&GEN_SLOT_LEDS[slot]) == ledIndex)
      break;
  }
  if(slot == GEN_COLOUR_SLOTS)
    slot = 0;

  uint8_t setIndex = pgm_read_byte_near(&_def->colourSets[colourSet]);
  return paletteColour(pgm_read_byte_near(&GEN_COLOUR_SETS[setIndex][slot]));
}

EIdleType DataCharacter::getIdleAnimationType() const
{
  return (EIdleType)pgm_read_byte_near(&_def->idleType);
}

int DataCharacter::holdPressedButtonColourTimeInMS() const
{
  return pgm_read_word_near(&_def->holdMs);
}

int DataCharacter::fadePressedButtonColourTimeInMS() const
{
  return pgm_read_word_near(&_def->fadeMs);
}

static void playAnimation(const GenAnim& anim)
{
  RGB_t col = paletteColour(anim.colour);

  switch(anim.typeAndRepeat & 0x0F)
  {
    case GAT_Wave:
      WaveEffect_Combo_Animation((EWaveType)(anim.p0 & 0x0F), (EWaveSpeed)(anim.p0 >> 4), anim.p1, col.r, col.g, col.b);
      break;
    case GAT_FlashColour:
      FlashColour_Combo_Animation(col.r, col.g, col.b, anim.p1);
      break;
    case GAT_CircleRGB:
      CircleRGB_Combo_Animation(anim.p1);
      break;
    case GAT_CircleOneColour:
      Circle_OneColour_Combo_Animation(anim.p1, col.r, col.g, col.b);
      break;
    case GAT_FlashAllSpeedIncreasing:
      FlashAllSpeedIncreasing_Combo_Animation(col.r, col.g, col.b);
      break;
    case GAT_KnightRider:
      KnightRider_Combo_Animation(anim.p1, anim.p0 != 0, col.r, col.g, col.b);
      break;
    case GAT_Randomise:
      Randomise_Combo_Animation(anim.p0, anim.p1, anim.p2, col.r, col.g, col.b);
      break;
    case GAT_Pause:
      delay(anim.p1);
      break;
  }
}

bool DataCharacter::testForCharacterCombos() const
{
  GenCharacter character;
  memcpy_P(&character, _def, sizeof(GenCharacter));

  //Moves are stored most complex first so the first one to pass wins
  for(uint8_t moveIndex = 0; moveIndex < character.moveCount; ++moveIndex)
  {
    GenMove move;
    memcpy_P(&move, &GEN_MOVES[character.firstMove + moveIndex], sizeof(GenMove));

    for(uint8_t testIndex = 0; testIndex < move.testCount; ++testIndex)
    {
      GenTest test;
      GenMotion motion;
      GenTriggerSet triggers;
      memcpy_P(&test, &GEN_TESTS[move.firstTest + testIndex], sizeof(GenTest));
      memcpy_P(&motion, &GEN_MOTIONS[test.motion & ~GEN_CHARGE_FLAG], sizeof(GenMotion));
      memcpy_P(&triggers, &GEN_TRIGGER_SETS[test.triggerSet], sizeof(GenTriggerSet));

      if(kaimana.switchHistoryTest(&GEN_MOTION_DATA[motion.offset], motion.length, triggers.inputs, triggers.count, (test.motion & GEN_CHARGE_FLAG) != 0))
      {
        for(uint8_t animIndex = 0; animIndex < move.animCount; ++animIndex)
        {
          GenAnim anim;
          memcpy_P(&anim, &GEN_ANIMS[move.firstAnim + animIndex], sizeof(GenAnim));

          for(uint8_t repeat = 0; repeat <= (anim.typeAndRepeat >> 4); ++repeat)
            playAnimation(anim);
        }

        return true;
      }
    }
  }

  return false;
}
