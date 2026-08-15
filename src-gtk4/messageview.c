/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "messageview.h"

MessageView *message_view_create(void)
{
	MessageView *msgview;
	GtkWidget *scrolled_win;

	msgview = g_new0(MessageView, 1);

	msgview->text_view = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(msgview->text_view), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(msgview->text_view), FALSE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(msgview->text_view), GTK_WRAP_WORD_CHAR);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(msgview->text_view), 12);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(msgview->text_view), 12);
	gtk_text_view_set_top_margin(GTK_TEXT_VIEW(msgview->text_view), 10);
	gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(msgview->text_view), 10);

	msgview->buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(msgview->text_view));

	/* Initial welcome message */
	message_view_set_text(msgview,
		"From: Sylpheed Team <sylpheed@sraoss.jp>\n"
		"Subject: Welcome to Sylpheed GTK4 Preview!\n"
		"Date: Sat, 15 Aug 2026 21:00:00 +0900\n\n"
		"Sylpheed の GTK4 プレビュー版へようこそ！\n\n"
		"GTK4 による高DPI・高速GPUレンダリング・最新のメニュー・ヘッダーバーに対応した次世代 UI です。\n"
		"左側のフォルダツリーやメール一覧を選択すると、プレビューが連動して更新されます。\n");

	scrolled_win = gtk_scrolled_window_new();
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_win), msgview->text_view);
	gtk_widget_set_vexpand(scrolled_win, TRUE);
	gtk_widget_set_hexpand(scrolled_win, TRUE);

	msgview->container = scrolled_win;

	return msgview;
}

void message_view_set_text(MessageView *msgview, const char *text)
{
	if (!msgview || !msgview->buffer) return;
	gtk_text_buffer_set_text(msgview->buffer, text ? text : "", -1);
}

void message_view_clear(MessageView *msgview)
{
	if (!msgview || !msgview->buffer) return;
	gtk_text_buffer_set_text(msgview->buffer, "", 0);
}
