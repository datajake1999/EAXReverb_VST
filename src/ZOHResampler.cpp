/*
Zero-order hold resampler
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

/*
Bassed on LinearResampler.cpp/h
*/

#include <stdlib.h>
#include <string.h>
#include "ZOHResampler.h"
#define RSM_FRAC    10

typedef struct _zohresampler
{
	signed long rateratio;
	signed long samplecnt;
	signed short samples[2];
} zohresampler;

void * ZOHResamplerCreate()
{
	zohresampler *zr = (zohresampler *) malloc(sizeof(zohresampler));
	if (zr) ZOHResamplerReset(zr);
	return zr;
}

void * ZOHResamplerDuplicate(void *_zr)
{
	zohresampler * zr = (zohresampler *)_zr;
	zohresampler *t = (zohresampler *) malloc(sizeof(zohresampler));
	if (zr && t)
	{
		memcpy(t, zr, sizeof(zohresampler));
	}
	else if (t)
	{
		ZOHResamplerReset(t);
	}
	return t;
}

void ZOHResamplerDestroy(void *zr)
{
	free(zr);
}

void ZOHResamplerReset(void *_zr)
{
	zohresampler * zr = (zohresampler *)_zr;
	zr->rateratio = 1024;
	zr->samplecnt = 0;
	zr->samples[0] = 0;
	zr->samples[1] = 0;
}

void ZOHResamplerSetup(void *_zr, unsigned int srcrate, unsigned int dstrate)
{
	zohresampler * zr = (zohresampler *)_zr;
	zr->rateratio = (dstrate << RSM_FRAC) / srcrate;
}

void ZOHResamplerProcess(void *_zr, signed short *buffer_in, unsigned int length_in, signed short *buffer_out, unsigned int length_out)
{
	zohresampler * zr = (zohresampler *)_zr;
	unsigned int samplecount = 0;
	unsigned int i;
	for(i = 0; i < length_out; i++)
	{
		while (zr->samplecnt >= zr->rateratio)
		{
			if (samplecount >= length_in*2)
			{
				zr->samples[0] = zr->samples[0];
				zr->samples[1] = zr->samples[1];
			}
			else
			{
				zr->samples[0] = buffer_in[samplecount];
				zr->samples[1] = buffer_in[samplecount+1];
				samplecount += 2;
			}
			zr->samplecnt -= zr->rateratio;
		}
		buffer_out[0] = zr->samples[0];
		buffer_out[1] = zr->samples[1];
		zr->samplecnt += 1 << RSM_FRAC;
		buffer_out += 2;
	}
}
