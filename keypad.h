/*
 * keypad.h
 *
 *  Created on: Apr 15, 2024
 *      Author: Brianna
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#define COL1 GPIO_PIN_0		//Keypad_Pin1
#define COL2 GPIO_PIN_1		//Keypad_Pin2
#define COL3 GPIO_PIN_2		//Keypad_Pin3
#define COL4 GPIO_PIN_3		//Keypad_Pin4
#define ROW1 GPIO_PIN_4		//Keypad_Pin5
#define ROW2 GPIO_PIN_5		//Keypad_Pin6
#define ROW3 GPIO_PIN_6		//Keypad_Pin7
#define ROW4 GPIO_PIN_7		//Keypad_Pin8

#define COL_PINS (COL1 | COL2 | COL3 | COL4)
#define ROW_PINS (ROW1 | ROW2 | ROW3 | ROW4)

void Keypad_Config (void);
int Keypad_IsKeyPressed (void);
int Keypad_WhichKey (void);

#endif /* INC_KEYPAD_H_ */
