#include <stdio.h>
#include "notes.h"

int main(){
   note_t note = reference_Note(443.0, just);
   while (note.octave < 8) {note = next_note(note);}

   while (note.octave >= 0) {
      print_note(note);
      note = prev_note(note);
   }
   return 0;
}
