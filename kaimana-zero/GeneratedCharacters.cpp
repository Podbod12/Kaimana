//  GeneratedCharacters.cpp
//
//  AUTO GENERATED FILE - DO NOT EDIT BY HAND.
//  Written by editor/character-editor.html (or node editor/codegen.js) from the files in characters/.
//  Change your characters in the editor and save again, any edits made here will be lost.
//
//  Profile slots (hold P1+P4+K1+K4 then press the button to pick one):
//    P1 : Ryu
//    P2 : E. Honda
//    P3 : Blanka
//    P4 : Guile
//    K1 : Ken
//    K2 : Chun-Li
//    K3 : Zangief
//    K4 : Dhalsim
//

#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>
#include "CharacterData.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Colours
const RGB_t GEN_PALETTE[] PROGMEM = {
  {   0,   0,   0 }, // 0 BLACK
  { 255, 255, 255 }, // 1 WHITE
  {   0, 255, 255 }, // 2 CYAN
  { 255,   0,   0 }, // 3 RED
  { 255, 255,   0 }, // 4 YELLOW
  {   0, 255,   0 }, // 5 GREEN
  {   0,   0, 255 }, // 6 BLUE
  { 255, 150,   0 }, // 7 GOLD
};

// Colour of each button : P1, P2, P3, P4, K1, K2, K3, K4, Up, Down, Left, Right, Select, Start, Home (255 = random)
const uint8_t GEN_COLOUR_SETS[][GEN_COLOUR_SLOTS] PROGMEM = {
  {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }, // 0
  {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1 }, // 1
  { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 }, // 2
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Motions (the joystick part of a special move)
const EInputTypes GEN_MOTION_DATA[] PROGMEM = {
  EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, // DOUBLE_QUARTERCIRCLE_RIGHT
  EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, // DOUBLE_QUARTERCIRCLE_LEFT
  EIT_Input_Left, EIT_Input_Down, EIT_Input_DownLeft, // DP_LEFT
  EIT_Input_Right, EIT_Input_Down, EIT_Input_DownRight, // DP_RIGHT
  EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, // QUARTERCIRCLE_RIGHT
  EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, // QUARTERCIRCLE_LEFT
  EIT_Input_Right, EIT_Input_Left, EIT_Input_Right, EIT_Input_Left, // CHARGE_SUPER_RIGHT_LEFT
  EIT_Input_Left, EIT_Input_Right, EIT_Input_Left, EIT_Input_Right, // CHARGE_SUPER_LEFT_RIGHT
  EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, // HALFCIRCLE_RIGHT
  EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, // HALFCIRCLE_LEFT
  EIT_Input_Down, EIT_Input_Up, // CHARGE_DOWN_UP
  EIT_Input_Left, EIT_Input_Right, // CHARGE_LEFT_RIGHT
  EIT_Input_Right, EIT_Input_Left, // CHARGE_RIGHT_LEFT
  EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, // 630_LEFT_CLOCKWISE
  EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, // 630_UP_CLOCKWISE
  EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up, // 630_RIGHT_CLOCKWISE
  EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right, // 630_DOWN_CLOCKWISE
  EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up, // 630_LEFT_ANTICLOCKWISE
  EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, // 630_UP_ANTICLOCKWISE
  EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, // 630_RIGHT_ANTICLOCKWISE
  EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left, // 630_DOWN_ANTICLOCKWISE
  EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, // 270_LEFT_CLOCKWISE
  EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, // 270_UP_CLOCKWISE
  EIT_Input_Right, EIT_Input_DownRight, EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up, // 270_RIGHT_CLOCKWISE
  EIT_Input_Down, EIT_Input_DownLeft, EIT_Input_Left, EIT_Input_UpLeft, EIT_Input_Up, EIT_Input_UpRight, EIT_Input_Right, // 270_DOWN_CLOCKWISE
  EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up, // 270_LEFT_ANTICLOCKWISE
  EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, // 270_UP_ANTICLOCKWISE
  EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left, EIT_Input_DownLeft, EIT_Input_Down, // 270_RIGHT_ANTICLOCKWISE
  EIT_Input_Down, EIT_Input_DownRight, EIT_Input_Right, EIT_Input_UpRight, EIT_Input_Up, EIT_Input_UpLeft, EIT_Input_Left, // 270_DOWN_ANTICLOCKWISE
  EIT_Input_Left, // LEFT
  EIT_Input_Right, // RIGHT
};

const GenMotion GEN_MOTIONS[] PROGMEM = {
  { 0, 6 }, // 0 DOUBLE_QUARTERCIRCLE_RIGHT
  { 6, 6 }, // 1 DOUBLE_QUARTERCIRCLE_LEFT
  { 12, 3 }, // 2 DP_LEFT
  { 15, 3 }, // 3 DP_RIGHT
  { 18, 3 }, // 4 QUARTERCIRCLE_RIGHT
  { 21, 3 }, // 5 QUARTERCIRCLE_LEFT
  { 24, 4 }, // 6 CHARGE_SUPER_RIGHT_LEFT
  { 28, 4 }, // 7 CHARGE_SUPER_LEFT_RIGHT
  { 32, 5 }, // 8 HALFCIRCLE_RIGHT
  { 37, 5 }, // 9 HALFCIRCLE_LEFT
  { 42, 2 }, // 10 CHARGE_DOWN_UP
  { 44, 2 }, // 11 CHARGE_LEFT_RIGHT
  { 46, 2 }, // 12 CHARGE_RIGHT_LEFT
  { 48, 15 }, // 13 630_LEFT_CLOCKWISE
  { 63, 15 }, // 14 630_UP_CLOCKWISE
  { 78, 15 }, // 15 630_RIGHT_CLOCKWISE
  { 93, 15 }, // 16 630_DOWN_CLOCKWISE
  { 108, 15 }, // 17 630_LEFT_ANTICLOCKWISE
  { 123, 15 }, // 18 630_UP_ANTICLOCKWISE
  { 138, 15 }, // 19 630_RIGHT_ANTICLOCKWISE
  { 153, 15 }, // 20 630_DOWN_ANTICLOCKWISE
  { 168, 0 }, // 21 BUTTONS_ONLY
  { 168, 7 }, // 22 270_LEFT_CLOCKWISE
  { 175, 7 }, // 23 270_UP_CLOCKWISE
  { 182, 7 }, // 24 270_RIGHT_CLOCKWISE
  { 189, 7 }, // 25 270_DOWN_CLOCKWISE
  { 196, 7 }, // 26 270_LEFT_ANTICLOCKWISE
  { 203, 7 }, // 27 270_UP_ANTICLOCKWISE
  { 210, 7 }, // 28 270_RIGHT_ANTICLOCKWISE
  { 217, 7 }, // 29 270_DOWN_ANTICLOCKWISE
  { 224, 1 }, // 30 LEFT
  { 225, 1 }, // 31 RIGHT
};

// Buttons that finish a move
const GenTriggerSet GEN_TRIGGER_SETS[] PROGMEM = {
  { 1, { EIT_Input_K1 } }, // 0
  { 1, { EIT_Input_K2 } }, // 1
  { 1, { EIT_Input_K3 } }, // 2
  { 1, { EIT_Input_P1 } }, // 3
  { 1, { EIT_Input_P2 } }, // 4
  { 1, { EIT_Input_P3 } }, // 5
  { 3, { EIT_Input_P3, EIT_Input_P2, EIT_Input_P1 } }, // 6
  { 2, { EIT_Input_P1, EIT_Input_P2 } }, // 7
  { 2, { EIT_Input_P1, EIT_Input_P3 } }, // 8
  { 2, { EIT_Input_P2, EIT_Input_P3 } }, // 9
  { 3, { EIT_Input_K3, EIT_Input_K2, EIT_Input_K1 } }, // 10
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ways to perform each move : { motion index (+128 if charge), button set index }
const GenTest GEN_TESTS[] PROGMEM = {
  {   0,   0 }, // 0 DOUBLE_QUARTERCIRCLE_RIGHT + K1
  {   0,   1 }, // 1 DOUBLE_QUARTERCIRCLE_RIGHT + K2
  {   0,   2 }, // 2 DOUBLE_QUARTERCIRCLE_RIGHT + K3
  {   1,   0 }, // 3 DOUBLE_QUARTERCIRCLE_LEFT + K1
  {   1,   1 }, // 4 DOUBLE_QUARTERCIRCLE_LEFT + K2
  {   1,   2 }, // 5 DOUBLE_QUARTERCIRCLE_LEFT + K3
  {   0,   3 }, // 6 DOUBLE_QUARTERCIRCLE_RIGHT + P1
  {   0,   4 }, // 7 DOUBLE_QUARTERCIRCLE_RIGHT + P2
  {   0,   5 }, // 8 DOUBLE_QUARTERCIRCLE_RIGHT + P3
  {   1,   3 }, // 9 DOUBLE_QUARTERCIRCLE_LEFT + P1
  {   1,   4 }, // 10 DOUBLE_QUARTERCIRCLE_LEFT + P2
  {   1,   5 }, // 11 DOUBLE_QUARTERCIRCLE_LEFT + P3
  {   2,   3 }, // 12 DP_LEFT + P1
  {   2,   4 }, // 13 DP_LEFT + P2
  {   2,   5 }, // 14 DP_LEFT + P3
  {   3,   3 }, // 15 DP_RIGHT + P1
  {   3,   4 }, // 16 DP_RIGHT + P2
  {   3,   5 }, // 17 DP_RIGHT + P3
  {   4,   3 }, // 18 QUARTERCIRCLE_RIGHT + P1
  {   4,   4 }, // 19 QUARTERCIRCLE_RIGHT + P2
  {   4,   5 }, // 20 QUARTERCIRCLE_RIGHT + P3
  {   5,   3 }, // 21 QUARTERCIRCLE_LEFT + P1
  {   5,   4 }, // 22 QUARTERCIRCLE_LEFT + P2
  {   5,   5 }, // 23 QUARTERCIRCLE_LEFT + P3
  {   4,   0 }, // 24 QUARTERCIRCLE_RIGHT + K1
  {   4,   1 }, // 25 QUARTERCIRCLE_RIGHT + K2
  {   4,   2 }, // 26 QUARTERCIRCLE_RIGHT + K3
  {   5,   0 }, // 27 QUARTERCIRCLE_LEFT + K1
  {   5,   1 }, // 28 QUARTERCIRCLE_LEFT + K2
  {   5,   2 }, // 29 QUARTERCIRCLE_LEFT + K3
  { 134,   0 }, // 30 CHARGE_SUPER_RIGHT_LEFT (charge) + K1
  { 134,   1 }, // 31 CHARGE_SUPER_RIGHT_LEFT (charge) + K2
  { 134,   2 }, // 32 CHARGE_SUPER_RIGHT_LEFT (charge) + K3
  { 135,   0 }, // 33 CHARGE_SUPER_LEFT_RIGHT (charge) + K1
  { 135,   1 }, // 34 CHARGE_SUPER_LEFT_RIGHT (charge) + K2
  { 135,   2 }, // 35 CHARGE_SUPER_LEFT_RIGHT (charge) + K3
  {   8,   0 }, // 36 HALFCIRCLE_RIGHT + K1
  {   8,   1 }, // 37 HALFCIRCLE_RIGHT + K2
  {   8,   2 }, // 38 HALFCIRCLE_RIGHT + K3
  {   9,   0 }, // 39 HALFCIRCLE_LEFT + K1
  {   9,   1 }, // 40 HALFCIRCLE_LEFT + K2
  {   9,   2 }, // 41 HALFCIRCLE_LEFT + K3
  { 138,   0 }, // 42 CHARGE_DOWN_UP (charge) + K1
  { 138,   1 }, // 43 CHARGE_DOWN_UP (charge) + K2
  { 138,   2 }, // 44 CHARGE_DOWN_UP (charge) + K3
  { 139,   3 }, // 45 CHARGE_LEFT_RIGHT (charge) + P1
  { 139,   4 }, // 46 CHARGE_LEFT_RIGHT (charge) + P2
  { 139,   5 }, // 47 CHARGE_LEFT_RIGHT (charge) + P3
  { 140,   3 }, // 48 CHARGE_RIGHT_LEFT (charge) + P1
  { 140,   4 }, // 49 CHARGE_RIGHT_LEFT (charge) + P2
  { 140,   5 }, // 50 CHARGE_RIGHT_LEFT (charge) + P3
  {   4,   3 }, // 51 QUARTERCIRCLE_RIGHT + P1
  {   4,   4 }, // 52 QUARTERCIRCLE_RIGHT + P2
  {   4,   5 }, // 53 QUARTERCIRCLE_RIGHT + P3
  {   5,   3 }, // 54 QUARTERCIRCLE_LEFT + P1
  {   5,   4 }, // 55 QUARTERCIRCLE_LEFT + P2
  {   5,   5 }, // 56 QUARTERCIRCLE_LEFT + P3
  {   0,   3 }, // 57 DOUBLE_QUARTERCIRCLE_RIGHT + P1
  {   0,   4 }, // 58 DOUBLE_QUARTERCIRCLE_RIGHT + P2
  {   0,   5 }, // 59 DOUBLE_QUARTERCIRCLE_RIGHT + P3
  {   1,   3 }, // 60 DOUBLE_QUARTERCIRCLE_LEFT + P1
  {   1,   4 }, // 61 DOUBLE_QUARTERCIRCLE_LEFT + P2
  {   1,   5 }, // 62 DOUBLE_QUARTERCIRCLE_LEFT + P3
  { 134,   0 }, // 63 CHARGE_SUPER_RIGHT_LEFT (charge) + K1
  { 134,   1 }, // 64 CHARGE_SUPER_RIGHT_LEFT (charge) + K2
  { 134,   2 }, // 65 CHARGE_SUPER_RIGHT_LEFT (charge) + K3
  { 135,   0 }, // 66 CHARGE_SUPER_LEFT_RIGHT (charge) + K1
  { 135,   1 }, // 67 CHARGE_SUPER_LEFT_RIGHT (charge) + K2
  { 135,   2 }, // 68 CHARGE_SUPER_LEFT_RIGHT (charge) + K3
  { 134,   3 }, // 69 CHARGE_SUPER_RIGHT_LEFT (charge) + P1
  { 134,   4 }, // 70 CHARGE_SUPER_RIGHT_LEFT (charge) + P2
  { 134,   5 }, // 71 CHARGE_SUPER_RIGHT_LEFT (charge) + P3
  { 135,   3 }, // 72 CHARGE_SUPER_LEFT_RIGHT (charge) + P1
  { 135,   4 }, // 73 CHARGE_SUPER_LEFT_RIGHT (charge) + P2
  { 135,   5 }, // 74 CHARGE_SUPER_LEFT_RIGHT (charge) + P3
  {   0,   0 }, // 75 DOUBLE_QUARTERCIRCLE_RIGHT + K1
  {   0,   1 }, // 76 DOUBLE_QUARTERCIRCLE_RIGHT + K2
  {   0,   2 }, // 77 DOUBLE_QUARTERCIRCLE_RIGHT + K3
  {   1,   0 }, // 78 DOUBLE_QUARTERCIRCLE_LEFT + K1
  {   1,   1 }, // 79 DOUBLE_QUARTERCIRCLE_LEFT + K2
  {   1,   2 }, // 80 DOUBLE_QUARTERCIRCLE_LEFT + K3
  { 138,   0 }, // 81 CHARGE_DOWN_UP (charge) + K1
  { 138,   1 }, // 82 CHARGE_DOWN_UP (charge) + K2
  { 138,   2 }, // 83 CHARGE_DOWN_UP (charge) + K3
  {  13,   3 }, // 84 630_LEFT_CLOCKWISE + P1
  {  14,   3 }, // 85 630_UP_CLOCKWISE + P1
  {  15,   3 }, // 86 630_RIGHT_CLOCKWISE + P1
  {  16,   3 }, // 87 630_DOWN_CLOCKWISE + P1
  {  17,   3 }, // 88 630_LEFT_ANTICLOCKWISE + P1
  {  18,   3 }, // 89 630_UP_ANTICLOCKWISE + P1
  {  19,   3 }, // 90 630_RIGHT_ANTICLOCKWISE + P1
  {  20,   3 }, // 91 630_DOWN_ANTICLOCKWISE + P1
  {  13,   4 }, // 92 630_LEFT_CLOCKWISE + P2
  {  14,   4 }, // 93 630_UP_CLOCKWISE + P2
  {  15,   4 }, // 94 630_RIGHT_CLOCKWISE + P2
  {  16,   4 }, // 95 630_DOWN_CLOCKWISE + P2
  {  17,   4 }, // 96 630_LEFT_ANTICLOCKWISE + P2
  {  18,   4 }, // 97 630_UP_ANTICLOCKWISE + P2
  {  19,   4 }, // 98 630_RIGHT_ANTICLOCKWISE + P2
  {  20,   4 }, // 99 630_DOWN_ANTICLOCKWISE + P2
  {  13,   5 }, // 100 630_LEFT_CLOCKWISE + P3
  {  14,   5 }, // 101 630_UP_CLOCKWISE + P3
  {  15,   5 }, // 102 630_RIGHT_CLOCKWISE + P3
  {  16,   5 }, // 103 630_DOWN_CLOCKWISE + P3
  {  17,   5 }, // 104 630_LEFT_ANTICLOCKWISE + P3
  {  18,   5 }, // 105 630_UP_ANTICLOCKWISE + P3
  {  19,   5 }, // 106 630_RIGHT_ANTICLOCKWISE + P3
  {  20,   5 }, // 107 630_DOWN_ANTICLOCKWISE + P3
  {  21,   6 }, // 108 BUTTONS_ONLY + P3+P2+P1
  {  21,   7 }, // 109 BUTTONS_ONLY + P1+P2
  {  21,   8 }, // 110 BUTTONS_ONLY + P1+P3
  {  21,   9 }, // 111 BUTTONS_ONLY + P2+P3
  {  22,   3 }, // 112 270_LEFT_CLOCKWISE + P1
  {  23,   3 }, // 113 270_UP_CLOCKWISE + P1
  {  24,   3 }, // 114 270_RIGHT_CLOCKWISE + P1
  {  25,   3 }, // 115 270_DOWN_CLOCKWISE + P1
  {  26,   3 }, // 116 270_LEFT_ANTICLOCKWISE + P1
  {  27,   3 }, // 117 270_UP_ANTICLOCKWISE + P1
  {  28,   3 }, // 118 270_RIGHT_ANTICLOCKWISE + P1
  {  29,   3 }, // 119 270_DOWN_ANTICLOCKWISE + P1
  {  22,   4 }, // 120 270_LEFT_CLOCKWISE + P2
  {  23,   4 }, // 121 270_UP_CLOCKWISE + P2
  {  24,   4 }, // 122 270_RIGHT_CLOCKWISE + P2
  {  25,   4 }, // 123 270_DOWN_CLOCKWISE + P2
  {  26,   4 }, // 124 270_LEFT_ANTICLOCKWISE + P2
  {  27,   4 }, // 125 270_UP_ANTICLOCKWISE + P2
  {  28,   4 }, // 126 270_RIGHT_ANTICLOCKWISE + P2
  {  29,   4 }, // 127 270_DOWN_ANTICLOCKWISE + P2
  {  22,   5 }, // 128 270_LEFT_CLOCKWISE + P3
  {  23,   5 }, // 129 270_UP_CLOCKWISE + P3
  {  24,   5 }, // 130 270_RIGHT_CLOCKWISE + P3
  {  25,   5 }, // 131 270_DOWN_CLOCKWISE + P3
  {  26,   5 }, // 132 270_LEFT_ANTICLOCKWISE + P3
  {  27,   5 }, // 133 270_UP_ANTICLOCKWISE + P3
  {  28,   5 }, // 134 270_RIGHT_ANTICLOCKWISE + P3
  {  29,   5 }, // 135 270_DOWN_ANTICLOCKWISE + P3
  {  30,   6 }, // 136 LEFT + P3+P2+P1
  {  31,   6 }, // 137 RIGHT + P3+P2+P1
  {  30,  10 }, // 138 LEFT + K3+K2+K1
  {  31,  10 }, // 139 RIGHT + K3+K2+K1
  {   8,   0 }, // 140 HALFCIRCLE_RIGHT + K1
  {   9,   0 }, // 141 HALFCIRCLE_LEFT + K1
  {   8,   1 }, // 142 HALFCIRCLE_RIGHT + K2
  {   9,   1 }, // 143 HALFCIRCLE_LEFT + K2
  {   8,   2 }, // 144 HALFCIRCLE_RIGHT + K3
  {   9,   2 }, // 145 HALFCIRCLE_LEFT + K3
  {   8,   3 }, // 146 HALFCIRCLE_RIGHT + P1
  {   8,   4 }, // 147 HALFCIRCLE_RIGHT + P2
  {   8,   5 }, // 148 HALFCIRCLE_RIGHT + P3
  {   9,   3 }, // 149 HALFCIRCLE_LEFT + P1
  {   9,   4 }, // 150 HALFCIRCLE_LEFT + P2
  {   9,   5 }, // 151 HALFCIRCLE_LEFT + P3
};

// Animation steps : { type | (repeat - 1) << 4, p0, p1, p2, colour } - see EGenAnimType in CharacterData.h
const GenAnim GEN_ANIMS[] PROGMEM = {
  { GAT_FlashAllSpeedIncreasing, 0, 0, 0, 1 }, // 0
  { GAT_Wave, 16, 2, 0, 2 }, // 1
  { GAT_Wave, 19, 2, 0, 2 }, // 2
  { GAT_FlashColour, 0, 250, 0, 1 }, // 3
  { GAT_FlashColour, 0, 500, 0, 1 }, // 4
  { GAT_FlashColour, 0, 750, 0, 1 }, // 5
  { GAT_Wave, 0, 0, 0, 2 }, // 6
  { GAT_Wave, 16, 0, 0, 2 }, // 7
  { GAT_Wave, 32, 0, 0, 2 }, // 8
  { GAT_Wave, 3, 0, 0, 2 }, // 9
  { GAT_Wave, 19, 0, 0, 2 }, // 10
  { GAT_Wave, 35, 0, 0, 2 }, // 11
  { GAT_CircleOneColour, 0, 1, 0, 1 }, // 12
  { GAT_CircleOneColour, 0, 2, 0, 3 }, // 13
  { GAT_FlashAllSpeedIncreasing, 0, 0, 0, 3 }, // 14
  { GAT_CircleOneColour, 0, 2, 0, 4 }, // 15
  { GAT_Wave, 6, 0, 0, 4 }, // 16
  { GAT_Wave, 22, 0, 0, 4 }, // 17
  { GAT_Wave, 38, 0, 0, 4 }, // 18
  { GAT_Wave, 0, 0, 0, 4 }, // 19
  { GAT_Wave, 16, 0, 0, 4 }, // 20
  { GAT_Wave, 32, 0, 0, 4 }, // 21
  { GAT_Wave, 3, 0, 0, 4 }, // 22
  { GAT_Wave, 19, 0, 0, 4 }, // 23
  { GAT_Wave, 35, 0, 0, 4 }, // 24
  { GAT_Randomise, 16, 60, 30, 4 }, // 25
  { GAT_FlashAllSpeedIncreasing, 0, 0, 0, 4 }, // 26
  { GAT_FlashColour, 0, 750, 0, 4 }, // 27
  { GAT_CircleOneColour, 0, 2, 0, 5 }, // 28
  { GAT_Wave, 6, 0, 0, 5 }, // 29
  { GAT_Wave, 22, 0, 0, 5 }, // 30
  { GAT_Wave, 38, 0, 0, 5 }, // 31
  { GAT_Wave, 0, 0, 0, 5 }, // 32
  { GAT_Wave, 16, 0, 0, 5 }, // 33
  { GAT_Wave, 32, 0, 0, 5 }, // 34
  { GAT_Wave, 3, 0, 0, 5 }, // 35
  { GAT_Wave, 19, 0, 0, 5 }, // 36
  { GAT_Wave, 35, 0, 0, 5 }, // 37
  { GAT_Wave, 22, 0, 0, 5 }, // 38
  { GAT_Wave, 22, 0, 0, 5 }, // 39
  { GAT_CircleOneColour, 0, 3, 0, 5 }, // 40
  { GAT_CircleOneColour, 0, 3, 0, 4 }, // 41
  { GAT_CircleOneColour, 0, 1, 0, 4 }, // 42
  { GAT_FlashAllSpeedIncreasing, 0, 0, 0, 3 }, // 43
  { GAT_Wave, 16, 2, 0, 3 }, // 44
  { GAT_Wave, 19, 2, 0, 3 }, // 45
  { GAT_FlashColour, 0, 250, 0, 3 }, // 46
  { GAT_FlashColour, 0, 500, 0, 3 }, // 47
  { GAT_FlashColour, 0, 750, 0, 3 }, // 48
  { GAT_CircleOneColour, 0, 1, 0, 3 }, // 49
  { GAT_FlashAllSpeedIncreasing, 0, 0, 0, 6 }, // 50
  { GAT_FlashColour | (7 << 4), 0, 100, 0, 6 }, // 51
  { GAT_KnightRider, 0, 2, 0, 6 }, // 52
  { GAT_Wave, 0, 0, 0, 6 }, // 53
  { GAT_Wave, 16, 0, 0, 6 }, // 54
  { GAT_Wave, 32, 0, 0, 6 }, // 55
  { GAT_Wave, 3, 0, 0, 6 }, // 56
  { GAT_Wave, 19, 0, 0, 6 }, // 57
  { GAT_Wave, 35, 0, 0, 6 }, // 58
  { GAT_CircleRGB, 0, 1, 0, 0 }, // 59
  { GAT_FlashAllSpeedIncreasing, 0, 0, 0, 3 }, // 60
  { GAT_CircleOneColour, 0, 4, 0, 3 }, // 61
  { GAT_Wave, 22, 1, 0, 7 }, // 62
  { GAT_Wave, 23, 1, 0, 7 }, // 63
  { GAT_KnightRider, 1, 2, 0, 3 }, // 64
  { GAT_CircleOneColour, 0, 2, 0, 7 }, // 65
  { GAT_CircleOneColour, 0, 2, 0, 3 }, // 66
  { GAT_Wave, 22, 3, 0, 3 }, // 67
  { GAT_FlashAllSpeedIncreasing, 0, 0, 0, 3 }, // 68
  { GAT_Wave, 16, 4, 0, 3 }, // 69
  { GAT_Wave, 19, 4, 0, 3 }, // 70
  { GAT_KnightRider, 1, 1, 0, 3 }, // 71
  { GAT_KnightRider, 0, 1, 0, 3 }, // 72
  { GAT_Wave, 6, 2, 0, 3 }, // 73
  { GAT_Wave, 22, 2, 0, 3 }, // 74
  { GAT_Wave, 38, 2, 0, 3 }, // 75
  { GAT_Wave, 0, 0, 0, 3 }, // 76
  { GAT_Wave, 16, 0, 0, 3 }, // 77
  { GAT_Wave, 32, 0, 0, 3 }, // 78
  { GAT_Wave, 3, 0, 0, 3 }, // 79
  { GAT_Wave, 19, 0, 0, 3 }, // 80
  { GAT_Wave, 35, 0, 0, 3 }, // 81
  { GAT_Wave, 22, 0, 0, 3 }, // 82
};

// Special moves : { first test, test count, first animation, animation count }
const GenMove GEN_MOVES[] PROGMEM = {
  // Ryu
  { 0, 6, 0, 1 }, // 0 Super Dragon punch (both sides)
  { 6, 3, 1, 1 }, // 1 Super fireball right
  { 9, 3, 2, 1 }, // 2 Super fireball left
  { 12, 1, 3, 1 }, // 3 Dragon punch right (P1)
  { 13, 1, 4, 1 }, // 4 Dragon punch right (P2)
  { 14, 1, 5, 1 }, // 5 Dragon punch right (P3)
  { 15, 1, 3, 1 }, // 6 Dragon punch left (P1)
  { 16, 1, 4, 1 }, // 7 Dragon punch left (P2)
  { 17, 1, 5, 1 }, // 8 Dragon punch left (P3)
  { 18, 1, 6, 1 }, // 9 Fireball to the right (P1)
  { 19, 1, 7, 1 }, // 10 Fireball to the right (P2)
  { 20, 1, 8, 1 }, // 11 Fireball to the right (P3)
  { 21, 1, 9, 1 }, // 12 Fireball to the left (P1)
  { 22, 1, 10, 1 }, // 13 Fireball to the left (P2)
  { 23, 1, 11, 1 }, // 14 Fireball to the left (P3)
  { 24, 6, 12, 1 }, // 15 Hurricane kick right
  // E. Honda
  { 0, 6, 13, 2 }, // 16 Final bout (both sides)
  { 30, 3, 2, 1 }, // 17 killer head Ram Left
  { 33, 3, 1, 1 }, // 18 killer head Ram Right
  { 36, 6, 15, 1 }, // 19 Oicho left
  { 42, 1, 16, 1 }, // 20 sumo smash (K1)
  { 43, 1, 17, 1 }, // 21 sumo smash (K2)
  { 44, 1, 18, 1 }, // 22 sumo smash (K3)
  { 45, 1, 19, 1 }, // 23 headbutt to the right (P1)
  { 46, 1, 20, 1 }, // 24 headbutt to the right (P2)
  { 47, 1, 21, 1 }, // 25 headbutt to the right (P3)
  { 48, 1, 22, 1 }, // 26 headbutt to the left (P1)
  { 49, 1, 23, 1 }, // 27 headbutt to the left (P2)
  { 50, 1, 24, 1 }, // 28 headbutt to the left (P3)
  { 51, 6, 25, 1 }, // 29 HHSlap (Both sides)
  // Blanka
  { 0, 6, 26, 1 }, // 30 Ground shave cannonball (both sides)
  { 57, 6, 27, 1 }, // 31 shout of earth (both sides)
  { 36, 6, 28, 1 }, // 32 Backstep rolling attack (both sides
  { 42, 1, 29, 1 }, // 33 Vertical rolling attack (K1)
  { 43, 1, 30, 1 }, // 34 Vertical rolling attack (K2)
  { 44, 1, 31, 1 }, // 35 Vertical rolling attack (K3)
  { 45, 1, 32, 1 }, // 36 Rolling attack to the right (P1)
  { 46, 1, 33, 1 }, // 37 Rolling attack to the right (P2)
  { 47, 1, 34, 1 }, // 38 Rolling attack to the right (P3)
  { 48, 1, 35, 1 }, // 39 Rolling attack to the left (P1)
  { 49, 1, 36, 1 }, // 40 Rolling attack to the left (P2)
  { 50, 1, 37, 1 }, // 41 Rolling attack to the left (P3)
  { 51, 6, 25, 1 }, // 42 ElectricThunder (Both sides)
  // Guile
  { 63, 6, 38, 2 }, // 43 CrossFire somersault (both ways)
  { 57, 6, 40, 1 }, // 44 Solid puncher right (both sides)
  { 69, 6, 41, 1 }, // 45 Sonic Hurricane (BothSides)
  { 42, 1, 29, 1 }, // 46 Flash kick (K1)
  { 43, 1, 30, 1 }, // 47 Flash kick (K2)
  { 44, 1, 31, 1 }, // 48 Flash kick (K3)
  { 45, 1, 19, 1 }, // 49 Sonic boom to the right (P1)
  { 46, 1, 20, 1 }, // 50 Sonic boom to the right (P2)
  { 47, 1, 21, 1 }, // 51 Sonic boom to the right (P3)
  { 48, 1, 22, 1 }, // 52 Sonic boom to the left (P1)
  { 49, 1, 23, 1 }, // 53 Sonic boom to the left (P2)
  { 50, 1, 24, 1 }, // 54 Sonic boom to the left (P3)
  { 51, 6, 42, 1 }, // 55 sonic blade (Both sides)
  // Ken
  { 57, 6, 43, 1 }, // 56 Super Dragon punch right
  { 75, 3, 44, 1 }, // 57 Super kicks right
  { 78, 3, 45, 1 }, // 58 Super kicks left
  { 12, 1, 46, 1 }, // 59 Dragon punch right (P1)
  { 13, 1, 47, 1 }, // 60 Dragon punch right (P2)
  { 14, 1, 48, 1 }, // 61 Dragon punch right (P3)
  { 15, 1, 46, 1 }, // 62 Dragon punch left (P1)
  { 16, 1, 47, 1 }, // 63 Dragon punch left (P2)
  { 17, 1, 48, 1 }, // 64 Dragon punch left (P3)
  { 18, 1, 6, 1 }, // 65 Fireball to the right (P1)
  { 19, 1, 7, 1 }, // 66 Fireball to the right (P2)
  { 20, 1, 8, 1 }, // 67 Fireball to the right (P3)
  { 21, 1, 9, 1 }, // 68 Fireball to the left (P1)
  { 22, 1, 10, 1 }, // 69 Fireball to the left (P2)
  { 23, 1, 11, 1 }, // 70 Fireball to the left (P3)
  { 24, 6, 49, 1 }, // 71 Hurricane kick (both sides)
  // Chun-Li
  { 0, 6, 50, 1 }, // 72 Critical (both sides)
  { 6, 3, 1, 1 }, // 73 Super fireball right
  { 9, 3, 2, 1 }, // 74 Super fireball left
  { 24, 6, 51, 1 }, // 75 Hundred kicks (both sides)
  { 81, 3, 52, 1 }, // 76 Spinning Bird kick
  { 45, 1, 53, 1 }, // 77 Fireball Right (P1)
  { 46, 1, 54, 1 }, // 78 Fireball Right (P2)
  { 47, 1, 55, 1 }, // 79 Fireball Right (P3)
  { 48, 1, 56, 1 }, // 80 Fireball left (P1)
  { 49, 1, 57, 1 }, // 81 Fireball left (P2)
  { 50, 1, 58, 1 }, // 82 Fireball left (P3)
  // Zangief
  { 84, 24, 59, 2 }, // 83 Buster (both ways)
  { 57, 6, 61, 1 }, // 84 Cyclone Lariat (both sides)
  { 0, 6, 62, 2 }, // 85 AerialSlam (both sides)
  { 108, 4, 64, 1 }, // 86 Lariat
  { 36, 6, 65, 1 }, // 87 bear grab (both sides)
  { 112, 24, 66, 1 }, // 88 piledriver
  // Dhalsim
  { 0, 6, 67, 2 }, // 89 Merciless Yoga Both Sides
  { 6, 3, 69, 1 }, // 90 Inferno right
  { 9, 3, 70, 1 }, // 91 Inferno left
  { 136, 2, 71, 1 }, // 92 Teleport (P3+P2+P1)
  { 138, 2, 72, 1 }, // 93 Teleport (K3+K2+K1)
  { 140, 2, 73, 1 }, // 94 yoga Blast (K1)
  { 142, 2, 74, 1 }, // 95 yoga Blast (K2)
  { 144, 2, 75, 1 }, // 96 yoga Blast (K3)
  { 146, 3, 44, 1 }, // 97 Flame to the right
  { 149, 3, 45, 1 }, // 98 flame to the left
  { 18, 1, 76, 1 }, // 99 Fireball to the right (P1)
  { 19, 1, 77, 1 }, // 100 Fireball to the right (P2)
  { 20, 1, 78, 1 }, // 101 Fireball to the right (P3)
  { 21, 1, 79, 1 }, // 102 Fireball to the left (P1)
  { 22, 1, 80, 1 }, // 103 Fireball to the left (P2)
  { 23, 1, 81, 1 }, // 104 Fireball to the left (P3)
  { 24, 6, 82, 1 }, // 105 Yoga arch (both sides)
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Characters : { idle type, { idle static, idle pulse, not pressed, pressed colour sets }, hold ms, fade ms, first move, move count }
const GenCharacter GEN_CHARACTERS[] PROGMEM = {
  { EIT_RainbowCircling, { 0, 1, 0, 2 }, 0, 0, 0, 16 }, // 0 Ryu
  { EIT_RainbowPulsing, { 0, 1, 0, 2 }, 0, 0, 16, 14 }, // 1 E. Honda
  { EIT_RainbowCircling, { 0, 1, 0, 2 }, 0, 0, 30, 13 }, // 2 Blanka
  { EIT_RainbowCircling, { 0, 1, 0, 2 }, 0, 0, 43, 13 }, // 3 Guile
  { EIT_RainbowCircling, { 0, 1, 0, 2 }, 0, 0, 56, 16 }, // 4 Ken
  { EIT_RainbowCircling, { 0, 1, 0, 2 }, 0, 0, 72, 11 }, // 5 Chun-Li
  { EIT_RainbowCircling, { 0, 1, 0, 2 }, 0, 0, 83, 6 }, // 6 Zangief
  { EIT_RainbowCircling, { 0, 1, 0, 2 }, 0, 0, 89, 17 }, // 7 Dhalsim
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Profiles
DataCharacter gen_StreetFighter6_ryu(&GEN_CHARACTERS[0]); // Ryu
DataCharacter gen_StreetFighter6_honda(&GEN_CHARACTERS[1]); // E. Honda
DataCharacter gen_StreetFighter6_blanka(&GEN_CHARACTERS[2]); // Blanka
DataCharacter gen_StreetFighter6_guile(&GEN_CHARACTERS[3]); // Guile
DataCharacter gen_StreetFighter6_ken(&GEN_CHARACTERS[4]); // Ken
DataCharacter gen_StreetFighter6_chun(&GEN_CHARACTERS[5]); // Chun-Li
DataCharacter gen_StreetFighter6_gief(&GEN_CHARACTERS[6]); // Zangief
DataCharacter gen_StreetFighter6_dhalsim(&GEN_CHARACTERS[7]); // Dhalsim

const Character* AllCharacters[NUM_CHARACTERS] = { &gen_StreetFighter6_ryu, &gen_StreetFighter6_honda, &gen_StreetFighter6_blanka, &gen_StreetFighter6_guile, &gen_StreetFighter6_ken, &gen_StreetFighter6_chun, &gen_StreetFighter6_gief, &gen_StreetFighter6_dhalsim };
