/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "summaryview.h"
#include <glib/gi18n.h>

#define TYPE_MSG_ITEM (msg_item_get_type())
G_DECLARE_FINAL_TYPE(MsgItem, msg_item, MSG, ITEM, GObject)

struct _MsgItem
{
	GObject parent_instance;
	char *from;
	char *subject;
	char *date;
	char *size;
	char *body;
};

G_DEFINE_TYPE(MsgItem, msg_item, G_TYPE_OBJECT)

static void msg_item_init(MsgItem *self) {}
static void msg_item_finalize(GObject *object)
{
	MsgItem *self = (MsgItem *)object;
	g_free(self->from);
	g_free(self->subject);
	g_free(self->date);
	g_free(self->size);
	g_free(self->body);
	G_OBJECT_CLASS(msg_item_parent_class)->finalize(object);
}

static void msg_item_class_init(MsgItemClass *klass)
{
	G_OBJECT_CLASS(klass)->finalize = msg_item_finalize;
}

static MsgItem *msg_item_new(const char *from, const char *subject, const char *date, const char *size, const char *body)
{
	MsgItem *item = g_object_new(TYPE_MSG_ITEM, NULL);
	item->from = g_strdup(from);
	item->subject = g_strdup(subject);
	item->date = g_strdup(date);
	item->size = g_strdup(size);
	item->body = g_strdup(body);
	return item;
}

/* Column Factories */
static void on_setup_label(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_label_new(NULL);
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);
	gtk_widget_set_margin_start(label, 6);
	gtk_widget_set_margin_end(label, 6);
	gtk_widget_set_margin_top(label, 4);
	gtk_widget_set_margin_bottom(label, 4);
	gtk_list_item_set_child(list_item, label);
}

static void on_bind_from(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	MsgItem *item = (MsgItem *)gtk_list_item_get_item(list_item);
	if (item && label) gtk_label_set_text(GTK_LABEL(label), item->from);
}

static void on_bind_subject(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	MsgItem *item = (MsgItem *)gtk_list_item_get_item(list_item);
	if (item && label) gtk_label_set_text(GTK_LABEL(label), item->subject);
}

static void on_bind_date(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	MsgItem *item = (MsgItem *)gtk_list_item_get_item(list_item);
	if (item && label) gtk_label_set_text(GTK_LABEL(label), item->date);
}

static void on_bind_size(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	MsgItem *item = (MsgItem *)gtk_list_item_get_item(list_item);
	if (item && label) gtk_label_set_text(GTK_LABEL(label), item->size);
}

static void on_summary_selection_changed(GtkSingleSelection *selection, GParamSpec *pspec, gpointer user_data)
{
	SummaryView *summaryview = (SummaryView *)user_data;
	guint pos = gtk_single_selection_get_selected(selection);

	if (pos != GTK_INVALID_LIST_POSITION && summaryview->message_selected_cb) {
		MsgItem *item = (MsgItem *)g_list_model_get_item(G_LIST_MODEL(summaryview->store), pos);
		if (item) {
			summaryview->message_selected_cb(summaryview, item->body ? item->body : "", summaryview->user_data);
			g_object_unref(item);
		}
	}
}

SummaryView *summary_view_create(void)
{
	SummaryView *summaryview;
	GtkWidget *scrolled_win;
	GtkListItemFactory *factory;
	GtkColumnViewColumn *col;

	summaryview = g_new0(SummaryView, 1);

	summaryview->store = g_list_store_new(TYPE_MSG_ITEM);
	summaryview->selection = gtk_single_selection_new(G_LIST_MODEL(summaryview->store));

	/* Add sample messages */
	g_list_store_append(summaryview->store,
		msg_item_new("Sylpheed Team <sylpheed@sraoss.jp>",
			     "Welcome to Sylpheed GTK4 Preview!",
			     "2026/08/15 21:00",
			     "3.2 KB",
			     "From: Sylpheed Team <sylpheed@sraoss.jp>\n"
			     "Subject: Welcome to Sylpheed GTK4 Preview!\n"
			     "Date: Sat, 15 Aug 2026 21:00:00 +0900\n\n"
			     "Sylpheed の GTK4 プレビュー版へようこそ！\n\n"
			     "GTK4 による高DPI・高速GPUレンダリング・最新のメニュー・ヘッダーバーに対応した次世代 UI です。\n"));

	g_list_store_append(summaryview->store,
		msg_item_new("Microsoft 365 <no-reply@microsoft.com>",
			     "Microsoft 365 / Outlook.com サポート",
			     "2026/08/15 20:30",
			     "5.8 KB",
			     "From: Microsoft 365 <no-reply@microsoft.com>\n"
			     "Subject: Microsoft 365 / Outlook.com サポート\n"
			     "Date: Sat, 15 Aug 2026 20:30:00 +0900\n\n"
			     "OAuth 2.0 (XOAUTH2) および最新の SSL/TLS (OpenSSL 3.x) による安全な通信が確立されました。\n"));

	summaryview->column_view = gtk_column_view_new(GTK_SELECTION_MODEL(summaryview->selection));
	gtk_column_view_set_show_row_separators(GTK_COLUMN_VIEW(summaryview->column_view), TRUE);
	gtk_column_view_set_show_column_separators(GTK_COLUMN_VIEW(summaryview->column_view), TRUE);

	/* Column 1: From */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_from), NULL);
	col = gtk_column_view_column_new(_("差出人"), factory);
	gtk_column_view_column_set_fixed_width(col, 240);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(summaryview->column_view), col);

	/* Column 2: Subject */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_subject), NULL);
	col = gtk_column_view_column_new(_("件名"), factory);
	gtk_column_view_column_set_expand(col, TRUE);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(summaryview->column_view), col);

	/* Column 3: Date */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_date), NULL);
	col = gtk_column_view_column_new(_("日時"), factory);
	gtk_column_view_column_set_fixed_width(col, 160);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(summaryview->column_view), col);

	/* Column 4: Size */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_size), NULL);
	col = gtk_column_view_column_new(_("サイズ"), factory);
	gtk_column_view_column_set_fixed_width(col, 80);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(summaryview->column_view), col);

	g_signal_connect(summaryview->selection, "notify::selected", G_CALLBACK(on_summary_selection_changed), summaryview);

	scrolled_win = gtk_scrolled_window_new();
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_win), summaryview->column_view);
	gtk_widget_set_vexpand(scrolled_win, TRUE);
	gtk_widget_set_hexpand(scrolled_win, TRUE);

	summaryview->container = scrolled_win;

	return summaryview;
}

void summary_view_set_selected_callback(SummaryView *summaryview,
					void (*cb)(SummaryView *summaryview, const char *msg_id, gpointer user_data),
					gpointer user_data)
{
	if (!summaryview) return;
	summaryview->message_selected_cb = cb;
	summaryview->user_data = user_data;
}

void summary_view_load_folder(SummaryView *summaryview, gpointer folder_item)
{
	/* Phase 3 load messages */
}
