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


#define _BVUL(bit) (1UL << (bit))

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

typedef struct {
	void *data;
	size_t siz;
	uint32_t sent;
	uint8_t flags;
} data_t;
 
#define CHUNK_CONFIRMED _BV(0)
#define PACKETS_SENT    _BV(1)
#define PARITY_ERROR    _BV(2)


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


/* NOTE: *start_time must be primed before first run */
int conditional_delay_ms(uint32_t delay_time, uint32_t *start_time)
{
	uint32_t cur_time = millis();

	// delay complete
	if (cur_time - *start_time >= delay_time) {
		*start_time = cur_time;
		return 1;
	}

	return 0;
}

/* populate data_t */
void init_data_t(data_t *in, void *data, size_t siz)
{
	in->data  = data;
	in->siz   = siz;
	in->sent  = 0;
	in->flags = 0;
}

/* send up to an 8-byte packet */
void send_packet(data_t *in)
{
	uint8_t *tracer = in->data;
	uint8_t cnt = 0;
	uint8_t i = 0;

#ifdef DEBUG
	uint8_t lcd_pos[2];
	sprintf(lcd_buf[0], "SEND PACKET: ");
	lcd_pos[0] = strlen(lcd_buf[0]);
	lcd_pos[1] = 0;
	lcd.clear();
	lcd.print(lcd_buf[0]);
#endif /* DEBUG */

	while (i < in->siz && i < 32 && cnt < 8) {
		if (!(in->sent & _BVUL(i))) {
			Serial.write(tracer[i]);
			++cnt;
			in->sent |= _BVUL(i);
#ifdef DEBUG
			sprintf(lcd_buf[1], "%2X", tracer[i]);
			lcd.print(lcd_buf[1]);
			lcd.setCursor(lcd_pos[1], 1);
			lcd.print(lcd_buf[1]);
			lcd_pos[1] += 2;
			lcd.setCursor(lcd_pos[0], 0);
			delay(LCD_PRINT_DELAY);
#endif /* DEBUG */
		}
		++i;
	}
}

uint32_t confirm_packet(uint32_t cur_bytes, uint8_t rec_bytes, size_t siz)
{
	uint8_t cnt = 0;
	uint8_t i   = 0;

	while (i < siz && i < 32 && cnt < 8) {
		// check if byte already confirmed
		if (!(cur_bytes & _BVUL(i))) {
			// set bit if byte received successfully
			cur_bytes |= (rec_bytes & _BVUL(cnt)) ? _BVUL(i) : 0;
			++cnt;
		}
		++i;
	}

	return cur_bytes;
}

/* return a mask with bytes that should be confirmed */
uint32_t confirm_mask(size_t siz)
{
	if (siz == 0) return siz;

	int32_t val, mask;
	val = mask = _BVUL(31);

	// perform a sign extension
	val >>= 31;
	// don't return zero for multiples of 32
	if (siz % 32 == 0) return (uint32_t) val;
	// get the appropriate mask
	mask >>= 31 - siz;

	// mask off the unused bits
	return (uint32_t) (val ^ mask);
}
