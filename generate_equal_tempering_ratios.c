#include <stdio.h>
#include <math.h>

int main() {

   printf("#ifndef EQAL_TEMPERED_RATIOS_H\n");
   printf("#define EQAL_TEMPERED_RATIOS_H\n\n");
   printf("   //Frequency ratios in the equal tempered system\n");

   double P1 = pow(2.0,  0.0 / 12.0); // Perfect unison
   double m2 = pow(2.0,  1.0 / 12.0); // Minor second
   double M2 = pow(2.0,  2.0 / 12.0); // Major second
   double m3 = pow(2.0,  3.0 / 12.0); // Minor third
   double M3 = pow(2.0,  4.0 / 12.0); // Major third
   double P4 = pow(2.0,  5.0 / 12.0); // Fourth
   double A4 = pow(2.0,  6.0 / 12.0); // Augmented fourth
   double P5 = pow(2.0,  7.0 / 12.0); // Fifth
   double m6 = pow(2.0,  8.0 / 12.0); // Minor sixth
   double M6 = pow(2.0,  9.0 / 12.0); // Major sixth
   double m7 = pow(2.0, 10.0 / 12.0); // Minor seventh
   double M7 = pow(2.0, 11.0 / 12.0); // Major seventh
   double P8 = pow(2.0, 12.0 / 12.0); // Perfect octave

   printf("   const double P1 = %20.17lf; // (2^( 0/12)) = Perfect unison\n",   P1);
   printf("   const double m2 = %20.17lf; // (2^( 1/12)) = Minor second\n",     m2);
   printf("   const double M2 = %20.17lf; // (2^( 2/12)) = Major second\n",     M2);
   printf("   const double m3 = %20.17lf; // (2^( 3/12)) = Minor third\n",      m3);
   printf("   const double M3 = %20.17lf; // (2^( 4/12)) = Major third\n",      M3);
   printf("   const double P4 = %20.17lf; // (2^( 5/12)) = Fourth\n",           P4);
   printf("   const double A4 = %20.17lf; // (2^( 6/12)) = Augmented fourth\n", A4);
   printf("   const double P5 = %20.17lf; // (2^( 7/12)) = Fifth\n",            P5);
   printf("   const double m6 = %20.17lf; // (2^( 8/12)) = Minor sixth\n",      m6);
   printf("   const double M6 = %20.17lf; // (2^( 9/12)) = Major sixth\n",      M6);
   printf("   const double m7 = %20.17lf; // (2^(10/12)) = Minor seventh\n",    m7);
   printf("   const double M7 = %20.17lf; // (2^(11/12)) = Major seventh\n",    M7);
   printf("   const double P8 = %20.17lf; // (2^(12/12)) = Perfect octave\n",   P8);
   printf("\n");

   printf("   double ratios[13] = {P1, m2, M2, m3, M3, P4, A4, P5, m6, M6, m7, M7, P8};\n");
   printf("\n");

   printf("#endif\n");

   return 0;
}
