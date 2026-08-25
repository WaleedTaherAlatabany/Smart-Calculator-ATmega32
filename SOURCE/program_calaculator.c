/*
 * program_calaculator.c
 *
 *  Created on: Aug 23, 2026
 *      Author: alata
 */

#define F_CPU 8000000UL
#include "../INCLUDE/LIB/BIT_MATH.h"
#include "../INCLUDE/LIB/STD_TYPES.h"

// DIO Directives
#include "../INCLUDE/MCAL/DIO/DIO_INTERFACE.h"
#include "../INCLUDE/MCAL/DIO/DIO_PRIVATE.h"
#include "../INCLUDE/MCAL/DIO/DIO_CFG.h"

// LCD & KEYPAD Directives
#include "../INCLUDE/HAL/LCD/LCD_INTERFACE.h"
#include "../INCLUDE/HAL/KEYPAD/KEYPAD_INTERFACE.h"

#include <util/delay.h>

int main(void)
{

    u8 Local_u8Key;
    s32 Local_s32Num1 = 0;
    s32 Local_s32Num2 = 0;
    s32 Local_s32Result = 0;
    u8 Local_u8Operator = 0;
    u8 Local_u8State = 0;


    MDIO_voidSetPortDirection(PORTD, 0xFF);
    MDIO_voidSetPinDirection(PORTC, PIN0, DIO_OUTPUT); // RS
    MDIO_voidSetPinDirection(PORTC, PIN1, DIO_OUTPUT); // RW
    MDIO_voidSetPinDirection(PORTC, PIN7, DIO_OUTPUT); // E


    MDIO_voidSetPinDirection(PORTB, PIN0, DIO_OUTPUT);
    MDIO_voidSetPinDirection(PORTB, PIN1, DIO_OUTPUT);
    MDIO_voidSetPinDirection(PORTB, PIN2, DIO_OUTPUT);
    MDIO_voidSetPinDirection(PORTB, PIN3, DIO_OUTPUT);


    MDIO_voidSetPinDirection(PORTB, PIN4, DIO_INPUT);
    MDIO_voidSetPinDirection(PORTB, PIN5, DIO_INPUT);
    MDIO_voidSetPinDirection(PORTB, PIN6, DIO_INPUT);
    MDIO_voidSetPinDirection(PORTB, PIN7, DIO_INPUT);


    MDIO_voidSetPinValue(PORTB, PIN4, DIO_HIGH);
    MDIO_voidSetPinValue(PORTB, PIN5, DIO_HIGH);
    MDIO_voidSetPinValue(PORTB, PIN6, DIO_HIGH);
    MDIO_voidSetPinValue(PORTB, PIN7, DIO_HIGH);


    LCD_voidInit();
    LCD_voidWriteString((u8*)"Smart Calculator");
    _delay_ms(1500);
    LCD_voidClearDisplay();

    while (1)
    {

        Local_u8Key = HKEYPAD_u8GetPressedKey();


        if (Local_u8Key != 0xFF)
        {

            if (Local_u8Key == 'C')
            {
                LCD_voidClearDisplay();
                Local_s32Num1 = 0;
                Local_s32Num2 = 0;
                Local_u8Operator = 0;
                Local_u8State = 0;
            }


            else if (Local_u8Key >= '0' && Local_u8Key <= '9')
            {

                if (Local_u8State == 2)
                {
                    LCD_voidClearDisplay();
                    Local_s32Num1 = 0;
                    Local_s32Num2 = 0;
                    Local_u8Operator = 0;
                    Local_u8State = 0;
                }

                LCD_voidWriteChar(Local_u8Key);


                if (Local_u8State == 0)
                {
                    Local_s32Num1 = (Local_s32Num1 * 10) + (Local_u8Key - '0');
                }
                else if (Local_u8State == 1)
                {
                    Local_s32Num2 = (Local_s32Num2 * 10) + (Local_u8Key - '0');
                }
            }


            else if (Local_u8Key == '+' || Local_u8Key == '-' || Local_u8Key == '*' || Local_u8Key == '/')
            {

                if (Local_u8State == 2)
                {
                    Local_s32Num1 = Local_s32Result;
                    Local_s32Num2 = 0;
                    LCD_voidClearDisplay();

                    LCD_voidWrite_u32Number((u32)Local_s32Num1);
                }

                Local_u8Operator = Local_u8Key;
                Local_u8State = 1;
                LCD_voidWriteChar(Local_u8Key);
            }


           else if (Local_u8Key == '=')
            {
                if (Local_u8State == 1)
                {
                    LCD_voidWriteChar(Local_u8Key);


                    switch (Local_u8Operator)
                    {
                        case '+': Local_s32Result = Local_s32Num1 + Local_s32Num2; break;
                        case '-': Local_s32Result = Local_s32Num1 - Local_s32Num2; break;
                        case '*': Local_s32Result = Local_s32Num1 * Local_s32Num2; break;
                        case '/':
                            if (Local_s32Num2 != 0) Local_s32Result = Local_s32Num1 / Local_s32Num2;
                            break;
                    }


                    LCD_voidGoTo_XY(LINE_2, 0);


                    if (Local_u8Operator == '/' && Local_s32Num2 == 0)
                    {
                        LCD_voidWriteString((u8*)"Error: Div By 0");
                    }
                    else
                    {
                        LCD_voidWrite_u32Number((u32)Local_s32Result);
                    }

                    Local_u8State = 2;
                }
            }
        }
    }
}
