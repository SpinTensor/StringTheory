#include <stdio.h>

int main() {

   printf("#ifndef JUST_TEMPERED_RATIOS_H\n");
   printf("#define JUST_TEMPERED_RATIOS_H\n\n");
   printf("   //Frequency ratios in the just tempered system\n");

   double P1 =  1.0 /  1.0; // Perfect unison
   double m2 = 16.0 / 15.0; // Minor second
   double M2 =  9.0 /  8.0; // Major second
   double m3 =  6.0 /  5.0; // Minor third
   double M3 =  5.0 /  4.0; // Major third
   double P4 =  4.0 /  3.0; // Perfect fourth
   double A4 =  7.0 /  5.0; // Augmented fourth
   double P5 =  3.0 /  2.0; // Perfect fifth
   double m6 =  8.0 /  5.0; // Minor sixth
   double M6 =  5.0 /  3.0; // Major sixth
   double m7 =  9.0 /  5.0; // Minor seventh
   double M7 = 15.0 /  8.0; // Major seventh
   double P8 =  2.0 /  1.0; // Perfect octave

   printf("   const double P1 = %20.17lf; // ( 1 /  1) = Perfect unison\n",   P1);
   printf("   const double m2 = %20.17lf; // (16 / 15) = Minor second\n",     m2);
   printf("   const double M2 = %20.17lf; // ( 9 /  8) = Major second\n",     M2);
   printf("   const double m3 = %20.17lf; // ( 6 /  5) = Minor third\n",      m3);
   printf("   const double M3 = %20.17lf; // ( 5 /  4) = Major third\n",      M3);
   printf("   const double P4 = %20.17lf; // ( 4 /  3) = Perfect fourth\n",   P4);
   printf("   const double A4 = %20.17lf; // ( 7 /  5) = Augmented fourth\n", A4);
   printf("   const double P5 = %20.17lf; // ( 3 /  2) = Perfect fifth\n",    P5);
   printf("   const double m6 = %20.17lf; // ( 8 /  5) = Minor sixth\n",      m6);
   printf("   const double M6 = %20.17lf; // ( 5 /  3) = Major sixth\n",      M6);
   printf("   const double m7 = %20.17lf; // ( 9 /  5) = Minor seventh\n",    m7);
   printf("   const double M7 = %20.17lf; // (15 /  8) = Major seventh\n",    M7);
   printf("   const double P8 = %20.17lf; // ( 2 /  1) = Perfect octave\n",   P8);
   printf("\n");

   printf("   double ratios[13] = {P1, m2, M2, m3, M3, P4, A4, P5, m6, M6, m7, M7, P8};\n");
   printf("\n");

   printf("#endif\n");

   return 0;
}
