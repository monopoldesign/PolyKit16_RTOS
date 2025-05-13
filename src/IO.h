#ifndef _IO_H_
#define _IO_H_

/*----------------------------------------------------------------------------
- PolyKit16 - FreeRTOS-Version
- (C)2025 M.Volkel (mario.volkel@outlook.com)
-
- IO-Functions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
- Header-Files
----------------------------------------------------------------------------*/
#include <FreeRTOS_TEENSY4.h>
#include <Button.h>

/*----------------------------------------------------------------------------
- Definitions
----------------------------------------------------------------------------*/
#define TD_GETINPUTS	1

#define TP_GETINPUTS	(tskIDLE_PRIORITY + 2)

#define OCTO_TOTAL 2
#define NUM_BUTTONS		(8 * OCTO_TOTAL)

#define PIN_DATA		50	// pin 9 on 74HC165 (DATA)
#define PIN_LOAD		49  // pin 1 on 74HC165 (LOAD)
#define PIN_CLK			51  // pin 2 on 74HC165 (CLK))

/*----------------------------------------------------------------------------
- Class
----------------------------------------------------------------------------*/
class IO
{
	public:
			// Class-Functions
			IO();
			~IO();

			// Task-Functions
			static void createTasks();

			// Init-Functions
			static void init();

private:
			// Task-Functions
			static void task_GetInputs(void *arg);

			// Init-Functions
			static void initIOPins();

			// Hardware-Pin-Functions
			static void getInputSR();
			static uint8_t readShiftReg(uint32_t dataPin, uint32_t clkPin);

			// Button-Functions
			static void setupButtons();
			static void updateButtons();
			static void handleButtons();

			static void vaBut_notify(uint8_t but);
};

#endif
