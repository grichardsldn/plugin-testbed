#pragma once

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

    double through = 0;

    HarmonicSet() {
    }

    void SetSquare() {
        for (int i = 0 ; i < NUM_HARMONICS; i++) {
            const int harmonic = i+1;
            if (harmonic % 2 == 0) {
                this->volumes[i] = 0.0;
            } else {
                this->volumes[i] = 1.0 / harmonic;
            }
        }
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
            const int harmonicNumber = i+1;
            const double harmonicFreq = freq * harmonicNumber;

            double oscVal = sin(this->through * harmonicNumber * twopi);

             output += oscVal * filter(harmonicFreq, this->volumes[i], cutoff, rollOff, resOctaves);   
        }
        return output;
    }
};
