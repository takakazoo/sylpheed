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

static void load_custom_css(void)
{
	GtkCssProvider *provider = gtk_css_provider_new();
	const char *css_data =
		"label.badge {\n"
		"    background-color: #1a73e8;\n"
		"    color: white;\n"
		"    font-weight: bold;\n"
		"    font-size: 11px;\n"
		"    border-radius: 10px;\n"
		"    padding: 1px 7px;\n"
		"    margin-start: 4px;\n"
		"}\n"
		"label.dim-label {\n"
		"    opacity: 0.65;\n"
		"    font-weight: 500;\n"
		"}\n";

	gtk_css_provider_load_from_string(provider, css_data);
	gtk_style_context_add_provider_for_display(gdk_display_get_default(),
						   GTK_STYLE_PROVIDER(provider),
						   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	g_object_unref(provider);
}

static void on_activate(GtkApplication *app, gpointer user_data)
{
	load_custom_css();

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
