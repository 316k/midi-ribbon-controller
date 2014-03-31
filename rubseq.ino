#include <MIDI.h>
#include <math.h>
#include "moving_average.h"

// #define DEBUG

#define     FORCE_SENSOR_PIN 0
#define     POTENTIOMETER_PIN 5
#define 	MIDI_PITCHBEND_MIN   -8192.0
#define 	MIDI_PITCHBEND_MAX   8191.0

// Number of notes in range
#define     RANGE       24

// TODO : read the midi channel from a physical source
#define     MIDI_CHANNEL 1

char last_midi_note = 0;
char last_midi_pitch = 0;
char last_midi_velocity = 0;
int velocity;
int potentiometer_reading;
MovingAverage potentiometer_reading_average;
char note;
int pitch;
char octave = 4;

void setup() {
    #ifdef DEBUG
    Serial.begin(9600);
    #else
    MIDI.begin();
    // All Notes Off
    MIDI.sendControlChange(123, 0, MIDI_CHANNEL);
    #endif
}

void loop() {
    velocity = map(analogRead(FORCE_SENSOR_PIN), 0, 1000, 0, 127);
    velocity = velocity > 4 ? velocity : 0; // Force sensor uncertainty

    potentiometer_reading = analogRead(POTENTIOMETER_PIN);

    if(!potentiometer_reading) {
        return;
    }

    potentiometer_reading_average.add(potentiometer_reading);
    potentiometer_reading = potentiometer_reading_average.average();

    note = potentiometer_reading * RANGE / 1023;
    pitch = 127 / 2 * (potentiometer_reading * (RANGE / 1023.0) - note) + 127 / 2;

    // Transpose on the right octave
    note += 12 * octave;

    if(velocity && pitch != last_midi_pitch) {
        #ifndef DEBUG
        MIDI.sendPitchBend((double) pitch/127.0, MIDI_CHANNEL);

        /* This is retarded. In theory, we don't need this line of code but
         * there seems to be a bug in the MIDI library.
         * Whatever... now it works.
         */
        MIDI.sendControlChange(0, 0, 1);
        #endif

        last_midi_pitch = pitch;
    }

    if(velocity && note != last_midi_note) {
        #ifndef DEBUG
        // Play new note
        MIDI.sendNoteOn(note, velocity, MIDI_CHANNEL);

        // Stop last note
        MIDI.sendNoteOff(last_midi_note, 0, MIDI_CHANNEL);
        #endif

        // Update last stuff
        last_midi_note = note;
        last_midi_velocity = velocity;
    } else if(velocity != last_midi_velocity) { // Only change the velocity if the note hasn't been updated
        // Aftertouch
        #ifndef DEBUG
        MIDI.sendPolyPressure(last_midi_note, velocity, MIDI_CHANNEL);
        MIDI.sendControlChange(0, 0, 1);
        #endif

        last_midi_velocity = velocity;
    } else if(!velocity && last_midi_note != -1) {
        #ifndef DEBUG
        MIDI.sendNoteOff(last_midi_note, 0, MIDI_CHANNEL);
        #endif

        last_midi_note = -1;
        last_midi_velocity = 0;
    }

    #ifdef DEBUG
    Serial.print("Playing note #");
    Serial.print((int) last_midi_note);
    Serial.print(" with pitch ");
    Serial.print((int) last_midi_pitch);
    Serial.print(" with velocity ");
    Serial.println((int) last_midi_velocity);
    #endif

    delay(40);
}

