/**
 * Helper functions and constants to deal with MIDI signals by Nicolas Hurtubise
 */

#include <math.h>

#define 	MIDI_PITCHBEND_MIN   -8192
#define 	MIDI_PITCHBEND_MAX   8191

// Minimum and maximum MIDI frequencies (in Hz)
#define     MIN_MIDI_FREQ        8
#define     MAX_MIDI_FREQ        12543

/**
 * Returns a round midi note
 * Use midi_pitch_round() to get the pitch correction of a precise frequency
 *
 * @param frequency The frequency (in Hz) to parse
 */
int midi_note_round(long frequency) {
    return 69 + 12*log(frequency/440.0)/log(2);
}

/**
 * Returns a midi pitch
 * Use midi_note_round() to get the midi base note to adjust
 * 
 * @param frequency The frequency (in Hz) to parse
 */
int midi_pitch_round(long frequency) {
    return map(floor((71 + 12 * log((frequency/440.0))/log(2) - floor(69 + 12 * log((frequency/440.0))/log(2))) * 32),
                0, 127, MIDI_PITCHBEND_MIN, MIDI_PITCHBEND_MAX);
}

/**
 * Autotunes a note on a given scale
 * 
 * @param note the midi note
 * @param start the first note of the scale (default : C)
 */
int scale_round(int note, int start = 0) {

    int closest_note = -1;
    int octave = 0;

    int notes[] = {0, 2, 3, 5, 7, 8, 10};
    int number_of_notes = 6;

    note -= start;
    octave = note/12;
    note %= 12;

    for(int i=0; i < number_of_notes && closest_note == -1; i++) {
        if (i == number_of_notes - 1 || note <= notes[i] + (notes[i+1] - notes[i])/2.0) {
            closest_note = notes[i];
        }
    }

    return closest_note + 12 * octave + start;
}
