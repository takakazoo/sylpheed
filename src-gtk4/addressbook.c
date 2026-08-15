/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "addressbook.h"
#include <glib/gi18n.h>

#define TYPE_CONTACT_ITEM (contact_item_get_type())
G_DECLARE_FINAL_TYPE(ContactItem, contact_item, CONTACT, ITEM, GObject)

struct _ContactItem
{
	GObject parent_instance;
	char *name;
	char *email;
	char *remarks;
};

G_DEFINE_TYPE(ContactItem, contact_item, G_TYPE_OBJECT)

static void contact_item_init(ContactItem *self) {}
static void contact_item_finalize(GObject *object)
{
	ContactItem *self = (ContactItem *)object;
	g_free(self->name);
	g_free(self->email);
	g_free(self->remarks);
	G_OBJECT_CLASS(contact_item_parent_class)->finalize(object);
}

static void contact_item_class_init(ContactItemClass *klass)
{
	G_OBJECT_CLASS(klass)->finalize = contact_item_finalize;
}

static ContactItem *contact_item_new(const char *name, const char *email, const char *remarks)
{
	ContactItem *item = g_object_new(TYPE_CONTACT_ITEM, NULL);
	item->name = g_strdup(name);
	item->email = g_strdup(email);
	item->remarks = g_strdup(remarks);
	return item;
}

static void on_setup_label(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_label_new(NULL);
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);
	gtk_widget_set_margin_start(label, 8);
	gtk_widget_set_margin_end(label, 8);
	gtk_widget_set_margin_top(label, 4);
	gtk_widget_set_margin_bottom(label, 4);
	gtk_list_item_set_child(list_item, label);
}

static void on_bind_name(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	ContactItem *item = (ContactItem *)gtk_list_item_get_item(list_item);
	if (item && label) gtk_label_set_text(GTK_LABEL(label), item->name);
}

static void on_bind_email(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	ContactItem *item = (ContactItem *)gtk_list_item_get_item(list_item);
	if (item && label) gtk_label_set_text(GTK_LABEL(label), item->email);
}

static void on_bind_remarks(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	ContactItem *item = (ContactItem *)gtk_list_item_get_item(list_item);
	if (item && label) gtk_label_set_text(GTK_LABEL(label), item->remarks);
}

void addressbook_window_show(GtkWindow *parent)
{
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *btn_add, *btn_delete;
	GtkWidget *scrolled_win;
	GtkWidget *column_view;
	GListStore *store;
	GtkSingleSelection *selection;
	GtkListItemFactory *factory;
	GtkColumnViewColumn *col;

	win = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(win), _("アドレス帳 - Sylpheed GTK4"));
	gtk_window_set_default_size(GTK_WINDOW(win), 720, 480);
	if (parent) {
		gtk_window_set_transient_for(GTK_WINDOW(win), parent);
	}

	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);

	btn_add = gtk_button_new_with_label(_("新規連絡先"));
	gtk_button_set_icon_name(GTK_BUTTON(btn_add), "contact-new-symbolic");
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_add);

	btn_delete = gtk_button_new();
	gtk_button_set_icon_name(GTK_BUTTON(btn_delete), "user-trash-symbolic");
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_delete);

	store = g_list_store_new(TYPE_CONTACT_ITEM);
	g_list_store_append(store, contact_item_new("Sylpheed Team", "sylpheed@sraoss.jp", "公式サポート"));
	g_list_store_append(store, contact_item_new("開発チーム", "dev@example.com", "プロジェクトメンバー"));

	selection = gtk_single_selection_new(G_LIST_MODEL(store));
	column_view = gtk_column_view_new(GTK_SELECTION_MODEL(selection));
	gtk_column_view_set_show_row_separators(GTK_COLUMN_VIEW(column_view), TRUE);

	/* Name Column */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_name), NULL);
	col = gtk_column_view_column_new(_("氏名"), factory);
	gtk_column_view_column_set_fixed_width(col, 200);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(column_view), col);

	/* Email Column */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_email), NULL);
	col = gtk_column_view_column_new(_("メールアドレス"), factory);
	gtk_column_view_column_set_expand(col, TRUE);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(column_view), col);

	/* Remarks Column */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_remarks), NULL);
	col = gtk_column_view_column_new(_("備考"), factory);
	gtk_column_view_column_set_fixed_width(col, 160);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(column_view), col);

	scrolled_win = gtk_scrolled_window_new();
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_win), column_view);
	gtk_widget_set_vexpand(scrolled_win, TRUE);
	gtk_widget_set_hexpand(scrolled_win, TRUE);
	gtk_window_set_child(GTK_WINDOW(win), scrolled_win);

	gtk_window_present(GTK_WINDOW(win));
}
