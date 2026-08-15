/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "editaddress.h"
#include <glib/gi18n.h>

static void on_save_clicked(GtkButton *btn, gpointer user_data)
{
	GtkWidget *win = GTK_WIDGET(user_data);
	g_print("[AddressBook] 連絡先を保存しました。\n");
	gtk_window_destroy(GTK_WINDOW(win));
}

void edit_address_dialog_show(GtkWindow *parent,
			      const char *name,
			      const char *email,
			      const char *remarks)
{
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *btn_save;
	GtkWidget *vbox, *grid;
	GtkWidget *label;
	GtkWidget *entry_name, *entry_email, *entry_phone, *entry_org, *entry_remarks;

	win = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(win), _("連絡先の編集 - Sylpheed GTK4"));
	gtk_window_set_default_size(GTK_WINDOW(win), 540, 360);
	if (parent) {
		gtk_window_set_transient_for(GTK_WINDOW(win), parent);
		gtk_window_set_modal(GTK_WINDOW(win), TRUE);
	}

	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);

	btn_save = gtk_button_new_with_label(_("保存"));
	gtk_widget_add_css_class(btn_save, "suggested-action");
	g_signal_connect(btn_save, "clicked", G_CALLBACK(on_save_clicked), win);
	gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), btn_save);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
	gtk_widget_set_margin_start(vbox, 16);
	gtk_widget_set_margin_end(vbox, 16);
	gtk_widget_set_margin_top(vbox, 16);
	gtk_widget_set_margin_bottom(vbox, 16);
	gtk_window_set_child(GTK_WINDOW(win), vbox);

	grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 12);
	gtk_box_append(GTK_BOX(vbox), grid);

	/* Name */
	label = gtk_label_new(_("表示名 (氏名):"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
	entry_name = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(entry_name), name ? name : "山田 太郎");
	gtk_widget_set_hexpand(entry_name, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_name, 1, 0, 1, 1);

	/* Email */
	label = gtk_label_new(_("メールアドレス:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
	entry_email = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(entry_email), email ? email : "yamada@example.com");
	gtk_widget_set_hexpand(entry_email, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_email, 1, 1, 1, 1);

	/* Phone */
	label = gtk_label_new(_("電話番号:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);
	entry_phone = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(entry_phone), "03-1234-5678");
	gtk_widget_set_hexpand(entry_phone, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_phone, 1, 2, 1, 1);

	/* Organization */
	label = gtk_label_new(_("組織・会社名:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 3, 1, 1);
	entry_org = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(entry_org), "Example Inc.");
	gtk_widget_set_hexpand(entry_org, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_org, 1, 3, 1, 1);

	/* Remarks */
	label = gtk_label_new(_("備考:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 4, 1, 1);
	entry_remarks = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(entry_remarks), remarks ? remarks : "");
	gtk_widget_set_hexpand(entry_remarks, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_remarks, 1, 4, 1, 1);

	gtk_window_present(GTK_WINDOW(win));
}
