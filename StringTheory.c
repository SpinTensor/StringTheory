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
#include "freq_estimator.h"

// Global Window
GtkWidget *window;
GtkWidget *GlobalFixed;

  // Vertical Spliting
  GtkWidget *VerticalPaned;
    // Left Side Panel
    GtkWidget *ControlPanel;

    // Horizontal Splitting
    GtkWidget *HorizontalPaned;
      // Main Panel
      GtkWidget *MainPanel;
      // Frequency
      GtkWidget *DisplayFrequency;

      // Bottom Panel
      GtkWidget *BottomPanel;
        // Audio Data Plotting
        GtkWidget *SignalPlottingArea;

GtkBuilder *builder;

// declaration for the function that updates all audio data
gboolean update_audio_data();
// Global declaration of all data 
// TODO: figure out how to make this local
audio_IO_t audiodata;
fft_t fftdata;
double result_freq;

// Main function
int main(int argc, char **argv) {

   // samples per second
   const int sps = 8;

   // init SDL and Audio handling
   if (SDL_Init(SDL_INIT_AUDIO)) {
      fprintf(stderr, "Failed to initialize SDL\n");
      return -1;
   }
   audiodata = init_audio(sps);
   fftdata = init_fft(audiodata.buffsize, 1.0/(audiodata.audiodevice.freq));

   // initialize GTK for GUI handling
   gtk_init(&argc, &argv);

   // parse glade file to build gtk-gui
   builder = gtk_builder_new_from_file("StringTheory.glade");

   // create the main window and fixed container
   window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
     g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
     GlobalFixed = GTK_WIDGET(gtk_builder_get_object(builder, "GlobalFixed"));

     // Split vertically
     VerticalPaned = GTK_WIDGET(gtk_builder_get_object(builder, "VerticalPaned"));
       // Create left side panel a.k.a. control panel (Left
       ControlPanel = GTK_WIDGET(gtk_builder_get_object(builder, "ControlPanel"));
       // TODO: Rest
       
       // Split right side of vertical split horizontally
       HorizontalPaned = GTK_WIDGET(gtk_builder_get_object(builder, "HorizontalPaned"));
         // Create Main Panel (Top)
         MainPanel = GTK_WIDGET(gtk_builder_get_object(builder, "MainPanel"));
         // Display frequency
         DisplayFrequency = GTK_WIDGET(gtk_builder_get_object(builder, "DisplayFrequency"));
         // TODO: Rest


         // Create Bottom Panel (Bottom)
         BottomPanel = GTK_WIDGET(gtk_builder_get_object(builder, "BottomPanel"));
           // Create Plotting Area for Audio Data;
           SignalPlottingArea = GTK_WIDGET(gtk_builder_get_object(builder, "SignalPlottingArea"));
   
   // create connections between widgets
   gtk_builder_connect_signals(builder, NULL);

   // release builder 
   g_object_unref(builder);

   // present the main window
   gtk_widget_show(window);

   // functions to be called repeatedly throughout runtime
   g_timeout_add(1000/sps, (GSourceFunc) update_audio_data, NULL);

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
   // get audio data 
   get_audio_data(audiodata);
   // perform fourier transformation
   perform_fft(audiodata.buffer, fftdata);
   // estimate the most prominent frequency
   result_freq = estimate_freq(fftdata, 1.0);
   // Display the frequency in the main window label
   char freqstr[10];
   sprintf(freqstr, "%5.1lf Hz", result_freq);
   gtk_label_set_text(GTK_LABEL(DisplayFrequency), freqstr);

   // initiate plotting of data
   gtk_widget_queue_draw(SignalPlottingArea);

   return true;
}

gboolean on_SignalPlottingArea_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
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

   // plot raw audio data
   ndata = audiodata.buffsize/5;
   scale = 1000.0;
   offset = (1.0-0.75) * height;
   xold = 0.0;
   yold = offset - scale*audiodata.buffer[0];
   cairo_set_source_rgb(cr, 1.0, 0.0, 0.0); //color red
   for (int i=1; i<ndata; i++) {
      cairo_move_to(cr, xold, yold);
      double di = (double) i;
      xnew = (di*width)/(ndata-1);
      ynew = offset - scale*audiodata.buffer[i];
      cairo_line_to(cr, xnew, ynew);
      cairo_stroke(cr);
      xold = xnew;
      yold = ynew;
   }

   // plot fourier transformed audio data (frequencies)
   ndata = fftdata.outsize/5;
   double rep = fftdata.outdata[0][0];
   double imp = fftdata.outdata[0][1];
   double amp = sqrt(rep*rep+imp*imp);
   scale = 20.0;
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
