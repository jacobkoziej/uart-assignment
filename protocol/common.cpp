/*
 * common.cpp -- common functions for communication protocol
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
#include "signals.h"
#include "types.h"


/* check if the next byte has a parity error */
inline uint8_t check_parity()
{
	// return 1 on no parity error
	return UCSR0A & _BV(UPE0) ? 0 : 1;
}

/* flush all data in the serial buffer */
inline void flush_serial_buffer()
{
	while (Serial.available()) Serial.read();
}

/* return a mask with bytes that should be confirmed */
uint32_t confirm_mask(size_t siz)
{
	// special cases
	if (siz == 0) return 0;
	if (!(siz % 32)) return ~0UL;

	int32_t val = _BVUL(31);

	// perform a sign extension
	val >>= 31 - siz;

	return (uint32_t) ~val;
}

/* NOTE: *start_time must be primed before first run */
uint8_t conditional_delay_ms(uint32_t delay_time, uint32_t *start_time)
{
	uint32_t cur_time = millis();

	// delay complete
	if (cur_time - *start_time >= delay_time) {
		*start_time = cur_time;
		return 1;
	}

	return 0;
}

/* handle a serial error */
uint8_t serial_error_handler()
{
	// don't bother if there's nothing
	if (!Serial.available()) return 0;

	// request data again on error
	if (!check_parity()) {
		flush_serial_buffer();
		Serial.write(PARITY_ERROR);
		return 0;
	}

	return 1;
}

/* populate data_t */
void init_data_t(data_t *in, void *data, size_t siz)
{
	in->data         = data;
	in->siz          = siz;
	in->sent         = 0;
	in->confirm_mask = confirm_mask(siz);
	in->flags        = 0;
}
