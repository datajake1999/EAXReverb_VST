/*
Interface for linear resampler
Copyright (C) 2019 Datajake

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

#ifndef LINEAR_RESAMPLER_H
#define LINEAR_RESAMPLER_H

#ifdef __cplusplus
extern "C" {
#endif
	void * LinearResamplerCreate();
	void LinearResamplerDestroy(void *);
	void LinearResamplerSetup(void *, unsigned int srcrate, unsigned int dstrate);
	void LinearResamplerProcess(void *, signed short *buffer_in, unsigned int length_in, signed short *buffer_out, unsigned int length_out);
#ifdef __cplusplus
}
#endif

#endif //LINEAR_RESAMPLER_H