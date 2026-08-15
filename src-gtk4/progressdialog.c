/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "progressdialog.h"
#include <glib/gi18n.h>

static void on_cancel_clicked(GtkButton *btn, gpointer user_data)
{
	ProgressDialog *dialog = (ProgressDialog *)user_data;
	dialog->cancelled = TRUE;
	progress_dialog_destroy(dialog);
}

ProgressDialog *progress_dialog_create(GtkWindow *parent, const char *title)
{
	ProgressDialog *dialog = g_new0(ProgressDialog, 1);
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *vbox;

	win = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(win), title ? title : _("送受信の進行状況"));
	gtk_window_set_default_size(GTK_WINDOW(win), 440, 160);
	if (parent) {
		gtk_window_set_transient_for(GTK_WINDOW(win), parent);
		gtk_window_set_modal(GTK_WINDOW(win), TRUE);
	}
	dialog->window = win;

	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);
	dialog->header_bar = header_bar;

	dialog->btn_cancel = gtk_button_new_with_label(_("中止"));
	g_signal_connect(dialog->btn_cancel, "clicked", G_CALLBACK(on_cancel_clicked), dialog);
	gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), dialog->btn_cancel);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_widget_set_margin_start(vbox, 16);
	gtk_widget_set_margin_end(vbox, 16);
	gtk_widget_set_margin_top(vbox, 16);
	gtk_widget_set_margin_bottom(vbox, 16);
	gtk_window_set_child(GTK_WINDOW(win), vbox);

	dialog->label_title = gtk_label_new(title ? title : _("処理中..."));
	gtk_label_set_xalign(GTK_LABEL(dialog->label_title), 0.0);
	gtk_widget_add_css_class(dialog->label_title, "dim-label");
	gtk_box_append(GTK_BOX(vbox), dialog->label_title);

	dialog->progress_bar = gtk_progress_bar_new();
	gtk_box_append(GTK_BOX(vbox), dialog->progress_bar);

	dialog->label_status = gtk_label_new(_("接続中..."));
	gtk_label_set_xalign(GTK_LABEL(dialog->label_status), 0.0);
	gtk_box_append(GTK_BOX(vbox), dialog->label_status);

	gtk_window_present(GTK_WINDOW(win));

	return dialog;
}

void progress_dialog_set_value(ProgressDialog *dialog, double fraction)
{
	if (!dialog || !dialog->progress_bar) return;
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(dialog->progress_bar), fraction);
}

void progress_dialog_set_status(ProgressDialog *dialog, const char *status)
{
	if (!dialog || !dialog->label_status) return;
	gtk_label_set_text(GTK_LABEL(dialog->label_status), status ? status : "");
}

void progress_dialog_destroy(ProgressDialog *dialog)
{
	if (!dialog) return;
	if (dialog->window) {
		gtk_window_destroy(GTK_WINDOW(dialog->window));
	}
	g_free(dialog);
}
