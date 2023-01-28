    #pragma once

class LFO {
    public:
    LFO(double phase, const double* rate, const  double* samplerate, const int* waveform) {
        this->rate = rate; // hz
        this->samplerate = samplerate;
        this->waveform = waveform;

        through = phase;
    }
    private:

    double currentOutput = 0.0;
    double currentSampleAndHold = 0.0;
    double lastSampleAndHold = 0.0;

    double soften(double input) {
        double delta = input - currentOutput;
        currentOutput += delta / 10.0;
        return currentOutput;
    }

    double triFunction(double through) {
        if (through > 0.75) {
            double subThrough = through - 0.75;
            return 1.0 - (subThrough * 4.0);
        } else if (through > 0.5) {
            double subThrough = through - 0.5;
            return subThrough * 4.0;
        } else if (through > 0.25) {
            double subThrough = through - 0.25;
            return -1.0 + (subThrough * 4.0);
        }
        else {
            return (through * 4.0) *  -1.0;
        }
    }

    double squareFunction(double through) {
        if (through > 0.5) {
            return -1.0;
        } else {
            return 1.0;
        }
    }


    double randomRampFunction(double through) {
        double diff = currentSampleAndHold - lastSampleAndHold;
        return lastSampleAndHold + (through * diff);
    }

    public:

    double Tick() {
        const double periodSamples = *samplerate / *rate;
        const double delta = 1.0/periodSamples;
        through += delta;
        if (delta > 1.0) {
            // too fast
            return 0.0;
        }
        if (through > 1.0) {
            lastSampleAndHold = currentSampleAndHold;
            currentSampleAndHold = ((double)(rand()%256)-128.0) / 128.0;
            through -= 1.0;
        }

        switch (*waveform) {
            case 0:
                return soften(triFunction(through));
            case 1:
                return soften(squareFunction(through));
            case 2:
                return soften(currentSampleAndHold);
            case 3:
                return soften(randomRampFunction(through));
            default:
                return 0;
        }
    };

    private:
    // config
    const double *samplerate;
    const double *rate;
    const int* waveform;

    // state
    double through;  // 0 > 1
};
