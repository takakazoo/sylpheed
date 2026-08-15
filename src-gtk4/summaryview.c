/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "summaryview.h"
#include <glib/gi18n.h>

G_DEFINE_TYPE(MsgItem, msg_item, G_TYPE_OBJECT)

static void msg_item_init(MsgItem *self) {}
static void msg_item_finalize(GObject *object)
{
	MsgItem *self = (MsgItem *)object;
	g_free(self->from);
	g_free(self->to);
	g_free(self->subject);
	g_free(self->date);
	g_free(self->size);
	g_free(self->body);
	g_free(self->attach_file);
	g_free(self->attach_size);
	G_OBJECT_CLASS(msg_item_parent_class)->finalize(object);
}

static void msg_item_class_init(MsgItemClass *klass)
{
	G_OBJECT_CLASS(klass)->finalize = msg_item_finalize;
}

static MsgItem *msg_item_new(const char *from, const char *to, const char *subject, const char *date, const char *size,
			     const char *body, const char *attach_file, const char *attach_size, gboolean unread, gboolean marked)
{
	MsgItem *item = g_object_new(TYPE_MSG_ITEM, NULL);
	item->from = g_strdup(from);
	item->to = g_strdup(to);
	item->subject = g_strdup(subject);
	item->date = g_strdup(date);
	item->size = g_strdup(size);
	item->body = g_strdup(body);
	item->attach_file = g_strdup(attach_file);
	item->attach_size = g_strdup(attach_size);
	item->unread = unread;
	item->marked = marked;
	return item;
}

/* Filter Function */
static gboolean filter_func(gpointer item, gpointer user_data)
{
	SummaryView *summaryview = (SummaryView *)user_data;
	MsgItem *msg = (MsgItem *)item;
	const char *text;

	if (!summaryview->search_entry) return TRUE;
	text = gtk_editable_get_text(GTK_EDITABLE(summaryview->search_entry));
	if (!text || !*text) return TRUE;

	if (msg->subject && g_strrstr_len(msg->subject, -1, text)) return TRUE;
	if (msg->from && g_strrstr_len(msg->from, -1, text)) return TRUE;
	if (msg->body && g_strrstr_len(msg->body, -1, text)) return TRUE;

	return FALSE;
}

static void on_search_changed(GtkSearchEntry *entry, gpointer user_data)
{
	SummaryView *summaryview = (SummaryView *)user_data;
	gtk_custom_filter_set_filter_func(summaryview->filter, filter_func, summaryview, NULL);
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

static void on_bind_status(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	MsgItem *item = (MsgItem *)gtk_list_item_get_item(list_item);
	if (item && label) {
		const char *icon_str = item->unread ? "🔵" : (item->marked ? "⭐" : "");
		gtk_label_set_text(GTK_LABEL(label), icon_str);
	}
}

static void on_bind_attach(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	MsgItem *item = (MsgItem *)gtk_list_item_get_item(list_item);
	if (item && label) {
		gtk_label_set_text(GTK_LABEL(label), (item->attach_file && *item->attach_file) ? "📎" : "");
	}
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
		MsgItem *item = (MsgItem *)g_list_model_get_item(G_LIST_MODEL(summaryview->filter_model), pos);
		if (item) {
			summaryview->message_selected_cb(summaryview, item, summaryview->user_data);
			g_object_unref(item);
		}
	}
}

static void on_summary_right_click(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data)
{
	SummaryView *summaryview = (SummaryView *)user_data;
	GtkWidget *menu_popover;
	GMenu *menu;

	if (gtk_gesture_single_get_current_button(GTK_GESTURE_SINGLE(gesture)) == GDK_BUTTON_SECONDARY) {
		menu = g_menu_new();
		g_menu_append(menu, _("返信"), "app.reply");
		g_menu_append(menu, _("転送"), "app.forward");
		g_menu_append(menu, _("メッセージのソースを表示"), "app.view-source");
		g_menu_append(menu, _("フォルダへ移動..."), "app.move-to");
		g_menu_append(menu, _("削除"), "app.delete");
		g_menu_append(menu, _("未読にする"), "app.mark-unread");
		g_menu_append(menu, _("マークを付ける"), "app.mark");

		menu_popover = gtk_popover_menu_new_from_model(G_MENU_MODEL(menu));
		gtk_widget_set_parent(menu_popover, summaryview->column_view);
		gtk_popover_set_pointing_to(GTK_POPOVER(menu_popover), &(const GdkRectangle){(int)x, (int)y, 1, 1});
		gtk_popover_popup(GTK_POPOVER(menu_popover));
	}
}

SummaryView *summary_view_create(void)
{
	SummaryView *summaryview;
	GtkWidget *vbox;
	GtkWidget *search_bar_box;
	GtkWidget *scrolled_win;
	GtkListItemFactory *factory;
	GtkColumnViewColumn *col;
	GtkGesture *right_click_gesture;

	summaryview = g_new0(SummaryView, 1);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	summaryview->container = vbox;

	/* 1. Quick Search Bar */
	search_bar_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
	gtk_widget_set_margin_start(search_bar_box, 6);
	gtk_widget_set_margin_end(search_bar_box, 6);
	gtk_widget_set_margin_top(search_bar_box, 4);
	gtk_widget_set_margin_bottom(search_bar_box, 4);

	summaryview->search_entry = gtk_search_entry_new();
	gtk_widget_set_hexpand(summaryview->search_entry, TRUE);
	g_object_set(summaryview->search_entry, "placeholder-text", _("クイック検索 (件名・差出人・本文)..."), NULL);
	g_signal_connect(summaryview->search_entry, "search-changed", G_CALLBACK(on_search_changed), summaryview);
	gtk_box_append(GTK_BOX(search_bar_box), summaryview->search_entry);
	gtk_box_append(GTK_BOX(vbox), search_bar_box);

	/* 2. Store and Filter Model */
	summaryview->store = g_list_store_new(TYPE_MSG_ITEM);
	summaryview->filter = gtk_custom_filter_new(filter_func, summaryview, NULL);
	summaryview->filter_model = gtk_filter_list_model_new(G_LIST_MODEL(summaryview->store), GTK_FILTER(summaryview->filter));
	summaryview->selection = gtk_single_selection_new(G_LIST_MODEL(summaryview->filter_model));

	/* Add sample messages */
	g_list_store_append(summaryview->store,
		msg_item_new("Sylpheed Team <sylpheed@sraoss.jp>",
			     "User <user@example.com>",
			     "Welcome to Sylpheed GTK4 Preview!",
			     "2026/08/15 21:00",
			     "3.2 KB",
			     "Sylpheed の GTK4 プレビュー版へようこそ！\n\n"
			     "> GTK2 の軽快さと操作性を完全に踏襲しつつ、\n"
			     "> GTK4 による高DPI対応、添付ファイルバー、クイック検索バーを統合しました。\n\n"
			     "上のメール一覧をクリックすると、メッセージプレビューが連動して切り替わります。\n",
			     "release-notes.pdf",
			     "145 KB",
			     FALSE, TRUE));

	g_list_store_append(summaryview->store,
		msg_item_new("Microsoft 365 <no-reply@microsoft.com>",
			     "User <user@example.com>",
			     "Microsoft 365 / Outlook.com サポート",
			     "2026/08/15 20:30",
			     "5.8 KB",
			     "Microsoft 365 / Outlook.com との安全な OAuth 2.0 (XOAUTH2) 連携がサポートされました。\n\n"
			     "> 暗号化通信 (TLS 1.3 / OpenSSL 3.x) により、セキュアにメール送受信が行えます。\n",
			     NULL, NULL,
			     TRUE, FALSE));

	g_list_store_append(summaryview->store,
		msg_item_new("Google Security <no-reply@google.com>",
			     "User <user@example.com>",
			     "Gmail OAuth 2.0 連携完了のお知らせ",
			     "2026/08/15 19:45",
			     "4.1 KB",
			     "Gmail アカウントとの OAuth 2.0 認証が正常に設定されました。\n\n"
			     "ブラウザ経由での安全なワンクリック認証により、パスワードを直接入力することなく接続できます。\n",
			     "security-guide.pdf",
			     "88 KB",
			     FALSE, FALSE));

	/* 3. Column View */
	summaryview->column_view = gtk_column_view_new(GTK_SELECTION_MODEL(summaryview->selection));
	gtk_column_view_set_show_row_separators(GTK_COLUMN_VIEW(summaryview->column_view), TRUE);
	gtk_column_view_set_show_column_separators(GTK_COLUMN_VIEW(summaryview->column_view), TRUE);

	/* Col: Status */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_status), NULL);
	col = gtk_column_view_column_new(_("状態"), factory);
	gtk_column_view_column_set_fixed_width(col, 45);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(summaryview->column_view), col);

	/* Col: Attach */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_attach), NULL);
	col = gtk_column_view_column_new(_("添付"), factory);
	gtk_column_view_column_set_fixed_width(col, 45);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(summaryview->column_view), col);

	/* Col: From */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_from), NULL);
	col = gtk_column_view_column_new(_("差出人"), factory);
	gtk_column_view_column_set_fixed_width(col, 220);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(summaryview->column_view), col);

	/* Col: Subject */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_subject), NULL);
	col = gtk_column_view_column_new(_("件名"), factory);
	gtk_column_view_column_set_expand(col, TRUE);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(summaryview->column_view), col);

	/* Col: Date */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_date), NULL);
	col = gtk_column_view_column_new(_("日時"), factory);
	gtk_column_view_column_set_fixed_width(col, 150);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(summaryview->column_view), col);

	/* Col: Size */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_size), NULL);
	col = gtk_column_view_column_new(_("サイズ"), factory);
	gtk_column_view_column_set_fixed_width(col, 75);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(summaryview->column_view), col);

	g_signal_connect(summaryview->selection, "notify::selected", G_CALLBACK(on_summary_selection_changed), summaryview);

	/* Right-click Gesture for Context Menu */
	right_click_gesture = gtk_gesture_click_new();
	gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(right_click_gesture), GDK_BUTTON_SECONDARY);
	g_signal_connect(right_click_gesture, "pressed", G_CALLBACK(on_summary_right_click), summaryview);
	gtk_widget_add_controller(summaryview->column_view, GTK_EVENT_CONTROLLER(right_click_gesture));

	scrolled_win = gtk_scrolled_window_new();
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_win), summaryview->column_view);
	gtk_widget_set_vexpand(scrolled_win, TRUE);
	gtk_widget_set_hexpand(scrolled_win, TRUE);

	gtk_box_append(GTK_BOX(vbox), scrolled_win);

	return summaryview;
}

void summary_view_set_selected_callback(SummaryView *summaryview,
					void (*cb)(SummaryView *summaryview, MsgItem *item, gpointer user_data),
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
