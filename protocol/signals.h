/*
 * signals.h -- signals and flags for communication protocol
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

#ifndef SIGNALS_H
#define SIGNALS_H

#include <Arduino.h>


// metadata
#define BEGIN 0xFF
#define DATA_SIZE 0xFE
#define PARITY_ERROR 0xFD
#define SENDING_CHUNK 0xFC
#define GOOD_CONNECTION 0xFB
#define UNKOWN_MESSAGE 0xFA

// data_t.flags
#define CHUNK_CONFIRMED _BV(0)
#define PACKETS_SENT    _BV(1)
#define META_SENT       _BV(2)
#define REPLY_WAIT      _BV(3)
#define D_PARITY_ERROR  _BV(4)

// receive flags
#define NO_PARITY_ERROR _BV(0)
#define RESEND_METADATA _BV(7)

// global communication flags
#define MASTER    _BV(0)
#define SLAVE     _BV(1)
#define WAITING   _BV(6)
#define CONNECTED _BV(7)

#endif /* SIGNALS_H */
