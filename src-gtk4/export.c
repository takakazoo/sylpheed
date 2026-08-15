/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "export.h"
#include "alertpanel.h"
#include <glib/gi18n.h>

static void on_export_clicked(GtkButton *btn, gpointer user_data)
{
	GtkWidget *win = GTK_WIDGET(user_data);
	alertpanel_notice(GTK_WINDOW(win), _("エクスポート完了"), _("メールデータのエクスポートが完了しました。"));
	gtk_window_destroy(GTK_WINDOW(win));
}

void export_dialog_show(GtkWindow *parent)
{
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *btn_export;
	GtkWidget *vbox, *grid;
	GtkWidget *label;
	GtkWidget *entry_src, *entry_dest;
	GtkWidget *radio_mbox, *radio_eml;

	win = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(win), _("メールのエクスポート - Sylpheed GTK4"));
	gtk_window_set_default_size(GTK_WINDOW(win), 520, 280);
	if (parent) {
		gtk_window_set_transient_for(GTK_WINDOW(win), parent);
		gtk_window_set_modal(GTK_WINDOW(win), TRUE);
	}

	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);

	btn_export = gtk_button_new_with_label(_("エクスポート開始"));
	gtk_widget_add_css_class(btn_export, "suggested-action");
	g_signal_connect(btn_export, "clicked", G_CALLBACK(on_export_clicked), win);
	gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), btn_export);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
	gtk_widget_set_margin_start(vbox, 16);
	gtk_widget_set_margin_end(vbox, 16);
	gtk_widget_set_margin_top(vbox, 16);
	gtk_widget_set_margin_bottom(vbox, 16);
	gtk_window_set_child(GTK_WINDOW(win), vbox);

	/* Format */
	label = gtk_label_new(_("エクスポート形式:"));
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);
	gtk_box_append(GTK_BOX(vbox), label);

	radio_mbox = gtk_check_button_new_with_label(_("UNIX mbox 形式"));
	gtk_check_button_set_active(GTK_CHECK_BUTTON(radio_mbox), TRUE);
	gtk_box_append(GTK_BOX(vbox), radio_mbox);

	radio_eml = gtk_check_button_new_with_label(_("EML / 個別メッセージファイル群"));
	gtk_check_button_set_group(GTK_CHECK_BUTTON(radio_eml), GTK_CHECK_BUTTON(radio_mbox));
	gtk_box_append(GTK_BOX(vbox), radio_eml);

	grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 12);
	gtk_box_append(GTK_BOX(vbox), grid);

	/* Source */
	label = gtk_label_new(_("エクスポート元フォルダ:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
	entry_src = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(entry_src), "INBOX");
	gtk_widget_set_hexpand(entry_src, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_src, 1, 0, 1, 1);

	/* Dest */
	label = gtk_label_new(_("エクスポート先ファイル:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
	entry_dest = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(entry_dest), "C:\\Backup\\export_inbox.mbox");
	gtk_widget_set_hexpand(entry_dest, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_dest, 1, 1, 1, 1);

	gtk_window_present(GTK_WINDOW(win));
}
