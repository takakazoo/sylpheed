/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "editgroup.h"
#include <glib/gi18n.h>

void edit_group_dialog_show(GtkWindow *parent, const char *group_name)
{
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *btn_save;
	GtkWidget *vbox;
	GtkWidget *label;
	GtkWidget *entry_name;
	GtkWidget *scrolled_win, *list_view;
	GtkStringList *members_model;
	const char *members[] = {
		"山田 太郎 <yamada@example.com>",
		"佐藤 次郎 <sato@example.com>",
		"鈴木 花子 <suzuki@example.com>",
		NULL
	};

	win = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(win), _("グループの編集 - Sylpheed GTK4"));
	gtk_window_set_default_size(GTK_WINDOW(win), 480, 360);
	if (parent) {
		gtk_window_set_transient_for(GTK_WINDOW(win), parent);
		gtk_window_set_modal(GTK_WINDOW(win), TRUE);
	}

	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);

	btn_save = gtk_button_new_with_label(_("保存"));
	gtk_widget_add_css_class(btn_save, "suggested-action");
	gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), btn_save);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_widget_set_margin_start(vbox, 16);
	gtk_widget_set_margin_end(vbox, 16);
	gtk_widget_set_margin_top(vbox, 16);
	gtk_widget_set_margin_bottom(vbox, 16);
	gtk_window_set_child(GTK_WINDOW(win), vbox);

	label = gtk_label_new(_("グループ名:"));
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);
	gtk_box_append(GTK_BOX(vbox), label);

	entry_name = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(entry_name), group_name ? group_name : "開発チーム");
	gtk_box_append(GTK_BOX(vbox), entry_name);

	label = gtk_label_new(_("グループメンバー:"));
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);
	gtk_box_append(GTK_BOX(vbox), label);

	members_model = gtk_string_list_new(members);
	list_view = gtk_list_view_new(GTK_SELECTION_MODEL(gtk_single_selection_new(G_LIST_MODEL(members_model))), NULL);

	scrolled_win = gtk_scrolled_window_new();
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_win), list_view);
	gtk_widget_set_vexpand(scrolled_win, TRUE);
	gtk_widget_set_hexpand(scrolled_win, TRUE);
	gtk_box_append(GTK_BOX(vbox), scrolled_win);

	gtk_window_present(GTK_WINDOW(win));
}
