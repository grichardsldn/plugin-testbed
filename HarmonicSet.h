#pragma once

#define NUM_HARMONICS (20)

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

    double Tick(double samplerate, double freq, double cutoff) {
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
            if ((harmonicFreq< cutoff)  && (harmonicFreq > 50.0)){
                output += (sin(this->through * harmonicNumber * twopi) * this->volumes[i]);
            }
            
        }
        return output;
    }
};
