#pragma once

#include "TestbenchConfig.h"
#include "math.h"

enum State { idle, attack, decay, sustain, release };

class TestbenchChannel {
    private:
    // inputs
    const double *modWheel;
    const double *bendWheel;
    const TestbenchConfig *config;

    // testbench state
    int noteNumber;
    double note;
    double velocity = 0.0;

    // application state
    double left = 0.0;
    bool noteOn = false;

    public:
    TestbenchChannel(const TestbenchConfig *config, const double *modWheel, const double *bendWheel) {
        this->config = config;
        this->modWheel = modWheel;
        this->bendWheel = bendWheel;
    };

    int getNote() {
        return noteNumber;
    }

    State getState() {
        return (this->noteOn == true) ? attack : idle;
    }

    void Trigger( int note, double velocity, int startingNote ) {
        this->noteNumber = note;
        this->velocity = velocity;

        // running 
        this->noteOn = true;
    };
    void Release() {
        this->noteOn = false;
    };

    double Tick() {
        double delta = this->noteNumber / 1000.0;

        this->left += delta;
        if (this->left > 1.0) {
            this->left = 0.0;
        }
        if (noteOn == true) {
            return this->left > 0.5 ? 0.1:-0.1;
        } else {
            return 0.0;
        }
        
    }
};
