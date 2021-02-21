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

	if (in->sent & PARITY_ERROR) {
		Serial.write(RESEND_METADATA);
		in->flags &= ~PARITY_ERROR;
	} else Serial.write(SENDING_CHUNK);

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

	static uint8_t control_byte;
	static uint8_t wait_byte_cnt;

	if (!(in->flags & REPLY_WAIT)) {
		if (!Serial.available()) return;

		// check if the initial control byte is bad
		if (UCSR0A & _BV(UPE0)) goto error;

		control_byte = Serial.read();

		if (control_byte & RESEND_METADATA) goto error;

		if (control_byte & NO_PARITY_ERROR) {
			in->flags |= CHUNK_CONFIRMED;
			return;
		}

		wait_byte_cnt = __builtin_popcount(control_byte);
		in->flags |= REPLY_WAIT;
	} else if (Serial.available() < wait_byte_cnt) return;

	/*
	 * Since the ATmega328P is little endian, tracer index zero corresponds
	 * with the LSB or the control byte's index.
	 */

	uint8_t *tracer = (uint8_t*) &in->sent;

	for (uint8_t i = 1; i < 5; i++) {
		if (control_byte & _BV(i)) {
			if (UCSR0A & _BV(UPE0)) {
				goto error;
			} else {
				*tracer &= Serial.read();
				++tracer;
			}
		}
	}

	in->flags &= ~(PACKETS_SENT | REPLY_WAIT);
	return;

error:
	while (Serial.available()) Serial.read();  // flush receive buffer
	in->flags &= ~REPLY_WAIT;
	in->flags |=  PARITY_ERROR;
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
