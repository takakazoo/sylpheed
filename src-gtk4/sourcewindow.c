/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "sourcewindow.h"
#include <glib/gi18n.h>

void source_window_show(GtkWindow *parent, const char *raw_source)
{
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *text_view;
	GtkTextBuffer *buffer;
	GtkWidget *scrolled_win;

	win = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(win), _("メッセージのソース - Sylpheed GTK4"));
	gtk_window_set_default_size(GTK_WINDOW(win), 720, 500);
	if (parent) {
		gtk_window_set_transient_for(GTK_WINDOW(win), parent);
	}

	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);

	text_view = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
	gtk_text_view_set_monospace(GTK_TEXT_VIEW(text_view), TRUE);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(text_view), 10);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(text_view), 10);
	gtk_text_view_set_top_margin(GTK_TEXT_VIEW(text_view), 8);
	gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(text_view), 8);

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
	if (raw_source) {
		gtk_text_buffer_set_text(buffer, raw_source, -1);
	} else {
		gtk_text_buffer_set_text(buffer,
			"Received: from mail.example.com by sylpheed.example (GTK4)\n"
			"From: Sylpheed Team <sylpheed@sraoss.jp>\n"
			"To: User <user@example.com>\n"
			"Subject: Welcome to Sylpheed GTK4 Preview!\n"
			"Date: Sat, 15 Aug 2026 21:00:00 +0900\n"
			"Content-Type: multipart/mixed; boundary=\"====boundary====\"\n"
			"MIME-Version: 1.0\n\n"
			"--====boundary====\n"
			"Content-Type: text/plain; charset=UTF-8\n\n"
			"Sylpheed の GTK4 プレビュー版へようこそ！\n"
			"--====boundary====--\n", -1);
	}

	scrolled_win = gtk_scrolled_window_new();
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_win), text_view);
	gtk_window_set_child(GTK_WINDOW(win), scrolled_win);

	gtk_window_present(GTK_WINDOW(win));
}
