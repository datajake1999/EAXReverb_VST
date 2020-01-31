// (c) Copyright 2016, Sean Connelly (@voidqk), http://syntheti.cc
// MIT License
// Project Home: https://github.com/voidqk/sndfilter

// biquad filtering based on WebAudio specification:
//   https://webaudio.github.io/web-audio-api/#the-biquadfilternode-interface

#ifndef SNDFILTER_BIQUAD_DOUBLE__H
#define SNDFILTER_BIQUAD_DOUBLE__H

typedef struct {
	double L; // left channel sample
	double R; // right channel sample
} sfd_sample_st;

// biquad filtering is a technique used to perform a variety of sound filters
//
// this API works by first initializing an sfd_biquad_state_st structure, and then using it to
// process a sample in chunks
//
// for example, for a lowpass filter over a stream with 128 samples per chunk, you would do:
//
//   sfd_biquad_state_st lowpass;
//   sfd_lowpass(&lowpass, 44100, 440, 1);
//
//   for each 128 length sample:
//     sfd_biquad_process(&lowpass, 128, input, output);
//
// notice that sfd_biquad_process will change the xn1,xn2,yn1,yn2 values inside of the state
// structure, since these values must be carried over across chunk boundaries
//
// also notice that the choice to divide the sound into chunks of 128 samples is completely
// arbitrary from the filter's perspective

typedef struct {
	double b0;
	double b1;
	double b2;
	double a1;
	double a2;
	sfd_sample_st xn1;
	sfd_sample_st xn2;
	sfd_sample_st yn1;
	sfd_sample_st yn2;
} sfd_biquad_state_st;

// these functions will initialize an sfd_biquad_state_st structure based on the desired filter
void sfd_lowpass  (sfd_biquad_state_st *state, int rate, double cutoff, double resonance);
void sfd_highpass (sfd_biquad_state_st *state, int rate, double cutoff, double resonance);
void sfd_bandpass (sfd_biquad_state_st *state, int rate, double freq, double Q);
void sfd_notch    (sfd_biquad_state_st *state, int rate, double freq, double Q);
void sfd_peaking  (sfd_biquad_state_st *state, int rate, double freq, double Q, double gain);
void sfd_allpass  (sfd_biquad_state_st *state, int rate, double freq, double Q);
void sfd_lowshelf (sfd_biquad_state_st *state, int rate, double freq, double Q, double gain);
void sfd_highshelf(sfd_biquad_state_st *state, int rate, double freq, double Q, double gain);

// this function will process the input sound based on the state passed
// the input and output buffers should be the same size
void sfd_biquad_process(sfd_biquad_state_st *state, int size, sfd_sample_st *input,
	sfd_sample_st *output);

#endif // SNDFILTER_BIQUAD_DOUBLE__H
