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


typedef struct {
	void *data;
	size_t siz;
	uint32_t sent;
} data_t;


void setup()
{
	Serial.begin(9600);
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

/* send up to an 8-byte packet */
void send_packet(uint8_t *tracer, size_t siz, uint32_t sent)
{
	uint8_t cnt = 0;
	uint8_t i = 0;

	while (i < siz && i < 32 && cnt < 8) {
		if (!(sent & _BV(i))) {
			Serial.write(tracer[i]);
			++cnt;
		}
		++i;
	}
}
