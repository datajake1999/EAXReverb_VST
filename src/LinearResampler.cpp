/*
Linear resampler
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

/*
Linear resampling algorithm taken from Nuked OPL3 emulator:
https://github.com/chocolate-doom/chocolate-doom/blob/master/opl/opl3.c
https://github.com/chocolate-doom/chocolate-doom/blob/master/opl/opl3.h
*/

#include <stdlib.h>
#include <string.h>
#include "LinearResampler.h"
#define RSM_FRAC    10

typedef struct _linearresampler
{
	signed long rateratio;
	signed long samplecnt;
	signed short oldsamples[2];
	signed short samples[2];
} linearresampler;

void * LinearResamplerCreate()
{
	linearresampler *lr = (linearresampler *) malloc(sizeof(linearresampler));
	if (lr) LinearResamplerReset(lr);
	return lr;
}

void * LinearResamplerDuplicate(void *_lr)
{
	linearresampler * lr = (linearresampler *)_lr;
	linearresampler *t = (linearresampler *) malloc(sizeof(linearresampler));
	if (lr && t)
	{
		memcpy(t, lr, sizeof(linearresampler));
	}
	else if (t)
	{
		LinearResamplerReset(t);
	}
	return t;
}

void LinearResamplerDestroy(void *lr)
{
	free(lr);
}

void LinearResamplerReset(void *_lr)
{
	linearresampler * lr = (linearresampler *)_lr;
	lr->rateratio = 1024;
	lr->samplecnt = 0;
	lr->oldsamples[0] = 0;
	lr->oldsamples[1] = 0;
	lr->samples[0] = 0;
	lr->samples[1] = 0;
}

void LinearResamplerSetup(void *_lr, unsigned int srcrate, unsigned int dstrate)
{
	linearresampler * lr = (linearresampler *)_lr;
	lr->rateratio = (dstrate << RSM_FRAC) / srcrate;
}

void LinearResamplerProcess(void *_lr, signed short *buffer_in, unsigned int length_in, signed short *buffer_out, unsigned int length_out)
{
	linearresampler * lr = (linearresampler *)_lr;
	unsigned int samplecount = 0;
	unsigned int i;
	for(i = 0; i < length_out; i++)
	{
		while (lr->samplecnt >= lr->rateratio)
		{
			lr->oldsamples[0] = lr->samples[0];
			lr->oldsamples[1] = lr->samples[1];
			if (samplecount >= length_in*2)
			{
				lr->samples[0] = lr->oldsamples[0];
				lr->samples[1] = lr->oldsamples[1];
			}
			else
			{
				lr->samples[0] = buffer_in[samplecount];
				lr->samples[1] = buffer_in[samplecount+1];
				samplecount += 2;
			}
			lr->samplecnt -= lr->rateratio;
		}
		buffer_out[0] = (signed short)((lr->oldsamples[0] * (lr->rateratio - lr->samplecnt)
		+ lr->samples[0] * lr->samplecnt) / lr->rateratio);
		buffer_out[1] = (signed short)((lr->oldsamples[1] * (lr->rateratio - lr->samplecnt)
		+ lr->samples[1] * lr->samplecnt) / lr->rateratio);
		lr->samplecnt += 1 << RSM_FRAC;
		buffer_out += 2;
	}
}
