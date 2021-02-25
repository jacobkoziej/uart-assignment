/*
 * receive.cpp -- receiving functions for communication protocol
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
#include "receive.h"
#include "signals.h"


uint8_t begin_connection(uint8_t *com_flags, uint32_t timeout, uint32_t *time)
{
	// why are you trying to reconnect??
	if (*com_flags & CONNECTED) return 1;

	if (*com_flags & WAITING) {
		// clear wait flag after timeout
		if (conditional_delay_ms(timeout, time))
			*com_flags &= ~WAITING;

		// don't bother if there's nothing
		if (!Serial.available()) return 0;

		// request data again on error
		if (!check_parity()) {
			flush_serial_buffer();
			Serial.write(PARITY_ERROR);
			return 0;
		}

		uint8_t byte_in = Serial.read();

		switch (byte_in) {
			case GOOD_CONNECTION:
				Serial.write(GOOD_CONNECTION);
				*com_flags &= ~WAITING;
				*com_flags |= CONNECTED;
				return 1;

			// slave device didn't get our last message
			case PARITY_ERROR:
				Serial.write(GOOD_CONNECTION);
				break;

			default:
				Serial.write(UNKOWN_MESSAGE);
				break;
		}

		return 0;
	}

	// request connection
	Serial.write(BEGIN);
	*com_flags |= WAITING;
	return 0;
}
