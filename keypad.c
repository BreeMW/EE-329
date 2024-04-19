/*
 *
  * @file   	: keypad.c
  * @brief  	: keypad configuration and debounced detection of keypresses functions
  * project 	: EE 329 S'23 Assignment 2
  * authors 	: Brianna Whann and Kassy Martinez
  * version 	:
  * date 		: 230417
  * compiler 	: STM32CubeIDE v.1.12.0 Build: 14980_20230301_1550 (UTC)
  * target 		: NUCLEO-L4A6ZG
  * clocks 		: 4 MHz MSI to AHB2
  *
  * Citations	: EE 329 S'24 Lab Manual (2024-Apr-15) Functions Kepad_IsKeyPressed and Keypad_WhichKey adapted
 */
#include "main.h"
#include "keypad.h"

void Keypad_Config (void){
	RCC->AHB2ENR   |= 	(RCC_AHB2ENR_GPIODEN);							//Enable GPIO D Bus

	GPIOD->MODER   &= 	~(GPIO_MODER_MODE4 | GPIO_MODER_MODE5 |			//Row Pins
					  	  GPIO_MODER_MODE6 | GPIO_MODER_MODE7);
	GPIOD->MODER   |= 	(GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0 |
						 GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0);
	GPIOD->OTYPER  &= ~(GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1);
	GPIOD->PUPDR   &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1);
	GPIOD->OSPEEDR |=  ((3 << GPIO_OSPEEDR_OSPEED0_Pos) |
	                    (3 << GPIO_OSPEEDR_OSPEED1_Pos));

	GPIOD->MODER   &=	~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1 |			//Column Pins
					  	  GPIO_MODER_MODE2 | GPIO_MODER_MODE3);
	GPIOD->PUPDR   &=	~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1 |
						  GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3);
	GPIOD->PUPDR   |=	(GPIO_PUPDR_PUPD0_1 | GPIO_PUPDR_PUPD1_1 |
							  GPIO_PUPDR_PUPD2_1 | GPIO_PUPDR_PUPD3_1);
}

int Keypad_IsKeyPressed (void) {
	//set all rows High
	GPIOD->BSRR = ROW_PINS;         	      // set all rows HI
	for (uint16_t idx=0; idx<200; idx++)   	// let it settle
		;
	if ((GPIOD->IDR & COL_PINS) != 0 )        // got a keypress!
		return 1;
	else				                        // nope.
		return 0;
}

int Keypad_WhichKey (void) {
	int8_t iRow=0, iCol=0, iKeyPress=0, iKey=0;  					// keypad row & col index, key ID result
	int8_t bGotKey = 0;             					// bool for keypress, 0 = no press
	GPIOD->BSRR = ROW_PINS;                       	 	// set all rows HI
	for (iCol = 0; iCol < 4; iCol++) {      	 				// check all COLUMNS
		if ( GPIOD->IDR & (GPIO_PIN_0 << iCol) ) {      	 	// keypress in iCol!!
			GPIOD->BRR = (ROW_PINS);            	 			// set all rows LO
	        for (iRow = 0; iRow < 4; iRow++) {   				// 1 row at a time
	        	GPIOD->BSRR = (GPIO_PIN_0 << (4+iRow));     	// set this row HI
	        	if (GPIOD->IDR & (GPIO_PIN_0 << iCol)) {    	// keypress in iCol!!
	        		bGotKey = 1;
	        		break;                                  	// exit for iRow loop
	        		}
	        }
	        if (bGotKey) {
	        	for (uint16_t idx=0; idx<200; idx++)				// Debounce Delay
	        		;
	            break;
	        }
		}
	}
	//encode into LED word:	Row 1: 1-3, A
	//						Row 2: 4-6, B
	//						Row 3: 7-8, C
	//						Row 4: *, 0, #, D
	//						No press: Return No_Keypress
	if (bGotKey) {
		iKeyPress = (iRow * 4) + iCol + 1;  		// handle numeric keys ...
		if ((iKeyPress == 1) || (iKeyPress == 2) || (iKeyPress == 3))			//Row 1 Columns 1-3		Numerical 1-3
			iKey = iKeyPress;
		else if ((iKeyPress == 5) || (iKeyPress == 6) || (iKeyPress == 7))		//Row 2 Columns 1-3		Numerical 4-6
			iKey = (iKeyPress - 1);
		else if (iKeyPress == 9 || (iKeyPress == 10) || (iKeyPress == 11)) {	//Row 3 Columns 1-3		Numerical 7-9
			iKey = (iKeyPress - 2);
		}
		else if (iKeyPress == 14) {				//Row 4 Column 2		Numerical Zero
			iKey = 0;
		}
		else if (iKeyPress == 4) {				//Row 1 Column 4		A
			iKey = 10;
		}
		else if (iKeyPress == 8) {				//Row 2 Column 4		B
			iKey = 11;
		}
		else if (iKeyPress == 12) {				//Row 3 Column 4		C
			iKey = 12;
		}
		else if (iKeyPress == 16) {				//Row 4 Column 4		D
			iKey = 13;
		}
		else if (iKeyPress == 13) {				//Row 4 Column 1		*
			iKey = 14;
		}
		else if (iKeyPress == 15) {				//Row 4 Column 3		#
			iKey = 15;
		}
		return(iKey);
	}
	else
		return (-1);
}


// * = 14    # = 15
