#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Audio IO handling via SDL2
#include <SDL2/SDL.h>

// GUI via GTK
#include <gtk/gtk.h>

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

      // Bottom Panel
      GtkWidget *BottomPanel;
        // Audio Data Plotting
        GtkWidget *SignalPlottingArea;

GtkBuilder *builder;

// remove later
gboolean timer_handler();
#define N 128
int arr[N];

int main(int argc, char **argv) {

   // init SDL and Audio handling
   if (SDL_Init(SDL_INIT_AUDIO)) {
      fprintf(stderr, "Failed to initialize SDL\n");
      return -1;
   }

   // initialize GTK
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
   g_timeout_add(125, (GSourceFunc) timer_handler, arr);

   // Run GTK-Gui
   gtk_main();

   // Finalize SDL 
   SDL_Quit();

   return 0;
}

gboolean timer_handler() {
   //dummy plotting function
   for (int i=0; i<N; i++) {
      arr[i] = rand();
   }
   gtk_widget_queue_draw(SignalPlottingArea);

   return true;
}

gboolean on_SignalPlottingArea_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
   guint width, height;

   width = gtk_widget_get_allocated_width(widget);
   height = gtk_widget_get_allocated_height(widget);

   cairo_set_line_width(cr, 1.0);

   for (int i=0; i<N-1; i++) {
      double di = (double) i;
      cairo_set_source_rgb(cr, 1.0, 0.0, 0.0); //color red
      cairo_move_to(cr, (di/N*width), height-(arr[i]%height));
      di = (double) i+1;
      cairo_line_to(cr, (di/N*width), height-(arr[i+1]%height));
      cairo_stroke(cr);
   }

   return false;
}
