#pragma once

#include "TestbenchChannel.h"

#define NUM_CHANNELS (8)
class TestbenchDSP {
    public:
    const TestbenchConfig* config;

    private:
    //components
    TestbenchChannel* channels[8];

    double modWheel = 0.0;
    double bendWheel = 0.0;

    // state
    double currentNote = -1;

    public:
    // methods
    TestbenchDSP(const TestbenchConfig* config) {
        this->config = config;


        for (int i = 0; i< 8; i++) {
            channels[i] = new TestbenchChannel(config, &modWheel, &bendWheel);
        }
    }
    void ModWheel(double value) {
        modWheel = value;
    }
    void BendWheel(double value) {
        bendWheel = value;
    }
    void NoteOn(int note, int velocity) {
        if (currentNote == -1) {
            currentNote = note;
        }

        int poly = 0;
        if (poly == 1) {
            channels[0]->Trigger(note, (double)velocity / 127.0, currentNote);
            currentNote = note;
        } else {
            TestbenchChannel *existingChannel = findNote(note);
            if (existingChannel != NULL) {
                existingChannel->Trigger(note, (double)velocity/127.0, currentNote);
                currentNote = note;
                return;
            }
            TestbenchChannel *channel = findChannel(idle);
            if (channel == NULL) {
                channel = findChannel(release);
            }
            if (channel == NULL) {
                channel = findChannel(sustain);
            }
            if (channel == NULL) {
                channel = findChannel(decay);
            }
            if (channel == NULL) {
                channel = findChannel(attack);
            }
            if (channel != NULL) {
                channel->Trigger(note, (double)velocity / 127.0, currentNote);
                currentNote = note;
            }
            return;
        }
    };

    void NoteOff(int note) {
        int poly=1;
        if (poly == 1) {
            if (channels[0]->getNote() == note) {
                channels[0]->Release();
            }
        } else {
            TestbenchChannel *channel = findNote(note);
            if (channel != NULL) {
                channel->Release();
            }
        }
        
    }

    iplug::sample Tick() {
      int poly = 0;
        iplug::sample output = 0.0;
        int numChannels = (poly == 1) ? 1:8;
        for (int i=0;i<numChannels;i++) {
            output += channels[i]->Tick();
        }
        return output;
    };

    private:
    TestbenchChannel* findChannel(State state) {
      for (int i = 0 ; i<NUM_CHANNELS;i++) {
            if (channels[i]->getState() == state) {
                return channels[i];
            }
        }
        return NULL;  
    }
    TestbenchChannel* findNote(int note) {
      for (int i = 0 ; i<NUM_CHANNELS;i++) {
            if (channels[i]->getNote() == note) {
                return channels[i];
            }
        }
        return NULL;
    }
    bool anyChannelGateOn() {
        for (int i = 0 ; i<NUM_CHANNELS;i++) {
            int state = channels[i]->getState();

            if (state != idle && state != release) {
                return true;
            }
        }
        return false;
    }
};
