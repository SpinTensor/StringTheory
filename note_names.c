#include <string.h>

//char notenames_sharp[12][3] = {"A ", "A#", "B ", "C ", "C#", "D ", "D#", "E ", "F ", "F#", "G ", "G#"};
//char notenames_flat[12][3] = {"A ", "Bb", "B ", "C ", "Db", "D ", "Eb", "E ", "F ", "Gb", "G ", "Ab"};
char notenames_sharp[12][3] = {"C ", "C#", "D ", "D#", "E ", "F ", "F#", "G ", "G#", "A ", "A#", "B "};
char notenames_flat[12][3]  = {"C ", "Db", "D ", "Eb", "E ", "F ", "Gb", "G ", "Ab", "A ", "Bb", "B "};

int note_name_idx(char* name) {
   for (int inote=0; inote<12; inote++) {
      if (!strcmp(notenames_sharp[inote], name) || !strcmp(notenames_flat[inote], name)) {
         return inote;
      }
   }
   return -1;
}

void flip_note_name(char* name) {
   if (name[1] != '#' && name[1] != 'b') {
      return;
   }

   int idx = note_name_idx(name);
   if (name[1] == '#') {
      strcpy(name, notenames_flat[idx]);
   } else {
      strcpy(name, notenames_sharp[idx]);
   }
   return;
}

void next_note_name(char* name) {
   int idx = note_name_idx(name);
   if (idx == -1) {return;}
   idx++;
   idx %= 12;
   strcpy(name, notenames_sharp[idx]);
   return;
}

void prev_note_name(char* name) {
   int idx = note_name_idx(name);
   if (idx == -1) {return;}
   idx--;
   idx = (idx%12+12)%12;
   strcpy(name, notenames_flat[idx]);
   return;
}
