#pragma once

#include "TestbenchConfig.h"

class TestbenchDSP {
    public:
    const TestbenchConfig* config;

    private:

    // state
    double currentNote = -1;

    // application state
    double left = 0.0;
    bool noteOn = false;

    public:
    // methods
    TestbenchDSP(const TestbenchConfig* config) {
        this->config = config;
    }
    void ModWheel(double value) {
    }
    void BendWheel(double value) {
    }

    void Trigger( int note, int velocity ) {

    }

    void Release() {

    }

    void NoteOn(int note, int velocity) {
        currentNote = note;
        Trigger(note, velocity);
    };

    void NoteOff(int note) {
        if (note == currentNote) {
            Release();
            currentNote = -1;
        }
    }

    double Tick() {
        double delta = this->currentNote / 1000.0;

        this->left += delta;
        if (this->left > 1.0) {
            this->left = 0.0;
        }
        if (currentNote != -1 ) {
            return this->left > config->paramA? (0.1 * config->volume) :(-0.1 * config->volume) ;
        } else {
            return 0.0;
        }
        
    }
    private:

};
