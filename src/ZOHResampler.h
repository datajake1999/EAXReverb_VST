/*
Interface for zero-order hold resampler
Copyright (C) 2020 Datajake

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef ZOH_RESAMPLER_H
#define ZOH_RESAMPLER_H

#ifdef __cplusplus
extern "C" {
#endif
	void * ZOHResamplerCreate();
	void * ZOHResamplerDuplicate(void *);
	void ZOHResamplerDestroy(void *);
	void ZOHResamplerReset(void *);
	void ZOHResamplerSetup(void *, unsigned int srcrate, unsigned int dstrate);
	void ZOHResamplerProcess(void *, signed short *buffer_in, unsigned int length_in, signed short *buffer_out, unsigned int length_out);
#ifdef __cplusplus
}
#endif

#endif //ZOH_RESAMPLER_H