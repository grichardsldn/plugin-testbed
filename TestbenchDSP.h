#pragma once

#include "TestbenchConfig.h"

#include "HarmonicSet.h"

class TestbenchDSP {
    public:
    const TestbenchConfig* config;

    private:

    // state
    double currentNote = -1;

    // application state
    double left = 0.0;
    bool noteOn = false;
    HarmonicSet harmonicSet;

    public:
    // methods
    TestbenchDSP(const TestbenchConfig* config) {
        this->config = config;
        this->harmonicSet.SetSquare();
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
        harmonicSet.through = 3.14;
        Trigger(note, velocity);
    };

    void NoteOff(int note) {
        if (note == currentNote) {
            Release();
            currentNote = -1;
        }
    }

    double OtherTick() {
        return harmonicSet.through / 6.28;
    }

    double Tick() {
        double a = 440; //frequency of A (coomon value is 440Hz)
        double hz = (a) * pow(2, (((currentNote) - 69.0) / 12.0));
        if (currentNote != -1) {
            return harmonicSet.Tick(config->samplerate, hz, config->paramA, config->paramB, config->paramC) * config->volume;
        } else {
            return 0.0;
        }
    }
    private:

};
