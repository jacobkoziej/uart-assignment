/*
 * common.h -- common header for config.cpp
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

#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>

#include "types.h"

#define _BVUL(bit) (1UL << (bit))


uint32_t confirm_mask(size_t);
uint8_t conditional_delay_ms(uint32_t, uint32_t*);
void init_data_t(data_t*, void*, size_t);

#endif /* COMMON_H */
