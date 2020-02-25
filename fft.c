#include <stdlib.h>

#include "fft.h"
#include "window_functions.h"

fft_t init_fft(int fft_size, double dt) {

   fft_t fftdata;

   fftdata.rawsize = fft_size;
   fftdata.rawdata = (double*)malloc(fftdata.rawsize*sizeof(double));

   // Prepare fourier transform
   // is twice as big as the audio buffer due to zero padding for non periodic fft
   fftdata.insize = 2*fftdata.rawsize;
   fftdata.indata = (double*)fftw_malloc(fftdata.insize*sizeof(double));
   for (int i=0; i<fftdata.insize; i++){
      fftdata.indata[i] = 0.0;
   }

   // is half as long as the input data array, but takes same amount of memory
   // as the output data is of complex type
   fftdata.outsize = fftdata.rawsize;
   fftdata.outdata = (fftw_complex*)fftw_malloc(fftdata.outsize*sizeof(fftw_complex));
   for (int i=0; i<fftdata.outsize; i++){
      fftdata.outdata[i][0] = 0.0;
      fftdata.outdata[i][1] = 0.0;
   }
   // precompute frequencies
   fftdata.freqs = (double*)malloc(fftdata.outsize*sizeof(double));
   double scale = 0.5/(fftdata.outsize*dt);
   for (int i=0; i<fftdata.outsize; i++) {
      fftdata.freqs[i] = scale*((double)i);
   }

   // creating the fftw plan
   fftdata.plan = fftw_plan_dft_r2c_1d(fftdata.insize,  //number of elements
                                       fftdata.indata,  //input data
                                       fftdata.outdata, //output data
                                       FFTW_ESTIMATE);  //FFTW algorithm

   return fftdata;

}

void perform_fft(fft_t fftdata) {
   // filter and fourier transform data
   for (int i=0; i<fftdata.rawsize; i++){
      fftdata.indata[i] = nuttall_window(fftdata.rawsize, (double) i) * fftdata.rawdata[i];
   }

   fftw_execute(fftdata.plan);

   return;
}

void free_fft(fft_t* fftdata) {
   // clear fft data
   fftw_destroy_plan(fftdata->plan);
   fftw_free(fftdata->indata);
   fftdata->insize = 0;
   fftw_free(fftdata->outdata);
   fftdata->outsize = 0;
   fftdata->rawsize = 0;
   free(fftdata->rawdata);

   return;
}
