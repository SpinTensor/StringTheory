#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// Audio IO handling via SDL2
#include <SDL2/SDL.h>

// GUI via GTK
#include <gtk/gtk.h>

#include "audio_IO.h"
#include "fft.h"

// Global Window
GtkWidget *window;
GtkWidget *GlobalFixed;
  // Select window function for fft
  GtkWidget *SetWindowFunction;
  GtkWidget *WindowFunctionEntry;

  // Three drawing areas
  // Raw audio signal
  GtkWidget *drawAudio;
  // Fourier Transformed
  GtkWidget *drawFrequencySpectrum;

GtkBuilder *builder;

// declaration for the function that updates all audio data
gboolean update_audio_data();
//sfd
gboolean on_drawAudio_draw(GtkWidget *widget, cairo_t *cr, gpointer data);

// Global declaration of all data
audio_IO_t audiodata;
fft_t fftdata;

int main(int argc, char **argv) {

   // samples per second
   const int sps = 8;

   // init SDL and Audio handling
   if (SDL_Init(SDL_INIT_AUDIO)) {
      fprintf(stderr, "Failed to initialize SDL\n");
      return -1; 
   }   
   init_audio(&audiodata, sps);
   fftdata = init_fft(audiodata.buffsize, 1.0/(audiodata.audiodevice.freq));

   // initialize GTK for GUI handling
   gtk_init(&argc, &argv);

   // parse glade file to build gtk-gui
   builder = gtk_builder_new_from_file("AudioVisualization.glade");

   // create the main window and fixed container
   window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
     g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
     GlobalFixed = GTK_WIDGET(gtk_builder_get_object(builder, "GlobalFixed"));

     // Selector for the window function
     SetWindowFunction = GTK_WIDGET(gtk_builder_get_object(builder, "SetWindowFunction"));
     WindowFunctionEntry = GTK_WIDGET(gtk_builder_get_object(builder, "WindowFunctionEntry"));

     // The two drawing areas
     drawAudio = GTK_WIDGET(gtk_builder_get_object(builder, "drawAudio"));
     drawFrequencySpectrum = GTK_WIDGET(gtk_builder_get_object(builder, "drawFrequencySpectrum"));

   // create connections between widgets
   gtk_builder_connect_signals(builder, NULL);

   // release builder 
   g_object_unref(builder);

   // present the main window
   gtk_widget_show(window);

   // functions to be called repeatedly throughout runtime
   g_timeout_add(1000/sps, (GSourceFunc) update_audio_data, NULL);

   // remaining GTK initializations
   gtk_combo_box_set_active(GTK_COMBO_BOX(SetWindowFunction), (int) fftdata.winfunc);

   // Run GTK-Gui
   gtk_main();

   // free audio data
   free_audio(&audiodata);
   free_fft(&fftdata);

   // Finalize SDL 
   SDL_Quit();

   return 0;
}

gboolean update_audio_data(){
   // Audio data is by updated via callback
   // copy audio data to fftdata struct
   for (int i=0; i<audiodata.buffsize; i++) {
      fftdata.rawdata[i] = (double) audiodata.buffer[i];
   }
   // perform fourier transformation
   perform_fft(fftdata);

   // initiate plotting of data
   gtk_widget_queue_draw(drawAudio);
   gtk_widget_queue_draw(drawFrequencySpectrum);

   return true;
}

gboolean on_drawAudio_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
   // plot audio signal and frequency data
   
   // height and width of the drawing area
   guint width, height;
   width = gtk_widget_get_allocated_width(widget);
   height = gtk_widget_get_allocated_height(widget);

   // set drawing line width
   cairo_set_line_width(cr, 1.0);
   // TODO: draw coordinate system
   
   int ndata = 0;
   double xold, xnew;
   double yold, ynew;
   double offset;
   double scale;
   scale = 8000.0;

   // plot raw audio data
   ndata = audiodata.buffsize;
   offset = (1.0-0.5) * height;
   xold = 0.0;
   yold = offset - scale*fftdata.rawdata[0];
   cairo_set_source_rgb(cr, 1.0, 0.0, 0.0); //color red
   for (int i=1; i<ndata; i++) {
      cairo_move_to(cr, xold, yold);
      double di = (double) i;
      xnew = (di*width)/(ndata-1);
      ynew = offset - scale*fftdata.rawdata[i];
      cairo_line_to(cr, xnew, ynew);
      cairo_stroke(cr);
      xold = xnew;
      yold = ynew;
   }

   // plot filtered audio data
   ndata = fftdata.rawsize;
   offset = (1.0-0.5) * height;
   xold = 0.0;
   yold = offset - scale*fftdata.indata[0];
   cairo_set_source_rgb(cr, 0.0, 0.0, 1.0); //color blue
   for (int i=1; i<ndata; i++) {
      cairo_move_to(cr, xold, yold);
      double di = (double) i;
      xnew = (di*width)/(ndata-1);
      ynew = offset - scale*fftdata.indata[i];
      cairo_line_to(cr, xnew, ynew);
      cairo_stroke(cr);
      xold = xnew;
      yold = ynew;
   }

   return false;
}

gboolean on_drawFrequencySpectrum_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
   // plot audio signal and frequency data
   
   // height and width of the drawing area
   guint width, height;
   width = gtk_widget_get_allocated_width(widget);
   height = gtk_widget_get_allocated_height(widget);

   // set drawing line width
   cairo_set_line_width(cr, 1.0);
   // TODO: draw coordinate system
   
   int ndata = 0;
   double xold, xnew;
   double yold, ynew;
   double offset;
   double scale;

      // plot fourier transformed audio data (frequencies)
   ndata = fftdata.outsize/5;
   double rep = fftdata.outdata[0][0];
   double imp = fftdata.outdata[0][1];
   double amp = sqrt(rep*rep+imp*imp);
   scale = 200.0;
   offset = (1.0-0.1) * height;
   xold = 0.0;
   yold = offset - scale*amp;
   cairo_set_source_rgb(cr, 0.0, 1.0, 0.0); //color red
   for (int i=1; i<ndata; i++) {
      cairo_move_to(cr, xold, yold);
      rep = fftdata.outdata[i][0];
      imp = fftdata.outdata[i][1];
      amp = sqrt(rep*rep+imp*imp);
      double di = (double) i;
      xnew = (di*width)/(ndata-1);
      ynew = offset - scale*amp;
      cairo_line_to(cr, xnew, ynew);
      cairo_stroke(cr);
      xold = xnew;
      yold = ynew;
   }

   return false;
}

void on_SetWindowFunction_changed(GtkComboBox *cb) {
   fftdata.winfunc = (winfunc_t) gtk_combo_box_get_active(GTK_COMBO_BOX(SetWindowFunction));
   return;
}
void on_WindowFunctionEntry_changed(GtkEntry *e) {
   return;
}
