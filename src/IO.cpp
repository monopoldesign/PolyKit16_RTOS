/*----------------------------------------------------------------------------
- PolyKit16 - FreeRTOS-Version
- (C)2025 M.Volkel (mario.volkel@outlook.com)
-
- IO-Functions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
- Header-Files
----------------------------------------------------------------------------*/
#include "IO.h"
#include "main.h"

/*----------------------------------------------------------------------------
- Global Variables
----------------------------------------------------------------------------*/
uint8_t input_sr[OCTO_TOTAL];
Button buttons[NUM_BUTTONS];

/*----------------------------------------------------------------------------
- public Class-Functions
----------------------------------------------------------------------------*/
/******************************************************************************
* @brief
* @param  None
*******************************************************************************/
IO::IO()
{
	Serial.println("I/O-Object created");
}

/******************************************************************************
* @brief
* @param  None
*******************************************************************************/
IO::~IO()
{
}

/*----------------------------------------------------------------------------
- Task-Functions
----------------------------------------------------------------------------*/
/******************************************************************************
* @brief
* @param  None
*******************************************************************************/
void IO::createTasks()
{
	xTaskCreate(task_GetInputs, NULL, TASK_STACKSIZE, NULL, TP_GETINPUTS, NULL);
}

/*----------------------------------------------------------------------------
- Init-Functions
----------------------------------------------------------------------------*/
/******************************************************************************
* @brief
* @param  None
*******************************************************************************/
void IO::init()
{
	initIOPins();
	setupButtons();
}

/*----------------------------------------------------------------------------
- private Class-Functions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
- Task-Functions
----------------------------------------------------------------------------*/
/******************************************************************************
* @brief
* @param  None
*******************************************************************************/
void IO::task_GetInputs(void *arg)
{
	portTickType xLastExecutionTime;

	xLastExecutionTime = xTaskGetTickCount();

	while (1)
	{
		vTaskDelayUntil(&xLastExecutionTime, TD_GETINPUTS / portTICK_RATE_MS);

		//elapsedMicros myTime = 0;

		getInputSR();			// 8us
 
		//app->vaTick();		// 2us

		/*
		app->vaButton_change();		// 1us
		app->vaEnc_buttons();

		Serial.println(myTime);
        */
	}
}

/******************************************************************************
* @brief
* @param  None
*******************************************************************************/
void IO::initIOPins()
{
	pinMode(PIN_LOAD, OUTPUT);
	pinMode(PIN_CLK, OUTPUT);
	pinMode(PIN_DATA, INPUT);

	getInputSR();
}

/*----------------------------------------------------------------------------
- Hardware-Pin-Functions
----------------------------------------------------------------------------*/
/******************************************************************************
* @brief
* @param  None
*******************************************************************************/
void IO::getInputSR()
{
	digitalWrite(PIN_LOAD, LOW);
	digitalWrite(PIN_LOAD, HIGH);

	for (uint8_t i = 0; i < OCTO_TOTAL; i++)
		input_sr[i] = readShiftReg(PIN_DATA, PIN_CLK);
}

/******************************************************************************
* @brief
* @param  None
*******************************************************************************/
uint8_t IO::readShiftReg(uint32_t dataPin, uint32_t clkPin)
{
	uint8_t _bit = 0;
	uint8_t _byte = 0;

	for (int i = 0; i < 8; i++)
	{
		_bit = digitalRead(dataPin);
		_byte |= (_bit << ((8-1) - i));

		digitalWrite(clkPin, HIGH);
		digitalWrite(clkPin, LOW);
	}

	return (_byte);
}

/*----------------------------------------------------------------------------
- Button-Functions
----------------------------------------------------------------------------*/
/******************************************************************************
* @brief
* @param  None
*******************************************************************************/
void IO::setupButtons()
{
	for (uint8_t i = 0; i < (8 * OCTO_TOTAL); i++)
	{
		buttons[i].setup(i, LOW, &input_sr[0]);
		buttons[i + 8].setup(i + 8, LOW, &input_sr[1]);
	}
}

/******************************************************************************
* @brief
* @param  None
*******************************************************************************/
void IO::updateButtons()
{
	for (auto& item : buttons)
		item.listen();
}

/******************************************************************************
* @brief
* @param  None
*******************************************************************************/
void IO::handleButtons()
{
	uint8_t i = 0;
	for (auto& item : buttons)
	{
		if (item.onPress())
			vaBut_notify(i);

		//if (item.onRelease())
		//	vaBut_notify_release(i);

		i++;
	}
}

/******************************************************************************
* @brief
* @param  None
*******************************************************************************/
void vaBut_notify(uint8_t but)
{
	switch (but)
	{
		case 0:
			Serial.println("Button0 pressed");
			break;

		case 1:
			Serial.println("Button1 pressed");
			break;

		case 2:
			Serial.println("Button2 pressed");
			break;

		case 3:
			Serial.println("Button3 pressed");
			break;

		case 8:
			Serial.println("Button8 pressed");
			break;

		case 9:
			Serial.println("Button9 pressed");
			break;

		case 10:
			Serial.println("Button10 pressed");
			break;

		case 11:
			Serial.println("Button11 pressed");
			break;

		default:
			Serial.println("Other Button pressed");
			break;
	}
}
