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
#include "send.h"
#include "types.h"


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

/* populate data_t */
void init_data_t(data_t *in, void *data, size_t siz)
{
	in->data         = data;
	in->siz          = siz;
	in->sent         = 0;
	in->confirm_mask = confirm_mask(siz);
	in->flags        = 0;
}
