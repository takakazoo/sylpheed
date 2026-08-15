/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "foldersel.h"
#include <glib/gi18n.h>

char *foldersel_folder_sel(GtkWindow *parent, const char *title, const char *default_folder)
{
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *btn_ok;
	GtkWidget *vbox;
	GtkWidget *scrolled_win;
	GtkWidget *list_view;
	GtkStringList *model;
	const char *folders[] = {
		"📥 受信箱 (INBOX)",
		"  ↳ 💼 Work",
		"  ↳ 👥 Private",
		"📤 送信済み (Sent)",
		"📝 下書き (Drafts)",
		"🗑️ ごみ箱 (Trash)",
		"🚫 迷惑メール (Junk)",
		NULL
	};

	win = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(win), title ? title : _("フォルダの選択 - Sylpheed GTK4"));
	gtk_window_set_default_size(GTK_WINDOW(win), 400, 350);
	if (parent) {
		gtk_window_set_transient_for(GTK_WINDOW(win), parent);
		gtk_window_set_modal(GTK_WINDOW(win), TRUE);
	}

	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);

	btn_ok = gtk_button_new_with_label(_("選択"));
	gtk_widget_add_css_class(btn_ok, "suggested-action");
	gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), btn_ok);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_window_set_child(GTK_WINDOW(win), vbox);

	model = gtk_string_list_new(folders);
	list_view = gtk_list_view_new(GTK_SELECTION_MODEL(gtk_single_selection_new(G_LIST_MODEL(model))), NULL);

	scrolled_win = gtk_scrolled_window_new();
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_win), list_view);
	gtk_widget_set_vexpand(scrolled_win, TRUE);
	gtk_widget_set_hexpand(scrolled_win, TRUE);
	gtk_box_append(GTK_BOX(vbox), scrolled_win);

	gtk_window_present(GTK_WINDOW(win));

	return g_strdup(default_folder ? default_folder : "INBOX");
}
