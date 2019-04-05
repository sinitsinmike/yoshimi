/*
    PADnoteParameters.h - Parameters for PADnote (PADsynth)

    Original ZynAddSubFX author Nasca Octavian Paul
    Copyright (C) 2002-2005 Nasca Octavian Paul
    Copyright 2009, Alan Calvert

    This file is part of yoshimi, which is free software: you can redistribute
    it and/or modify it under the terms of version 2 of the GNU General Public
    License as published by the Free Software Foundation.

    yoshimi is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.   See the GNU General Public License (version 2 or
    later) for more details.

    You should have received a copy of the GNU General Public License along with
    yoshimi; if not, write to the Free Software Foundation, Inc., 51 Franklin
    Street, Fifth Floor, Boston, MA  02110-1301, USA.

    This file is a derivative of the ZynAddSubFX original, modified October 2009
*/

#ifndef PAD_NOTE_PARAMETERS_H
#define PAD_NOTE_PARAMETERS_H

#include "Misc/XMLwrapper.h"
#include "DSP/FFTwrapper.h"
#include "Synth/OscilGen.h"
#include "Synth/Resonance.h"
#include "Misc/Util.h"
#include "Params/EnvelopeParams.h"
#include "Params/LFOParams.h"
#include "Params/FilterParams.h"
#include "Params/Presets.h"

class PADnoteParameters : public Presets
{
    public:
        PADnoteParameters(FFTwrapper *fft_);
        ~PADnoteParameters();

        void defaults(void);
        void add2XML(XMLwrapper *xml);
        void getfromXML(XMLwrapper *xml);

        //returns a value between 0.0-1.0 that represents the estimation
        // perceived bandwidth
        float getprofile(float *smp, int size);

        //parameters

        //the mode: 0 - bandwidth, 1 - discrete (bandwidth=0), 2 - continous
        //the harmonic profile is used only on mode 0
        unsigned char Pmode;

        //Harmonic profile (the frequency distribution of a single harmonic)
        struct {
            struct {    //base function
                unsigned char type;
                unsigned char par1;
            } base;
            unsigned char freqmult; // frequency multiplier of the distribution
            struct {                // the modulator of the distribution
                unsigned char par1;
                unsigned char freq;
            } modulator;

            unsigned char width; // the width of the resulting function after the modulation

            struct { // the amplitude multiplier of the harmonic profile
                unsigned char mode;
                unsigned char type;
                unsigned char par1;
                unsigned char par2;
            } amp;
            bool autoscale;        //  if the scale of the harmonic profile is
                                   // computed automaticaly
            unsigned char onehalf; // what part of the base function is used to
                                   // make the distribution
        } Php;


        unsigned int Pbandwidth; // the values are from 0 to 1000
        unsigned char Pbwscale;  // how the bandwidth is increased according to
                                 // the harmonic's frequency

        struct { // where are positioned the harmonics (on integer multiplier or different places)
            unsigned char type;
            unsigned char par1, par2, par3; // 0..255
        } Phrpos;

        struct { // quality of the samples (how many samples, the length of them,etc.)
            unsigned char samplesize;
            unsigned char basenote, oct, smpoct;
        } Pquality;

        // frequency parameters
        // If the base frequency is fixed to 440 Hz
        unsigned char Pfixedfreq;

        // Equal temperate (this is used only if the Pfixedfreq is enabled)
        // If this parameter is 0, the frequency is fixed (to 440 Hz);
        // if this parameter is 64, 1 MIDI halftone -> 1 frequency halftone
        unsigned char PfixedfreqET;
        unsigned short int PDetune;       // fine detune
        unsigned short int PCoarseDetune; // coarse detune+octave
        unsigned char PDetuneType;        // detune type

        EnvelopeParams *FreqEnvelope; // Frequency Envelope
        LFOParams *FreqLfo;           // Frequency LFO

        // Amplitude parameters
        unsigned char PStereo;
        // Panning -  0 - random
        //            1 - left
        //           64 - center
        //          127 - right */
        unsigned char PPanning;

        unsigned char PVolume;

        unsigned char PAmpVelocityScaleFunction;

        EnvelopeParams *AmpEnvelope;

        LFOParams *AmpLfo;

        unsigned char PPunchStrength, PPunchTime, PPunchStretch, PPunchVelocitySensing;

        // Filter Parameters
        FilterParams *GlobalFilter;

        // filter velocity sensing
        unsigned char PFilterVelocityScale;

        // filter velocity sensing
        unsigned char PFilterVelocityScaleFunction;

        EnvelopeParams *FilterEnvelope;
        LFOParams *FilterLfo;

        float setPbandwidth(int Pbandwidth); // returns the BandWidth in cents
        float getNhr(int n); // gets the n-th overtone position relatively to N harmonic

        void applyparameters(bool islocked);

        OscilGen *oscilgen;
        Resonance *resonance;

        struct {
            int size;
            float basefreq;
            float *smp;
        } sample[PAD_MAX_SAMPLES], newsample;

        boost::object_pool<Envelope> *envPool;
        boost::object_pool<LFO> *lfoPool;
        boost::pool<> *buffPool;

    private:
        void generatespectrum_bandwidthMode(float *spectrum, int size,
                                            float basefreq,
                                            float *profile,
                                            int profilesize,
                                            float bwadjust);
        void generatespectrum_otherModes(float *spectrum, int size,
                                         float basefreq,
                                         float *profile, int profilesize,
                                         float bwadjust);
        void deletesamples(void);
        void deletesample(int n);

        FFTwrapper *fft;
        //pthread_mutex_t *mutex;
};

#endif
