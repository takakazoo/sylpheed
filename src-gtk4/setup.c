/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "setup.h"
#include <glib/gi18n.h>

static void on_create_account_clicked(GtkButton *btn, gpointer user_data)
{
	GtkWidget *win = GTK_WIDGET(user_data);
	g_print("[Setup] アカウント作成完了\n");
	gtk_window_destroy(GTK_WINDOW(win));
}

void setup_wizard_show(GtkWindow *parent)
{
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *btn_finish;
	GtkWidget *vbox, *grid;
	GtkWidget *label;
	GtkWidget *radio_imap, *radio_pop3, *radio_gmail, *radio_ms365;
	GtkWidget *entry_name, *entry_addr, *entry_pass;

	win = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(win), _("アカウントの新規作成 - Sylpheed GTK4"));
	gtk_window_set_default_size(GTK_WINDOW(win), 580, 480);
	if (parent) {
		gtk_window_set_transient_for(GTK_WINDOW(win), parent);
		gtk_window_set_modal(GTK_WINDOW(win), TRUE);
	}

	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);

	btn_finish = gtk_button_new_with_label(_("作成"));
	gtk_widget_add_css_class(btn_finish, "suggested-action");
	g_signal_connect(btn_finish, "clicked", G_CALLBACK(on_create_account_clicked), win);
	gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), btn_finish);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
	gtk_widget_set_margin_start(vbox, 20);
	gtk_widget_set_margin_end(vbox, 20);
	gtk_widget_set_margin_top(vbox, 16);
	gtk_widget_set_margin_bottom(vbox, 16);
	gtk_window_set_child(GTK_WINDOW(win), vbox);

	/* Account Type */
	label = gtk_label_new(_("アカウントの種類を選択してください:"));
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);
	gtk_box_append(GTK_BOX(vbox), label);

	radio_imap = gtk_check_button_new_with_label(_("IMAP4 (標準)"));
	gtk_check_button_set_active(GTK_CHECK_BUTTON(radio_imap), TRUE);
	gtk_box_append(GTK_BOX(vbox), radio_imap);

	radio_pop3 = gtk_check_button_new_with_label(_("POP3 (標準)"));
	gtk_check_button_set_group(GTK_CHECK_BUTTON(radio_pop3), GTK_CHECK_BUTTON(radio_imap));
	gtk_box_append(GTK_BOX(vbox), radio_pop3);

	radio_gmail = gtk_check_button_new_with_label(_("Gmail (OAuth 2.0)"));
	gtk_check_button_set_group(GTK_CHECK_BUTTON(radio_gmail), GTK_CHECK_BUTTON(radio_imap));
	gtk_box_append(GTK_BOX(vbox), radio_gmail);

	radio_ms365 = gtk_check_button_new_with_label(_("Microsoft 365 / Outlook.com (OAuth 2.0)"));
	gtk_check_button_set_group(GTK_CHECK_BUTTON(radio_ms365), GTK_CHECK_BUTTON(radio_imap));
	gtk_box_append(GTK_BOX(vbox), radio_ms365);

	gtk_box_append(GTK_BOX(vbox), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL));

	/* Input Grid */
	grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 12);
	gtk_box_append(GTK_BOX(vbox), grid);

	/* Name */
	label = gtk_label_new(_("お名前:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
	entry_name = gtk_entry_new();
	gtk_widget_set_hexpand(entry_name, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_name, 1, 0, 1, 1);

	/* Mail Address */
	label = gtk_label_new(_("メールアドレス:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
	entry_addr = gtk_entry_new();
	gtk_widget_set_hexpand(entry_addr, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_addr, 1, 1, 1, 1);

	/* Password */
	label = gtk_label_new(_("パスワード:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);
	entry_pass = gtk_password_entry_new();
	gtk_password_entry_set_show_peek_icon(GTK_PASSWORD_ENTRY(entry_pass), TRUE);
	gtk_widget_set_hexpand(entry_pass, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_pass, 1, 2, 1, 1);

	gtk_window_present(GTK_WINDOW(win));
}
