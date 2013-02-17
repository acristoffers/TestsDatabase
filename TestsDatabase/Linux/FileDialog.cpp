#include <string>

#include <gtk/gtk.h>

std::string OpenFileDialog()
{
    GtkWidget *dialog;

    dialog = gtk_file_chooser_dialog_new ("Open File",
                                          0,
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);

    std::string path;
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
        path = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

    gtk_widget_destroy (dialog);	

    return path;
}

std::string SaveFileDialog()
{
    GtkWidget *dialog;

    dialog = gtk_file_chooser_dialog_new ("Save File",
                                          0,
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                          NULL);

    gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

    gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (dialog), "New DataBase");

    std::string path;
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
        path = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

    gtk_widget_destroy (dialog);

	path += ".qdb";

    return path;
}
