/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "inputdialog.h"
#include <glib/gi18n.h>

char *input_dialog(GtkWindow *parent,
		   const char *title,
		   const char *message,
		   const char *default_string)
{
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *vbox;
	GtkWidget *label;
	GtkWidget *entry;
	GtkWidget *btn_ok;

	win = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(win), title ? title : _("入力"));
	gtk_window_set_default_size(GTK_WINDOW(win), 380, 160);
	if (parent) {
		gtk_window_set_transient_for(GTK_WINDOW(win), parent);
		gtk_window_set_modal(GTK_WINDOW(win), TRUE);
	}

	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);

	btn_ok = gtk_button_new_with_label(_("OK"));
	gtk_widget_add_css_class(btn_ok, "suggested-action");
	gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), btn_ok);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_widget_set_margin_start(vbox, 16);
	gtk_widget_set_margin_end(vbox, 16);
	gtk_widget_set_margin_top(vbox, 16);
	gtk_widget_set_margin_bottom(vbox, 16);
	gtk_window_set_child(GTK_WINDOW(win), vbox);

	label = gtk_label_new(message ? message : "");
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);
	gtk_box_append(GTK_BOX(vbox), label);

	entry = gtk_entry_new();
	if (default_string) {
		gtk_editable_set_text(GTK_EDITABLE(entry), default_string);
	}
	gtk_box_append(GTK_BOX(vbox), entry);

	gtk_window_present(GTK_WINDOW(win));

	return g_strdup(default_string ? default_string : "");
}

char *input_dialog_with_invisible(GtkWindow *parent,
				  const char *title,
				  const char *message,
				  const char *default_string)
{
	return input_dialog(parent, title, message, default_string);
}
