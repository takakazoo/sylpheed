/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "folderview.h"
#include "folder.h"
#include <glib/gi18n.h>

static void on_factory_setup(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *box;
	GtkWidget *icon;
	GtkWidget *label;

	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	gtk_widget_set_margin_start(box, 6);
	gtk_widget_set_margin_end(box, 6);
	gtk_widget_set_margin_top(box, 4);
	gtk_widget_set_margin_bottom(box, 4);

	icon = gtk_image_new_from_icon_name("folder-symbolic");
	label = gtk_label_new(NULL);
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);

	gtk_box_append(GTK_BOX(box), icon);
	gtk_box_append(GTK_BOX(box), label);

	gtk_list_item_set_child(list_item, box);
}

static void on_factory_bind(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *box = gtk_list_item_get_child(list_item);
	GtkWidget *label = gtk_widget_get_last_child(box);
	GtkStringObject *strobj = GTK_STRING_OBJECT(gtk_list_item_get_item(list_item));

	if (strobj && label) {
		gtk_label_set_text(GTK_LABEL(label), gtk_string_object_get_string(strobj));
	}
}

static void on_selection_changed(GtkSingleSelection *selection, GParamSpec *pspec, gpointer user_data)
{
	FolderView *folderview = (FolderView *)user_data;
	guint selected = gtk_single_selection_get_selected(selection);

	if (selected != GTK_INVALID_LIST_POSITION && folderview->folder_selected_cb) {
		folderview->folder_selected_cb(folderview, folderview->selected_item, folderview->user_data);
	}
}

FolderView *folder_view_create(void)
{
	FolderView *folderview;
	GtkWidget *scrolled_win;
	GtkListItemFactory *factory;
	const char *default_folders[] = {
		"📥 受信箱 (Inbox)",
		"📤 送信済み (Sent)",
		"📝 下書き (Drafts)",
		"🗑️ ごみ箱 (Trash)",
		"🚫 迷惑メール (Junk)",
		NULL
	};

	folderview = g_new0(FolderView, 1);

	folderview->model = gtk_string_list_new(default_folders);
	folderview->selection = gtk_single_selection_new(G_LIST_MODEL(folderview->model));

	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_factory_setup), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_factory_bind), NULL);

	folderview->list_view = gtk_list_view_new(GTK_SELECTION_MODEL(folderview->selection), factory);
	g_signal_connect(folderview->selection, "notify::selected", G_CALLBACK(on_selection_changed), folderview);

	scrolled_win = gtk_scrolled_window_new();
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_win), folderview->list_view);
	gtk_widget_set_vexpand(scrolled_win, TRUE);
	gtk_widget_set_hexpand(scrolled_win, TRUE);

	folderview->container = scrolled_win;

	return folderview;
}

void folder_view_set_selected_callback(FolderView *folderview,
				       void (*cb)(FolderView *folderview, FolderItem *item, gpointer user_data),
				       gpointer user_data)
{
	if (!folderview) return;
	folderview->folder_selected_cb = cb;
	folderview->user_data = user_data;
}

void folder_view_refresh(FolderView *folderview)
{
	/* Phase 3 basic refresh */
}
