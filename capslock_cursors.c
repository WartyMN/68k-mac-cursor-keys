/*
 * capslock_cursors.c
 *
 *  Created on: Dec 31, 2020
 *      Author: micahbly
 */


/* about
 *
 * INIT for Mac System 6.x through 9.x that adds alternate key layout when 
 *  capslock is engaged.
 *
 * Works by patching the GetNextEvent trap (tail patch) and modifying the
 *  EventRecord if one of the desired keys has been typed with capslock on
 *
 * Primary reference: 
 *  http://preserve.mactech.com/articles/mactech/Vol.05/05.10/INITinC/index.html
 * 
 */


/*****************************************************************************/
/*                                Includes                                   */
/*****************************************************************************/

#include <SetUpA4.h>
#include <Traps.h>


/*****************************************************************************/
/*                            Macro Definitions                              */
/*****************************************************************************/

//#define WaitNextEventTrap 0xA860 // this is not in the mac plus ROM!
#define GetNextEventTrap 0xA970


/*****************************************************************************/
/*                               Enumerations                                */
/*****************************************************************************/


/*****************************************************************************/
/*                                 Structs                                   */
/*****************************************************************************/


/*****************************************************************************/
/*                             Global Variables                              */
/*****************************************************************************/

long origGetNextEvent; // address of original GetNextEvent


/*****************************************************************************/
/*                       Public Function Prototypes                          */
/*****************************************************************************/

void main(void);

// Patch for Toolbox GetNextEvent
pascal Boolean NewGetNextEvent(short eventMask, EventRecord *theEvent);


/*****************************************************************************/
/*                        Public Function Definitions                        */
/*****************************************************************************/


// Patch for Toolbox GetNextEvent
//   Calls ToolBox GetNextEvent, modifies EventRecord.message if appropriate
pascal Boolean NewGetNextEvent(short eventMask, EventRecord *theEvent)
{
	long theKey;
	long theChar;
	long modifiedKey = 0;
	long modifiedChar = 0;
	Boolean modified = TRUE;
	Boolean eventNeedsAction;

	// LOGIC:
	//   call original GetNextEvent()
	//   if a keydown event, inspect modifier. if not CapsLock, stop
	//   if charCode is for an uppercase alpha, and shift is NOT engaged,
	//     convert to lowercase alpha. This allows user to leave capslock on
	//     and still be productive generally (until they need [, ], \ =)
	//   inspect the keyCode. If [, ], \, or =, translate to a cursor key
	//   UPDATE: 
	//     - so far, it appears most programs are ignoring keyCode, and just
	//         looking at charCode. Must set charCode. Am also setting keyCode
	//         but without real expectation that programs need it or see it
	
	// ThinkC helper: 
	//   SetUpA4 puts current value in A4 on the stack and gets 
    //   value in special spot and puts it in A4.
	SetUpA4();

	// call original GetNextEvent
	eventNeedsAction = CallPascalB(eventMask, theEvent, origGetNextEvent);

	if (eventNeedsAction)
	{
		// LOGIC: on key repeats, the keyDown doesn't fire, but autoKey does
		if (theEvent->what == keyDown || theEvent->what == autoKey)
		{
			if (theEvent->modifiers & alphaLock)
			{
				theKey = (theEvent->message & keyCodeMask) >> 8;
				theChar = theEvent->message & charCodeMask;
				
				// is this an alpha key? If so, switch it back to lowercase
				// LOGIC: use charcode here, as keycode is not in a continuous block
				if ( !(theEvent->modifiers & shiftKey) && 
						(theChar > 0x40 && theChar < 0x5b) )
				{
					modifiedChar = theChar + 0x20;
					modifiedKey = theKey; // no need to change keyCode
				}
				else
				{
					// check for "cursor" keys
					switch(theKey)
					{
						case 24: // 0x18 - key code for this key on mac plus
							// = key -> UP CURSOR
							modifiedChar = 0x1e;
							modifiedKey = 0x4d; // Mac Plus keyboard desired cursor key
							break;
						
						case 33: // 0x21 - key code for this key on mac plus
							// [ key -> LEFT CURSOR
							modifiedChar = 0x1c; 
							modifiedKey = 0x46; // Mac Plus keyboard desired cursor key
							break;
						
						case 30: // 0x1E - key code for this key on mac plus
							// ] key -> DOWN CURSOR
							modifiedChar = 0x1f;
							modifiedKey = 0x48; // Mac Plus keyboard desired cursor key
							break;
						
						case 42: // 0x2a - key code for this key on mac plus
							// \ key -> RIGHT CURSOR
							modifiedChar = 0x1d;
							modifiedKey = 0x42; // Mac Plus keyboard desired cursor key
							break;
						
						default:
							modified = FALSE;
					}
				}
				
				
				if (modified)
				{
					// re-mask with adjusted values

					// blank out lower 2 bytes, preserve 3rd/4th byte
					theEvent->message &= ~0xFFFF;

					modifiedKey = modifiedKey << 8;
					theEvent->message |= modifiedKey;
					theEvent->message |= modifiedChar;
					// programs seem to use charCode over keycode?
					
					// clear the alphalock modifier in case having it on confseds some program
					//theEvent->modifiers &= ~alphaLock;
				}
			}
		}
	}
	
	// ThinkC helper: 
	//   RestoreA4 pops saved value of A4 from stack into A4.
	RestoreA4();
	
	return eventNeedsAction;
}


// Startup code: Install patch to GetNextEvent()
void main(void)
{
	Handle myHandle;
	Ptr myPtr;

	// Save 
	asm
	{
		move.l A0, myPtr
	}
	
	// Think C special goop: 
	//    RememberA4 puts the value in A4 in special spot.
 	RememberA0();
 	SetUpA4();
 	
 	// Install the patch unless user is holding down mouse button on startup
 	if(!Button()) 
 	{
 		myHandle = RecoverHandle(myPtr); 		
		DetachResource(myHandle);

 		origGetNextEvent = NGetTrapAddress((int)GetNextEventTrap, ToolTrap);
		NSetTrapAddress((long)NewGetNextEvent, (int)GetNextEventTrap, ToolTrap);
	}
	
	RestoreA4();
}
