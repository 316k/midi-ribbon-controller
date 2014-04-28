#include <MIDI.h>
#include <math.h>
#include "moving_average.h"

// Uncomment this line to dump values through Serial
// #define DEBUG

#define     FORCE_SENSOR_PIN    3
#define     POTENTIOMETER_PIN   5
#define     OCTAVE_PIN          4
#define     MIDI_PITCHBEND_MIN  -8192.0
#define     MIDI_PITCHBEND_MAX  8191.0

#define     LED_1               11
#define     LED_2               10
#define     LED_3               9

/* If the potentiometer reading is greater than
 * MovingAverage plus/minus this value, the average is reset
 */
#define     PORTAMENTO_TOLERENCE    90

// Minimumn value for a valid force sensor reading
#define     FORCE_SENSOR_TOLERENCE  20

// This value is used in indicative_lights()
#define     TUNING_TOLERENCE     4

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
char last_octave = 4;


/* This is retarded. In theory, we don't need this line of code but
 * there seems to be a bug in the MIDI library.
 * Whatever... now it works.
 */
void do_stuff_because_midi_lib_is_broken() {
    MIDI.sendControlChange(0, 0, 1);
}

/**
 * 
 *
 * @param octave The current octave
 */
void indicative_lights(char octave) {

    digitalWrite(LED_1, (octave & 1 ? HIGH : LOW));
    digitalWrite(LED_2, (octave & 2 ? HIGH : LOW));
    digitalWrite(LED_3, (octave & 4 ? HIGH : LOW));

    #ifdef DEBUG
    Serial.print("Current octave & values : ");
    Serial.print((int) octave);
    Serial.print(" ");
    Serial.print( ((octave & 1)));
    Serial.print(" ");
    Serial.print((int) ((octave & 2)));
    Serial.print(" ");
    Serial.print((int) ((octave & 4)));
    Serial.print(" ");
    #endif
}

void setup() {
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    pinMode(LED_3, OUTPUT);

    indicative_lights(octave);

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
    velocity = velocity > FORCE_SENSOR_TOLERENCE ? velocity : 0; // Force sensor uncertainty

    potentiometer_reading = analogRead(POTENTIOMETER_PIN);

    // Zero values aren't welcomed in MovingAverage class
    if(!potentiometer_reading) {
        return;
    }

    // Read current octave and display on indicative lights
    octave = map(analogRead(OCTAVE_PIN), 0, 1023, 0, 8);

    // Update lights
    if(octave != last_octave) {
        indicative_lights(octave);
        last_octave = octave;
    }

    if(potentiometer_reading > potentiometer_reading_average.average() + PORTAMENTO_TOLERENCE ||
       potentiometer_reading < potentiometer_reading_average.average() - PORTAMENTO_TOLERENCE) {
        potentiometer_reading_average.reset();
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

        do_stuff_because_midi_lib_is_broken();
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
        do_stuff_because_midi_lib_is_broken();
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
    Serial.print((int) last_midi_velocity);
    Serial.print(" (based on reading value : ");
    Serial.print((int) potentiometer_reading);
    Serial.println(")");
    #endif

    delay(10);
}

