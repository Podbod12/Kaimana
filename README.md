# Pods Kaimana V2.0

My kaimana code base is the most feature rich version available. Almost completely rewritten over the original version supplied by Paradise Arcade it provides a host of cool effects as well as bug fixes and better hardware support

Trailer for v2.0 features <a href="https://youtu.be/K5yb3yD0kEQ">Youtube link</a>

Please read the following for information. The further you go the more advanced it gets. Everyone should read to at least the end of the Special Button combos section and the Final Comments at the end.

## Features list

- Works with more leds than before. Users of the j4 line of leds would have had issues with the old code.
- Supports PA's 16 led joystick pcb
- Specify colour of every button for any mode
  - Idle mode (when you're not using the stick (see section on Idles)
  - When the button is pressed
  - When the button is not pressed and the idle mode isnt running
- 8 Different idle modes/animations
  - Circiling rainbow
  - Pulsing ranibow (all buttons show the same colour)
  - Single pulse (colour of your choosing rotates around all the leds) against a static colour of your choosing)
  - Double pulse (same as single pulse but with 2 pulses set 180 degrees out of phase)
  - Ping pong pulse (single pulse that reverses direction at the start and end of the led list)
  - Static colour (single colour that doesnt change)
  - Pulsing Static colour (single colour that fades to 25% and back again)
  - Disabled (turns off idle mode from starting)
- Special move detection to trigger effect animations
  - Proper special move input detection to trigger animations. Tap and charge motions supported. Timings and input leniancy closely resembles how SF6 works
  - Many animations pre written for use with parameters for easy variations
- Character profile system
  - 13 Characters provided with v2.0 at time of writing.
  - Special moves and animations already pre-configured for instant use or to be used as examples for extending or writing new ones
- 8 profiles stored on kaimana at once
  - Easy profile swapping by using button combo
- Tournament mode toggle, Turn off all leds to prevent distractions or potential rule breaking

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

Once you've had a look and set all that needs setting in kaimana_custom.h its time to hop over to kaimana-zero.ino Dont worry tho, we're almost done!

In kaimana-zero you can specify which characters you would like in the 8 profile slots. By default the 8 world warriors from street fighter 2 are pre selected for you. If you would like one of the other 5 bundled characters then simply comment out (put // before them) one of the original characters and then remove the comment from one of the other 5. Then on the line that says

> const Character* AllCharacters[NUM_CHARACTERS] = { &ryu, &honda, &blanka, &guile, &ken, &chun, &gief, &sim };

replace the charatcer you commented out with one that you commented in! Lets do an example, Lets replace Ken with Akuma. This is how it looks now (some lines removed for easy reading)

> //Characters <br />
> Ken ken;  <br />
> //Akuma akuma;  <br />
> const Character* AllCharacters[NUM_CHARACTERS] = { &ryu, &honda, &blanka, &guile, &ken, &chun, &gief, &sim };  <br />

and this is what it will look like after

> //Ken ken;  <br />
> Akuma akuma;  <br />
> const Character* AllCharacters[NUM_CHARACTERS] = { &ryu, &honda, &blanka, &guile, &akuma, &chun, &gief, &sim };  <br />

Easy!

And thats all there is too it. Now read on for more information about the more advanced features! 

## Special Button combos

There are two button combos you need to be aware of. Each can be changed in the kaiama_custom.h file

Enter/Exit tournament mode - Hold "Home" button for 5 seconds. <br />
(Note : I dont actually have a button hooked up to the start, select or home pin of the kaimana so I've used K4 on my stick since I never have a situation where i hold it for more than 5 seconds)

Change character profile - Hold P1, P4, K1, K4 for 3 seconds. <br />
The lights will change white. Release the buttons and then they will turn red. Then the next button you press will select a character. P1 to P4 will select characters 1-4 in the AllCharacters list and k1 to k4 will select characters 5-8.

## Button colour and idle modes

Setting the colour of buttons is all done in Characters.h. This file "declares" all the characters that I have written (their special moves are all in files names charatername.cpp) and I have left an different example inside each character to show how to setup Idle modes and button colours. Each character is built onto of a base "Character" which speficies some defaults <br />
Lets explain and build an example here <br />
Here is an empty character

> class Honda : public Character  <br />
> {  <br />
>   private:  <br />
>    <br />
>   public:  <br />
>     virtual bool testForCharacterCombos( ) const override;  <br />
> };  <br />

By default this will use the base settings that "Character" defines. Which is rainbow cicling effect when idle, When you press any button all the buttons turn off. The button you pressed will light up in a random colour. Each time you press a new button it will light up in a new random colour. Releasing a button will instantly turn in black/off. After 1 second of not holding any button the idle mode will restart <br />
All cool but we can do better. Lets change the idle mode! Add these lines to the character under the "public:" heading

> virtual EIdleType getIdleAnimationType() const override { return EIT_StaticColourCirclePulse; }  <br />
> virtual RGB_t getIdleAnimationStaticColour(int ledIndex) const { return getRGB(RED); };  <br />
> virtual RGB_t getIdleAnimationPulseColour(int ledIndex) const { return getRGB(YELLOW); };  <br />

Now when we're idling it will turn all the buttons red and there will be a yellow pulse travelling around the buttons/leds! <br />
Here are all the idle types
>  EIT_RainbowCircling  <br />
>  EIT_RainbowPulsing <br />
>  EIT_StaticColour <br />
>  EIT_StaticColourPulsing <br />
>  EIT_StaticColourCirclePulse <br />
>  EIT_StaticColourCircleDualPulse <br />
>  EIT_StaticColourPingPongPulse <br />
>  EIT_Disabled <br />
  
Next lets change the buttons so they dont go black but Green when we exit the idle mode and if they're held they go Purple. Add these lines to your character under the ones you just added

> virtual RGB_t notPressedStaticColour(int ledIndex) const override { return getRGB(GREEN); };  <br />
> virtual RGB_t pressedStaticColour(int ledIndex) const override { return getRGB(PURPLE); };  <br />

Lastly. Its a bit boring for them to instantly turn off when we let go. So lets add a delay so they hold their colour for a little while and then fade out back to the Green colour. Add these lines next

> virtual int holdPressedButtonColourTimeInMS( ) const override { return 500; };   <br />
> virtual int fadePressedButtonColourTimeInMS( ) const override { return 500; };  <br />

If you want them to fade faster then just lower "500"! Easy peasy.

** NOTE : by default the idle mode kicks in after 1 second if you havent changed that in kaimana_custom.h. If you want longer fade times I would go and increase that first or it will kick in before your fade ends

Right so, we've changed the idle and set colours for every state the button can be in. But so far all the buttons are the same colour if they're in the same state. Lets add some variation! Lets make the buttons the same colour as the ones on the original sf2 arcade machine. These were red, white and blue! Replace the line that read notPressedStaticColour with this new block of code

>   virtual RGB_t notPressedStaticColour(int ledIndex) const override  <br />
>   {  <br />
>     if(ledIndex == LED_P1 || ledIndex == LED_K1)  <br />
>       return getRGB(RED);  <br />
>      if(ledIndex == LED_P2 || ledIndex == LED_K2)  <br />
>       return getRGB(WHITE);  <br />
>     if(ledIndex == LED_P3 || ledIndex == LED_K3)  <br />
>       return getRGB(BLUE);  <br />
>  <br />
>     //all other buttons and directions  <br />
>     return getRGB(YELLOW);   <br />
>   };   <br />

  Now this looks a lot more advanced but its not as complicated as it looks. Lets break it down line by line

>     if(ledIndex == LED_P1 || ledIndex == LED_K1)
  
  All this says is, if the button pressed is P1 (light punch) or K1(Light kick) then do whats on the next line. The "||" is the programming code for "or". If you want to add more buttons to this line just seperate each one with another "||". So we said it does whats on the next line, which is

>     return getRGB(RED);

  Turn the button Red! The other lines do the same thing but check different buttons and return a different colour. ** NOTE : The full list of the LED_XX names are the ones you set in kaimana_custom.h. Dont forget you can do the same for the joystick/direction buttons! <br />
  Finally at the bottom, we have catch all for any other button we havent specifically catered for. ** Note! You must always have this even if you catered for every button or you wont be able to compile and upload the code!

  Now we're cooking! The good news is that every line we added further up when setting colours for idles, pulses, pressed and not pressed can be changed in this way too! You can have a pulse that is a different colour on every button if you want (although I'm not sure how good that would look!). Experiment and find the exact setup that defines you and yourr stick!! <br />
  Lastly for this section. here is a list the preset colours for easy access. 

  BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE, ORANGE, GOLD, BROWN, GREY, DARKGREY, DARKBLUE

  You can add more in kamana.h if you like. They're specified by 3 numbers that correspond to red, green, blue and go upto a maximum for 255 for full brightness. eg heres how white and orange are written

>   #define  WHITE    255,255,255  <br />
>   #define  ORANGE   220,127,0  <br />

## (Advanced users recommended) Adding New Characters

This section is for people who have some programming knowledge. I will no longer be explaining all concepts. <br />
To add a new character I suggest copying an existing one and then making edits. So lets make a new character profile for Kimberly. First copy Ryu.cpp and call the new file "Kimberly.cpp". In characters.h find where ryu is declated "class Ryu : public Character" and copy that whole section (until the line with just "};") and paste it at the bottom of the file. Rename it from "Ryu" to "Kimberly" like so

> class Kimberly : public Character <br />
> { <br />
>   private: <br />
>   <br />
>   public: <br />
>     virtual bool testForCharacterCombos( ) const override; <br />
> }; <br />

Add Kimberly.cpp to your aurdino ide and change this line

> bool Ryu::testForCharacterCombos() const

to say Kimberly instead of Ryu.

Next, you just need to fill in each special move kimberly has. each move is a combination or 1 or more input tests followed by an animation to play. Since the kaimana has no idea about left or right facing. all my examples work both ways. This also means that if a move shares an input but is mirrored to another move then I wont have defined it. For example, quarter circle forward and punch and quarter circle back and punch.

Heres one as an example

> if(kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K1Array, 1, false ) ) <br />
>  FlashAllSpeedIncreasing_Combo_Animation(WHITE); <br />

This tests for DOWN, DOWN-RIGHT, RIGHT followed by a Light kick (k1). if it passes it returns true and flashes all the buttons white a number of times

the syntax for the test is 
Movetypes, NumInputInMoveType, ArrayOfTriggersToBePressed, TriggerArraySize, IsThisAChargeMove

Movetypes are specified in kaimana_custom. You can add more if required <br />
Triggers is the final input(s) that finish a special move. For instance for Akuma's raging demon the trigger array is { Forward, k1, p3 } as all these must be pressed at the same time. <br />
IsThisAChargeMove defines if the first input has to be held for 750ms. This is the charge time for moves in Street fighter 6. You can edit this value in kaimana_custom.h if required.

## (Advanced users recommended) Memory limitations

I've pushed the memory of the kaimana pretty far. It has enough space to allow the 8 profiles and all the animations. However, if you want to write more complicated characters (eg. you might want to add EX moves for everyone) or add new animations then you might well find that it no longer compiles. The easiest way to get around this is to duplicate a character in the AllCharacters array. <br />
Each character in that array only takes up memory once. So 8 entries but 7 unique ones will free up 1 characters worth of memory.

** Extra Warning : (I add this here because it happened to me and PA's wiki/instructions have been offline for months at time of writing! <br />
Arduino Leonardo (of which kaimana is based on) doesnt have a dedicated USB chip. As such, when memory is low it can find itself unable to respond to upload requests. You should not allow the SRAM (working memory) to go over 70% to be safe (i think its 49% at time of writing). If this happens to you and you cant upload then you can fix it by doing the following

> Turn on verbose logging in the arduino ide <br />
> Hit upload <br />
> Quickly bridge the two pins nearest the usb port on the kaimana in the batch of 6 next to the led connector port. I use a small key i have <br />
> Wait for the ide to start printing the com port check over and over <br />
> unbridge the pins <br />

that should solve it.

## Final Comments

Thanks for reading this far. I hope people use and enjoy this code and its features. Please spread the word to people who have kaimanas as I'm sure they'll appreciate it. 

Feel free to message me if you have any issues or have a cool feature idea. I'll fix all the bugs at least. If you write a new character. Please submit it and I'll add it to the depot!
