/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "folderview.h"
#include "folder.h"
#include <glib/gi18n.h>

#define TYPE_FOLDER_ROW (folder_row_get_type())
G_DECLARE_FINAL_TYPE(FolderRow, folder_row, FOLDER, ROW, GObject)

struct _FolderRow
{
	GObject parent_instance;
	char *name;
	char *icon_name;
	int unread_count;
	int total_count;
};

G_DEFINE_TYPE(FolderRow, folder_row, G_TYPE_OBJECT)

static void folder_row_init(FolderRow *self) {}
static void folder_row_finalize(GObject *object)
{
	FolderRow *self = (FolderRow *)object;
	g_free(self->name);
	g_free(self->icon_name);
	G_OBJECT_CLASS(folder_row_parent_class)->finalize(object);
}

static void folder_row_class_init(FolderRowClass *klass)
{
	G_OBJECT_CLASS(klass)->finalize = folder_row_finalize;
}

static FolderRow *folder_row_new(const char *name, const char *icon_name, int unread_count, int total_count)
{
	FolderRow *row = g_object_new(TYPE_FOLDER_ROW, NULL);
	row->name = g_strdup(name);
	row->icon_name = g_strdup(icon_name);
	row->unread_count = unread_count;
	row->total_count = total_count;
	return row;
}

static void on_factory_setup(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *box;
	GtkWidget *icon;
	GtkWidget *label;
	GtkWidget *badge;

	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	gtk_widget_set_margin_start(box, 8);
	gtk_widget_set_margin_end(box, 8);
	gtk_widget_set_margin_top(box, 4);
	gtk_widget_set_margin_bottom(box, 4);

	icon = gtk_image_new();
	label = gtk_label_new(NULL);
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);
	gtk_widget_set_hexpand(label, TRUE);

	badge = gtk_label_new(NULL);
	gtk_widget_add_css_class(badge, "badge");

	gtk_box_append(GTK_BOX(box), icon);
	gtk_box_append(GTK_BOX(box), label);
	gtk_box_append(GTK_BOX(box), badge);

	gtk_list_item_set_child(list_item, box);
}

static void on_factory_bind(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *box = gtk_list_item_get_child(list_item);
	GtkWidget *icon = gtk_widget_get_first_child(box);
	GtkWidget *label = gtk_widget_get_next_sibling(icon);
	GtkWidget *badge = gtk_widget_get_next_sibling(label);
	FolderRow *row = (FolderRow *)gtk_list_item_get_item(list_item);

	if (row) {
		gtk_image_set_from_icon_name(GTK_IMAGE(icon), row->icon_name ? row->icon_name : "folder-symbolic");
		gtk_label_set_text(GTK_LABEL(label), row->name ? row->name : "");

		if (row->unread_count > 0) {
			gchar *badge_text = g_strdup_printf("%d", row->unread_count);
			gtk_label_set_text(GTK_LABEL(badge), badge_text);
			g_free(badge_text);
			gtk_widget_set_visible(badge, TRUE);
		} else {
			gtk_widget_set_visible(badge, FALSE);
		}
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
	GListStore *store;

	folderview = g_new0(FolderView, 1);

	store = g_list_store_new(TYPE_FOLDER_ROW);
	g_list_store_append(store, folder_row_new(_("受信箱 (Inbox)"), "mail-inbox-symbolic", 1, 3));
	g_list_store_append(store, folder_row_new(_("送信済み (Sent)"), "mail-send-symbolic", 0, 12));
	g_list_store_append(store, folder_row_new(_("下書き (Drafts)"), "document-edit-symbolic", 0, 2));
	g_list_store_append(store, folder_row_new(_("ごみ箱 (Trash)"), "user-trash-symbolic", 0, 5));
	g_list_store_append(store, folder_row_new(_("迷惑メール (Junk)"), "mail-mark-junk-symbolic", 0, 0));

	folderview->selection = gtk_single_selection_new(G_LIST_MODEL(store));

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
