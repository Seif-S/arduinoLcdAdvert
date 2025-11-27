#define __AVR_ATmega32U4__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lcd.h"
#include "uart.h"

// https://wokwi.com/projects/416241646559459329

// PORTB B (digital pin 8 to 13)
// C (analog input pins)
// PORTD D (digital pins 0 to 7)
#define LED_PIN 2
#define BUTTON_PIN 1

#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a,b) (!!((a) & (1ULL<<(b)))) 

#define BUTTON_IS_CLICKED(PINB,BUTTON_PIN) !BIT_CHECK(PINB,BUTTON_PIN)

typedef enum
{
    harrys,
    Pajer_AB,
    detektivbyra,
    Svartbyggen,
    buyAdvert,
}advert;

advert returnRandomAdvert();
void scrollText(char *text);
void flashingText(char *text);
void showMessage(char *text);

int main(void){
    init_serial();
    HD44780 lcd;

    lcd.Initialize(); // Initialize the LCD
    lcd.Clear();      // Clear the LCD

    int lastAdvert = buyAdvert;

    while(1)
    {
        int currentAdvert;
        int value;
        do
        {
            currentAdvert = returnRandomAdvert();
        } while (lastAdvert == currentAdvert);
        lastAdvert = currentAdvert;

        switch (currentAdvert)
        {
            case harrys:
                value = rand() % 3;
                switch (value)
                {
                case 0:
                    scrollText((char*)"Kop bil hos Harry");
                    break;
                case 1:
                    showMessage((char*)"En god bilaffar (for Harry!)");
                    break;
                case 2:
                    flashingText((char*)"Hederlige Harrys Bilar");
                    break;
                default:
                    printf("Message do not exist");
                    break;
                }
                break;
            case Pajer_AB:
                value = rand() % 2;
                switch (value)
                {
                case 0:
                    scrollText((char*)"Kop paj hos Farmor Anka");
                    break;
                case 1:
                    showMessage((char*)"Skynda innan Marten atit alla pajer");
                    break;
                default:
                    printf("Message do not exist");
                    break;
                }
                break;
            case detektivbyra:
                value = rand() % 2;
                switch (value)
                {
                case 0:
                    showMessage((char*)"Mysterier? Ring Langben");
                    break;
                case 1:
                    showMessage((char*)"Langben fixar biffen");
                    break;
                default:
                    printf("Message do not exist");
                    break;
                }
                break;
            case Svartbyggen:
                value = rand() % 2;
                switch (value)
                {
                case 0:
                    scrollText((char*)"Lat Petter bygga at dig");
                    break;
                case 1:
                    showMessage((char*)"Bygga svart? Ring Petter");
                    break;
                default:
                    printf("Message do not exist");
                    break;
                }
                break;
            case buyAdvert:
                showMessage((char*)"Synas har? IOT:s Reklambyra");
                break;
            default:
                printf("error novalue returned");
                break;
        }
    }
    while(1){
    }
    return 0;
}

void scrollText(char *text)
{
    HD44780 lcd;

    lcd.Initialize(); // Initialize the LCD
    lcd.Clear();      // Clear the LCD
    
    for (int x = 16; x >= 0; x--)
    {
        char rText[20] = " ";
        strncpy(rText, text, 16-x);
        lcd.Clear();
        lcd.GoTo(x, 0);
        lcd.WriteText(rText);

        _delay_ms(500);
    }
    for (int index = 1; index <= (int)strlen(text); index++)
    {
        char rText[20] = " ";
        lcd.Clear();
        lcd.GoTo(0, 0);
        strncpy(rText, text + index, 16);
        lcd.WriteText(rText);

        _delay_ms(500);
    }
    
}

void flashingText(char *text)
{
    HD44780 lcd;

    lcd.Initialize();
    lcd.Clear();

    for (int index = 0; index < 20; index++)
    {
        lcd.WriteText(text);
        _delay_ms(500);
        lcd.Clear();
        _delay_ms(500);
    }
}

void showMessage(char *text)
{
    HD44780 lcd;

    lcd.Initialize();
    lcd.Clear();

    lcd.WriteText(text);
    _delay_ms(20000);
}

advert returnRandomAdvert()
{
    // time(null) will always return same value
    int value = rand() % (14499 + 1);

    printf("%d ", value);
    
    if (value < 5000)
    { // Hederlige Harrys Bilar
        return harrys;
    }
    else if (value < 8000 && value >= 5000)
    { // Farmor Ankas Pajer AB
        return Pajer_AB;
    }
    else if (value < 12000 && value >= 8000)
    { // Långbens detektivbyrå
        return detektivbyra;
    }
    else if (value < 13500 && value >= 12000)
    { // Svarte Petters Svartbyggen
        return Svartbyggen;
    }
    else
    {// our own advert
        return buyAdvert;
    }
}
