/*
 * send.cpp -- sending functions for communication protocol
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


/* pickup a connection request from slave device */
uint8_t pickup_connection(uint8_t *com_flags)
{
	// why are you trying to reconnect??
	if (*com_flags & CONNECTED) return 1;

	// don't bother if there's nothing
	if (!serial_error_handler()) return 0;

	uint8_t byte_in = Serial.read();

	if (*com_flags & WAITING) {
		switch (byte_in) {
			case GOOD_CONNECTION:
				*com_flags &= ~WAITING;
				*com_flags |= CONNECTED;
				return 1;

			// slave device didn't get our last message
			case BEGIN:
			case PARITY_ERROR:
				Serial.write(GOOD_CONNECTION);
				break;

			default:
				Serial.write(UNKOWN_MESSAGE);
				break;
		}

		return 0;
	}

	flush_serial_buffer();  // remove any backlogged messages

	if (byte_in == BEGIN) {
		*com_flags |= WAITING;
		Serial.write(GOOD_CONNECTION);
	} else Serial.write(UNKOWN_MESSAGE);

	return 0;
}

/* send up to a 32-byte chunk of data */
uint8_t send_chunk(data_t *in)
{
	if (!(in->flags & PACKETS_SENT)) {
		send_packet(in);
		if (in->sent == in->confirm_mask) in->flags |= PACKETS_SENT;
		return 0;
	}

	confirm_chunk(in);

	if (in->flags & CHUNK_CONFIRMED) return 1;
	if (in->flags & REPLY_WAIT) return 0;

	if (in->sent & D_PARITY_ERROR) {
		Serial.write(RESEND_METADATA);
		in->flags &= ~D_PARITY_ERROR;
	} else Serial.write(SENDING_CHUNK);

	return 0;
}

/* send metadata about the data that's about to be sent */
uint8_t send_metadata(data_t *in, uint8_t *com_flags)
{
	// can't be sending metadata if you're not connected
	if (!(*com_flags & CONNECTED)) return 0;

	if (*com_flags & WAITING) {
		if (!serial_error_handler()) return 0;

		*com_flags &= ~WAITING;

		uint8_t byte_in = Serial.read();

		return (byte_in == ACKNOWLEDGED) ? 1 : 0;
	}

	uint8_t *tracer;

	Serial.write(DATA_SIZE);
	tracer = (uint8_t*) &in->siz;
	Serial.write(tracer[0]);
	Serial.write(tracer[1]);

	*com_flags |= WAITING;
	return 0;
}

/* check if the sent 32-byte chunk had any parity errors */
void confirm_chunk(data_t *in)
{
	/*
	 * In the worst case scenario there will be parity errors in each
	 * unique packet.  Since the first byte's flags specify which packets
	 * are "bad", each consecutive byte specifies the packet's "bad"
	 * byte(s).
	 */

	static uint8_t control_byte  = 0;
	static uint8_t wait_byte_cnt = 0;

	if (!(in->flags & REPLY_WAIT)) {
		if (!Serial.available()) return;

		// check if the initial control byte is bad
		if (!check_parity()) goto error;

		control_byte = Serial.read();

		if (control_byte & RESEND_METADATA) goto error;

		if (control_byte & NO_PARITY_ERROR) {
			in->flags |= CHUNK_CONFIRMED;
			return;
		}

		wait_byte_cnt = __builtin_popcount(control_byte);
		in->flags |= REPLY_WAIT;
		return;
	} else if (Serial.available() < wait_byte_cnt) return;

	/*
	 * Since the ATmega328P is little endian, tracer index zero corresponds
	 * with the LSB or the control byte's index.
	 */

	uint8_t *tracer = (uint8_t*) &in->sent;

	for (uint8_t i = 1; i < 5; i++) {
		if (control_byte & _BV(i)) {
			if (check_parity()) {
				tracer[i - 1] &= Serial.read();
			} else goto error;
		}
	}

	in->flags &= ~(PACKETS_SENT | REPLY_WAIT);
	return;

error:
	flush_serial_buffer();
	in->flags &= ~REPLY_WAIT;
	in->flags |=  D_PARITY_ERROR;
}

/* send up to an 8-byte packet */
void send_packet(data_t *in)
{
	uint8_t *tracer = in->data;
	uint8_t cnt = 0;
	uint8_t i = 0;

	while (i < in->siz && i < 32 && cnt < 8) {
		if (!(in->sent & _BVUL(i))) {
			Serial.write(tracer[i]);
			++cnt;
			in->sent |= _BVUL(i);
		}
		++i;
	}
}
