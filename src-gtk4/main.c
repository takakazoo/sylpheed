/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <stdio.h>
#include <stdlib.h>

#include "sylmain.h"
#include "prefs_common.h"
#include "mainwindow.h"

static MainWindow *static_mainwindow = NULL;

static void on_activate(GtkApplication *app, gpointer user_data)
{
	if (!static_mainwindow) {
		static_mainwindow = main_window_create(app);
	}
	gtk_window_present(GTK_WINDOW(static_mainwindow->window));
}

int main(int argc, char *argv[])
{
	GtkApplication *app;
	int status;

	/* Initialize LibSylph backend */
	syl_init();

	/* Initialize GTK4 Application */
	app = gtk_application_new("org.sylpheed.SylpheedGtk4", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

	status = g_application_run(G_APPLICATION(app), argc, argv);

	g_object_unref(app);

	return status;
}
