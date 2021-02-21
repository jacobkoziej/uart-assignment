/*
 * Task 1 (master)
 * Copyright (C) 2021  Jacob Koziej <jacobkoziej@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "common.h"
#include "send.h"
#include "signals.h"
#include "types.h"


#define DEBUG

#ifdef   DEBUG
#include <LiquidCrystal.h>

#define LCD_RS     4
#define LCD_ENABLE 5
#define LCD_D4     6
#define LCD_D5     7
#define LCD_D6     8
#define LCD_D7     9

#define LCD_COL 16
#define LCD_ROW 2

LiquidCrystal lcd(
		LCD_RS,
		LCD_ENABLE,
		LCD_D4,
		LCD_D5,
		LCD_D6,
		LCD_D7
		);

#define LCD_PRINT_DELAY 1000
char lcd_buf[2][17];
#endif  /* DEBUG */


void setup()
{
#ifdef DEBUG
	lcd.begin(LCD_COL, LCD_ROW);
	lcd.clear();
#endif
	Serial.begin(9600);
	UCSR0C |= _BV(UPM01) | _BV(UPM00);  // odd parity
}

void loop()
{
}
