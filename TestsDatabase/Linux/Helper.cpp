#include "Helper.h"

#include <gtk/gtk.h>

void openURI(std::string uri)
{
    GError *error = NULL;
    gtk_show_uri ( gdk_screen_get_default (), uri.c_str(), gtk_get_current_event_time (), &error);
}

