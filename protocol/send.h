/*
 * send.h -- common header for send.cpp
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

#ifndef SEND_H
#define SEND_H

#include <Arduino.h>

#include "types.h"


uint8_t pickup_connection(uint8_t*);
uint8_t send_chunk(data_t*);
uint8_t send_metadata(data_t*, uint8_t*);
void confirm_chunk(data_t*);
void send_packet(data_t*);

#endif /* SEND_H */
