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
//

#ifndef __Characters_h__
#define __Characters_h__

#define __PROG_TYPES_COMPAT__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "kaimana_custom.h"
#include "animations.h"

extern Kaimana kaimana;

#define NUM_CHARACTERS 8

//define the base character class for combos
class Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) {};
};

//define the actual Characters
//The classic 8
class Ryu : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) override;
};

class Ken : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) override;
};

class Chun : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) override;
};

class Guile : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) override;
};

class Gief : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) override;
};

class Dhalsim : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) override;
};

class Honda : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) override;
};

class Blanka : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) override;
};
//Some other sf6 favs
class DeeJay : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) override;
};

class Marisa : public Character
{
  private:
 
  public:
    virtual void testForCharacterCombos( ) override;
};

#endif
