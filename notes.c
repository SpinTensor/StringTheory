#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "note_names.h"
#include "notes.h"


note_t reference_Note (double pitchA, tuning_sys_t tuning) {
   note_t refNote;
   strcpy(refNote.name, "A ");
   strcpy(refNote.sharpname, "A ");
   strcpy(refNote.flatname, "A ");
   strcpy(refNote.basenote, "A ");
   refNote.octave = 4;
   refNote.freq = pitchA;
   refNote.tuning = tuning;

   return refNote;
}

note_t next_note(note_t old_note) {
   note_t new_note = old_note;
   next_note_name(new_note.name);
   strcpy(new_note.sharpname, new_note.name);
   strcpy(new_note.flatname, new_note.name);
   flip_note_name(new_note.flatname);
   if (strcmp(old_note.sharpname, "B ") == 0) {new_note.octave++;}
   switch (new_note.tuning) {
      case equal:
            new_note.freq = old_note.freq * equal_frequency_scale(old_note, new_note);
         break;
      case just:
            new_note.freq = old_note.freq * just_frequency_scale(old_note, new_note);
         break;
   }

   return new_note;
}

note_t prev_note(note_t old_note) {
   note_t new_note = old_note;
   prev_note_name(new_note.name);
   strcpy(new_note.sharpname, new_note.name);
   strcpy(new_note.flatname, new_note.name);
   flip_note_name(new_note.sharpname);
   if (strcmp(old_note.sharpname, "C ") == 0) {new_note.octave--;}
   switch (new_note.tuning) {
      case equal:
            new_note.freq = old_note.freq * equal_frequency_scale(old_note, new_note);
         break;
      case just:
            new_note.freq = old_note.freq * just_frequency_scale(old_note, new_note);
         break;
   }

   return new_note;
}

void print_note(note_t note) {
   printf("Note: %s (%s, %s) %d %f\n", note.name, note.sharpname, note.flatname, note.octave, note.freq);
   return;
}

double equal_frequency_scale(note_t old_note, note_t new_note) {
   #include "equal_tempered_ratios.h"

   double frequency = old_note.freq;

   // get note indices
   int ref_idx = note_name_idx(old_note.basenote);
   int old_idx = note_name_idx(old_note.name);
   int new_idx = note_name_idx(new_note.name);
   int tidx;

   // direction of frequency scaling
   int dir = 12*(new_note.octave-old_note.octave)+new_idx-old_idx;
   if (dir > 0) {
      // scale down to next lower (or equal) base note
      tidx = old_idx-ref_idx;
      tidx = (tidx%12+12)%12;
      frequency /= ratios[tidx];

      // scaling up in frequency
      if (new_idx == ref_idx) {
         // new note is the reference note (at the moment the frequency is an octave to low)
         tidx = 12;
      } else {
         tidx = new_idx-ref_idx;
         tidx = (tidx%12+12)%12;
      }
      frequency *= ratios[tidx];

   } else if (dir < 0) {
      // scaling down to next lower base note
      if (old_idx == ref_idx) {
         tidx = 12;
      } else {
         tidx = old_idx-ref_idx;
         tidx = (tidx%12+12)%12;
      }

      frequency /= ratios[tidx];

      // scale up in frequency
      tidx = new_idx-ref_idx;
      tidx = (tidx%12+12)%12;

      frequency *= ratios[tidx];
   }else {
      return 1.0;
   }

   return frequency / old_note.freq;
}

double just_frequency_scale(note_t old_note, note_t new_note) {
   #include "just_tempered_ratios.h"

   double frequency = old_note.freq;

   // get note indices
   int ref_idx = note_name_idx(old_note.basenote);
   int old_idx = note_name_idx(old_note.name);
   int new_idx = note_name_idx(new_note.name);
   int tidx;

   // direction of frequency scaling
   int dir = 12*(new_note.octave-old_note.octave)+new_idx-old_idx;
   if (dir > 0) {
      // scale down to next lower (or equal) base note
      tidx = old_idx-ref_idx;
      tidx = (tidx%12+12)%12;
      frequency /= ratios[tidx];

      // scaling up in frequency
      if (new_idx == ref_idx) {
         // new note is the reference note (at the moment the frequency is an octave to low)
         tidx = 12;
      } else {
         tidx = new_idx-ref_idx;
         tidx = (tidx%12+12)%12;
      }
      frequency *= ratios[tidx];

   } else if (dir < 0) {
      // scaling down to next lower base note
      if (old_idx == ref_idx) {
         tidx = 12;
      } else {
         tidx = old_idx-ref_idx;
         tidx = (tidx%12+12)%12;
      }

      frequency /= ratios[tidx];

      // scale up in frequency
      tidx = new_idx-ref_idx;
      tidx = (tidx%12+12)%12;

      frequency *= ratios[tidx];
   }else {
      return 1.0;
   }

   return frequency / old_note.freq;
}

