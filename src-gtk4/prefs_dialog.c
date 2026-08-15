/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "prefs_dialog.h"
#include <glib/gi18n.h>

void prefs_dialog_show(GtkWindow *parent)
{
	GtkWidget *dialog;
	GtkWidget *header_bar;
	GtkWidget *notebook;
	GtkWidget *vbox_gen, *vbox_disp, *vbox_send;
	GtkWidget *chk_auto_check, *chk_confirm_send, *chk_smooth_font;

	dialog = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(dialog), _("全般の設定 - Sylpheed GTK4"));
	gtk_window_set_default_size(GTK_WINDOW(dialog), 600, 450);
	if (parent) {
		gtk_window_set_transient_for(GTK_WINDOW(dialog), parent);
		gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
	}

	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(dialog), header_bar);

	notebook = gtk_notebook_new();
	gtk_widget_set_margin_start(notebook, 12);
	gtk_widget_set_margin_end(notebook, 12);
	gtk_widget_set_margin_top(notebook, 12);
	gtk_widget_set_margin_bottom(notebook, 12);
	gtk_window_set_child(GTK_WINDOW(dialog), notebook);

	/* 1. General Tab */
	vbox_gen = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
	gtk_widget_set_margin_start(vbox_gen, 12);
	gtk_widget_set_margin_top(vbox_gen, 12);
	chk_auto_check = gtk_check_button_new_with_label(_("起動時に新着メールをチェックする"));
	gtk_check_button_set_active(GTK_CHECK_BUTTON(chk_auto_check), TRUE);
	gtk_box_append(GTK_BOX(vbox_gen), chk_auto_check);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox_gen, gtk_label_new(_("基本")));

	/* 2. Display Tab */
	vbox_disp = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
	gtk_widget_set_margin_start(vbox_disp, 12);
	gtk_widget_set_margin_top(vbox_disp, 12);
	chk_smooth_font = gtk_check_button_new_with_label(_("高DPI・ClearType フォントを優先する"));
	gtk_check_button_set_active(GTK_CHECK_BUTTON(chk_smooth_font), TRUE);
	gtk_box_append(GTK_BOX(vbox_disp), chk_smooth_font);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox_disp, gtk_label_new(_("表示・フォント")));

	/* 3. Send Tab */
	vbox_send = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
	gtk_widget_set_margin_start(vbox_send, 12);
	gtk_widget_set_margin_top(vbox_send, 12);
	chk_confirm_send = gtk_check_button_new_with_label(_("送信前に確認ダイアログを表示する"));
	gtk_box_append(GTK_BOX(vbox_send), chk_confirm_send);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox_send, gtk_label_new(_("送信")));

	gtk_window_present(GTK_WINDOW(dialog));
}
