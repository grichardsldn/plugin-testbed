#pragma once

#include <math.h>
#define NUM_HARMONICS (40)

double reduceGain(double value, double attenuation) {
   return value * pow(10.0, -1.0 * attenuation / 20.0);
}

// for resonant filter, have a notch filter with a floor level for below notch freq
// (or possibly another way round, to avoid calculation when below the resonant freq)

// rollOff in db/octave
double filter(double inFreqHz, double inVol, double freqOctave, double rollOff, double resOctaves) {
    double frequency = 27.5 * pow(2, freqOctave);
    double inFreqOctave = log2( inFreqHz/ 27.5);
    if (inFreqOctave < freqOctave ) {
        double diffOctaves = freqOctave - inFreqOctave;
        if (diffOctaves > resOctaves) {
            diffOctaves = resOctaves;
        }
        return reduceGain(inVol, (diffOctaves * rollOff));
    } else {
        double diffOctaves = inFreqOctave - freqOctave;
        //return 0.0;
        //return inVol / (diffOctaves * rollOff);
        return reduceGain(inVol, (diffOctaves * rollOff));
    }
}

class HarmonicSet {
    public:
  double volumes[NUM_HARMONICS] = {0};
    double harmonics[NUM_HARMONICS] = {0};
    double through = 0;

    HarmonicSet() {
    }

    void SetSquare(double markSpaceRatio) {

        for (int i = 0 ; i < NUM_HARMONICS; i++) {
            const int harmonic = i+1;
            this->harmonics[i] = harmonic;
            // fabs give the square wave
            this->volumes[i] = fabs(2.0 / (harmonic) * sin(harmonic * M_PI * markSpaceRatio));
           
            // if (harmonic % 2 == 0) {
            //     this->volumes[i] = 0.0;
            //     this->harmonics[i] = harmonic;
            // } else {
            //     this->volumes[i] = 1.0 / harmonic;
            // }
        }
    }

    void SetNoise() {
        for (int i = 0 ; i < NUM_HARMONICS; i++) {
            const int harmonic = i+1;
            double a = 440; //frequency of A (coomon value is 440Hz)
            double hz = (a) * pow(2, (((i) - 69.0) / 12.0));
            this->harmonics[i] = (double)((rand()%10)+50.0 + (hz));
            
            this->volumes[i] = 0.9;
        }
    }

    void Shift() {

        int i,h;
        i = (rand()>>2)%NUM_HARMONICS;
        h = this->harmonics[i];
        h *= 0.999995;
        this->harmonics[i]=h;
        i = (rand()>>2)%NUM_HARMONICS;
        h = this->harmonics[i];
        h *= 1.000005;
        this->harmonics[i]=h;
    }

    double Tick(double samplerate, double freq, double cutoff, double rollOff, double resOctaves) {
        const double twopi = 3.14159 * 2.0;
        const double delta = freq / samplerate;
        this->through += delta;
        if (this->through > 1.0) {
            this->through -= 1.0;
        }
        double output = 0.0;
        for (int i = 0 ; i < (NUM_HARMONICS); i++) {
            const int harmonicNumber = this->harmonics[i];
            const double harmonicFreq = freq * harmonicNumber;

            double oscVal = sin(this->through * harmonicNumber * twopi);

             output += oscVal * filter(harmonicFreq, this->volumes[i], cutoff, rollOff, resOctaves);   
        }
        return output;
    }
};
