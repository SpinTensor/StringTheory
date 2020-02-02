#ifndef NOTES_H
#define NOTES_H

   typedef enum tuning_system_type { 
      just,
      equal
   } tuning_sys_t;
   
   typedef struct note_type {
      char name[3];
      char sharpname[3];
      char flatname[3];
      char basenote[3];
      int  octave;
      double freq;
      tuning_sys_t tuning;
   } note_t;
   
   note_t reference_Note (double pitchA, tuning_sys_t tuning);
   void print_note(note_t note);
   note_t next_note(note_t old_note);
   note_t prev_note(note_t old_note);
   double equal_frequency_scale(note_t old_note, note_t new_note);
   double just_frequency_scale(note_t old_note, note_t new_note);

#endif
