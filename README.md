# Pods Kaimana V2.5

My kaimana code base is the most feature rich version available. Almost completely rewritten over the original version supplied by Paradise Arcade it provides a host of cool effects as well as bug fixes and better hardware support

Trailer for v2.0 features <a href="https://youtu.be/K5yb3yD0kEQ">Youtube link</a>

NEW FOR V2.5
- Web page GUI for creating, editing and setting characters/profiles

Please read the following for information. The further you go the more advanced it gets. Everyone should read to at least the end of the Special Button combos section and the Final Comments at the end.

## Features list

- Works with more leds than before. Users of the j4 line of leds would have had issues with the old code.
- Supports PA's 16 led joystick pcb
- Specify colour of every button for any mode
  - Idle mode (when you're not using the stick (see section on Idles)
  - When the button is pressed
  - When the button is not pressed and the idle mode isn't running
- 8 Different idle modes/animations
  - Circling rainbow
  - Pulsing rainbow (all buttons show the same colour)
  - Single pulse (colour of your choosing rotates around all the leds) against a static colour of your choosing)
  - Double pulse (same as single pulse but with 2 pulses set 180 degrees out of phase)
  - Ping pong pulse (single pulse that reverses direction at the start and end of the led list)
  - Static colour (single colour that doesn't change)
  - Pulsing Static colour (single colour that fades to 25% and back again)
  - Disabled (turns off idle mode from starting)
- Special move detection to trigger effect animations
  - Proper special move input detection to trigger animations. Tap and charge motions supported. Timings and input leniancy closely resembles how SF6 works
  - Many animations pre written for use with parameters for easy variations
- Character profile system
  - 13 Characters provided with v2.0 at time of writing.
  - Special moves and animations already pre-configured for instant use or to be used as examples for extending or writing new ones
  - Character editor webpage. Pick your 8 profiles, change colours, idle modes, special moves and animations, and preview them (including testing your special move inputs on the keyboard) without touching any code
- 8 profiles stored on kaimana at once
  - Easy profile swapping by using button combo
- Tournament mode toggle, Turn off all leds to prevent distractions or potential rule breaking
- Web page based GUI for creating, editing and setting characters/profiles

## First time Setup

Setup has been streamlined to make it easier than ever to get your kaimana setup working. There are just 2 places most users will need to edit.
Firstly you will need to take a look in kaimana_custom.h as this is where most of the settings are. There are two sections clearly marked out. One for basic/common users and one for slightly more advanced/rare users.
Each setting has a descriptive comment that should be enough to tell you what its for. Edit each setting as required. Dont worry if you know nothing about programming. All you need to know is that "//" means that everything to the right is ignored. Lets take a look at the first option

> // Use this for J4's on your buttons  <br />
> #define LED_PER_BUTTON 4  <br />
> // Use this for J2's on your buttons  <br />
> //#define LED_PER_BUTTON 2  <br />

If you have used J2's in your stick then simply remove the "//" from the 4th line and add "//" to the second like so!

> // Use this for J4's on your buttons  <br />
> //#define LED_PER_BUTTON 4  <br />
> // Use this for J2's on your buttons  <br />
> #define LED_PER_BUTTON 2  <br />

Easy!

One thing to note. For the LED order section. This is the order that you've hooked up your leds to the kaimana and each number specifies the first led on that button. By default the settings are set for the way my stick is setup. I have the kaimana hooked to Light punch (p1) and then they daisy chain along the punches, then back in reverse through the kicks. Finally it connects to the joystick pcb.  <br />
Since I have the J4's (4 leds per button) then each number is 4 more than the previous. If you're using J2's or you hooked everything up slightly differently you'll have to change this section to match yours! Any buttons that dont have leds should be set to 0xFF. This is a special code that allows it to know not to try and light that button <br />
LED_COUNT should always be the total number of leds which will most likely 2 or 4 more than the highest number you've entered (to account for all the leds on that button)

Once you've had a look and set all that needs setting in kaimana_custom.h you're almost done! By default the 8 world warriors from street fighter 2 are in the 8 profile slots. If you're happy with that just compile and upload. If you want to swap in one of the other bundled characters (or change anything about them) then use the character editor described in the next section. No code editing needed.

And thats all there is too it. Now read on for more information about the more advanced features! 

## Special Button combos

There are two button combos you need to be aware of. Each can be changed in the kaiama_custom.h file

Enter/Exit tournament mode - Hold "Home" button for 5 seconds. <br />
(Note : I dont actually have a button hooked up to the start, select or home pin of the kaimana so I've used K4 on my stick since I never have a situation where i hold it for more than 5 seconds)

Change character profile - Hold P1, P4, K1, K4 for 3 seconds. <br />
The lights will change white. Release the buttons and then they will turn red. Then the next button you press will select a character. P1 to P4 will select characters 1-4 in the AllCharacters list and k1 to k4 will select characters 5-8.

## The character editor

Everything about a character (button colours, idle mode, special moves and their animations) and which 8 characters are on your stick is set in the character editor. It's a webpage that is part of the repository, so there's nothing to install.

1. Open **editor/character-editor.html** in **Chrome** or **Edge** (just double click it, or drag it into the browser).
2. Click **Open sketch folder** and pick the folder that has kaimana-zero.ino in it (the one above the editor folder). The browser will ask if the page can edit files there. Say yes.
3. Make your changes (see below). The preview on the right shows exactly what your stick will do.
4. Click **Save**. This writes your characters into the characters folder (one file per character, see Sharing characters below) and writes GeneratedCharacters.cpp (the code the Arduino IDE compiles). Only files that actually changed are written.
5. Open kaimana-zero.ino in the Arduino IDE and hit upload as normal.

Next time you open the editor it will offer to reopen the same folder.

> Using Firefox or Safari? They can't save into folders, so click **Open sketch folder** and pick the same folder. The browser can read it but not write to it, so when you save it downloads kaimana-characters.zip with just the changed files. Unzip it into the sketch folder, replacing the old files (the message after saving also lists any files to delete, eg if you renamed a character).

### Profiles on the stick
The 8 dropdowns at the top left are the 8 profiles. Characters are grouped by folder (eg StreetFighter6). Set a character's folder at the top of its page, it's just for keeping things organised. P1 to P4 and K1 to K4 are the buttons you press to pick them after holding P1, P4, K1 and K4 (see Special Button combos). You can put the same character in more than one slot and it only takes up memory once.

### Look & idle
- **Idle animation** is what plays when you're not using the stick. Here are all the idle types
  - Rainbow Circling. Rainbow that rotates through every led
  - Rainbow Pulsing. Every button shows the same colour, cycling through the rainbow
  - Static Colour. A colour that doesn't move
  - Static Colour Pulsing. The static colour fading to 20% and back again
  - Static Colour Circle Pulse. A pulse of colour travelling around the buttons over the static colour
  - Static Colour Circle Dual Pulse. Same but with 2 pulses on opposite sides
  - Static Colour Ping Pong Pulse. A single pulse that bounces back and forth
  - Disabled. No idle animation
- **Not pressed** is the colour of buttons when you're playing but not holding them. **Pressed** is the colour while held. Pressed can also be random (the 🎲 button), which picks a new random colour each press. That's the default.
- Set the colour for **All buttons**, then click the little swatch next to any button to give it its own colour (eg the red, white and blue of the original sf2 cabinet, see the SF2 Arcade character). Click × to put it back to the all buttons colour.
- **After release** lets a button hold its pressed colour for a while and then fade back to the not pressed colour. ** NOTE : by default the idle mode kicks in after 1 second if you havent changed that in kaimana_custom.h. If you want longer fade times I would go and increase that first or it will kick in before your fade ends

### Special moves
Each move is a list of **inputs** (any one of them triggers the move) and an **animation** (a list of steps that play in order).

- An input is a **motion** (eg quarter circle right) plus the **button(s)** you press together at the end. Tick **charge** for charge moves, the first direction then has to be held for 750ms (CHARGE_COMBO_INPUT_TIME_WINDOW in kaimana_custom.h). The **×3 P** and **×3 K** buttons quickly make one input for each of LP, MP and HP (or the kicks).
- Moves are tested from the top of the list down and the first one that matches wins. So always keep the more complicated moves higher up than the simpler ones that contain a subset of its inputs, eg a super (double quarter circle) above a fireball (quarter circle). Drag the ≡ handle or use the arrows to reorder.
- **⇋ Mirror to other side** adds a copy of the move for the other side of the screen with left and right swapped, including the direction of wave animations. The kaimana doesn't know which way you're facing, so most moves are in the list twice.
- Press **▶** to play a move's animation in the preview. You can also type your moves on the keyboard to check the timing works: W A S D (or the arrow keys) is the joystick, U I O P are P1 to P4 and J K L ; are K1 to K4. You can also click and hold the buttons on the preview stick.

### Motion library
All the motions (quarter circles, dragon punches, charges, 360s, raging demon etc) are shared by every character. Click **Edit** to change one or **+ New motion** to make your own using the joystick pad. Motions of 7 or more inputs let you miss one input, which makes 360s much easier. Leave a motion empty for moves that are just buttons pressed together (eg Zangief's lariat from street fighter).

### Sharing characters
Every character is saved as its own file, so they are easy to swap:

> characters/profiles.json - which character is in each of your 8 slots <br />
> characters/motions.json - the shared motion library <br />
> characters/StreetFighter6/ryu.json - one file per character. The folder it sits in is the folder shown in the editor <br />

- To share a character, send someone its file (eg characters/StreetFighter6/ryu.json).
- To add a character someone sent you, drop the file into the characters folder, or a folder inside it (make one for the game if you like, eg characters/Tekken8/), then reopen the editor. That's it.
- Each character file also carries the motions it uses, so it works even if your motion library doesn't have them. If it has its own version of a motion you already have (same name, different inputs), the editor keeps both, renames the character's copy (eg QUARTERCIRCLE_RIGHT_DAVE_RYU) and tells you.
- Characters are identified by their folder and file name together, so a Ryu someone sends you in characters/Dave/ryu.json won't replace your characters/StreetFighter6/ryu.json. Both show up in the editor and you can even have both in slots.
- Pulling updates from the main repo only changes the characters that were actually changed there, so your edits to other characters are safe. If github says GeneratedCharacters.cpp has a conflict, don't try to fix it by hand. Just open the editor and save (or run the command below) and it will be regenerated from your characters.

### Without a browser
If you have node.js installed you can also regenerate GeneratedCharacters.cpp from the characters folder on the command line by running `node editor/codegen.js` in the sketch folder.

Lastly for this section. The colour pickers have the preset colours from kaimana.h as suggestions (BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE, ORANGE, GOLD, BROWN, GREY, DARKGREY, DARKBLUE) but you can pick any colour you like.

## (Advanced users recommended) Hand written C++ characters

This section is for people who have some programming knowledge. I will no longer be explaining all concepts. <br />
Characters made in the editor are stored as tables in program memory (see CharacterData.h and the generated GeneratedCharacters.cpp) and CharacterData.cpp reads them at runtime. That covers everything the bundled characters do, but if you want a character with custom logic you can still write one by hand like before.

The original hand written versions of the bundled characters are kept in CharacterArchive/ as templates: CharacterArchive/StreetFighter6 (Ryu.h and Ryu.cpp, Ken.h and Ken.cpp etc) and CharacterArchive/StreetFighter2 (SF2ArcadeControlPanel.h, which only sets colours so has no .cpp). Nothing in CharacterArchive is compiled or shown in the editor, it's just there to copy from.

To make a hand written character (for example, kimberly from sf6), copy a template pair into a folder under src/, eg CharacterArchive/StreetFighter6/Ryu.h and Ryu.cpp to src/StreetFighter3/Kimberly.h and Kimberly.cpp. Rename the class inside both to Kimberly and change the #include "Ryu.h" in the .cpp to "Kimberly.h". The editor finds every class under src/ (and in Characters.h) that derives from Character and lists it as "C++: Name" in the slot dropdowns, grouped by folder. Pick "C++: Kimberly" for a slot and save, and it adds the #include for its header to GeneratedCharacters.cpp for you. Hand written characters are only compiled into the firmware if you put one in a slot, so they cost no memory otherwise.

Folder and file rules. The Arduino IDE only compiles files in the sketch folder itself and anything under src/ (including sub folders), so hand written characters must live under src/ or they will be ignored (that's why CharacterArchive isn't compiled). Includes from src/<Folder>/ go back up two levels to reach the main files, eg #include "../../kaimana.h". The templates already do this, so they work as is once copied to src/<Folder>/. Keep folder and file names to letters, numbers, _ - and . (no spaces), starting with a letter or number, and match the case exactly in #includes as Mac and Linux care about case. Arduino limits sketch names to 63 characters, and Windows limits full paths to 260 characters. The Arduino IDE builds in a temp folder (C:\Users\<you>\AppData\Local\Temp\arduino\sketches\<id>\sketch\...) so keep src/<Folder>/<Name>.cpp under about 100 characters to be safe.

### Move details

Each move is a combination of 1 or more input tests followed by an animation to play. Heres one as an example

> if(kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K1Array, 1, false ) ) <br />
>  FlashAllSpeedIncreasing_Combo_Animation(WHITE); <br />

This tests for DOWN, DOWN-RIGHT, RIGHT followed by a Light kick (k1). if it passes it returns true and flashes all the buttons white a number of times

the syntax for the test is
Movetypes, NumInputInMoveType, ArrayOfTriggersToBePressed, TriggerArraySize, IsThisAChargeMove

Movetypes are specified in kaimana_custom.h and must be PROGMEM arrays (they're read with pgm_read_byte). You can add more if required <br />
Triggers is the final input(s) that finish a special move. For instance for Akuma's raging demon the trigger array is { p3, k1 } as these must be pressed at the same time. <br />
IsThisAChargeMove defines if the first input has to be held for 750ms. This is the charge time for moves in Street fighter 6. You can edit this value in kaimana_custom.h if required.

** NOTE : If you change the structures in CharacterData.h (for example to add a new animation type) then editor/codegen.js has to be updated to match, and editor/sim.js if you want the preview to show it.

## (Advanced users recommended) Memory limitations

Characters made in the editor are stored as compact tables, so the 8 bundled world warriors only use about 2KB between them and the whole firmware sits at around 67% of program memory. There's lots of room for more complicated characters (eg. EX moves for everyone). The editor shows an estimate of the program memory used at the top of the page, but the number the Arduino IDE prints when you compile is the real one. <br />
Hand written C++ characters are much bigger (roughly 1.5KB each). If you use a few of those and it no longer compiles, the easiest way to get around it is to put the same character in two slots. Each character only takes up memory once. So 8 slots but 7 unique characters will free up 1 characters worth of memory.

** Extra Warning : (I add this here because it happened to me and PA's wiki/instructions have been offline for months at time of writing! <br />
Arduino Leonardo (of which kaimana is based on) doesnt have a dedicated USB chip. As such, when memory is low it can find itself unable to respond to upload requests. You should not allow the SRAM (working memory) to go over 70% to be safe (its 38% at time of writing). If this happens to you and you cant upload then you can fix it by doing the following

> Turn on verbose logging in the arduino ide <br />
> Hit upload <br />
> Quickly bridge the two pins nearest the usb port on the kaimana in the batch of 6 next to the led connector port. I use a small key i have <br />
> Wait for the ide to start printing the com port check over and over <br />
> unbridge the pins <br />

that should solve it.

## Final Comments

Thanks for reading this far. I hope people use and enjoy this code and its features. Please spread the word to people who have kaimanas as I'm sure they'll appreciate it. 

Feel free to message me if you have any issues or have a cool feature idea. I'll fix all the bugs at least. If you write a new character. Please submit it and I'll add it to the depot!
