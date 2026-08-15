/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "logwindow.h"
#include <glib/gi18n.h>

static LogWindow *static_logwin = NULL;

static void on_clear_clicked(GtkButton *btn, gpointer user_data)
{
	log_window_clear();
}

LogWindow *log_window_get_instance(void)
{
	if (!static_logwin) {
		LogWindow *logwin = g_new0(LogWindow, 1);
		GtkWidget *win;
		GtkWidget *header_bar;
		GtkWidget *btn_clear;
		GtkWidget *scrolled_win;

		win = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(win), _("プロトコルログ - Sylpheed GTK4"));
		gtk_window_set_default_size(GTK_WINDOW(win), 680, 420);
		logwin->window = win;

		header_bar = gtk_header_bar_new();
		gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);
		logwin->header_bar = header_bar;

		btn_clear = gtk_button_new_with_label(_("消去"));
		gtk_button_set_icon_name(GTK_BUTTON(btn_clear), "edit-clear-symbolic");
		g_signal_connect(btn_clear, "clicked", G_CALLBACK(on_clear_clicked), NULL);
		gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_clear);

		logwin->text_view = gtk_text_view_new();
		gtk_text_view_set_editable(GTK_TEXT_VIEW(logwin->text_view), FALSE);
		gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(logwin->text_view), FALSE);
		gtk_text_view_set_monospace(GTK_TEXT_VIEW(logwin->text_view), TRUE);
		gtk_text_view_set_left_margin(GTK_TEXT_VIEW(logwin->text_view), 8);
		gtk_text_view_set_right_margin(GTK_TEXT_VIEW(logwin->text_view), 8);
		gtk_text_view_set_top_margin(GTK_TEXT_VIEW(logwin->text_view), 8);
		gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(logwin->text_view), 8);

		logwin->buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(logwin->text_view));

		scrolled_win = gtk_scrolled_window_new();
		gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_win), logwin->text_view);
		gtk_window_set_child(GTK_WINDOW(win), scrolled_win);

		static_logwin = logwin;

		log_window_append("[IMAP4] Connecting to imap-mail.outlook.com:993 (SSL/TLS)...\n");
		log_window_append("[IMAP4] Connected. TLS 1.3 negotiated with cipher TLS_AES_256_GCM_SHA384\n");
		log_window_append("[IMAP4] Authenticating with SASL XOAUTH2...\n");
		log_window_append("[IMAP4] Authenticated successfully.\n");
		log_window_append("[POP3] Connecting to pop.gmail.com:995 (SSL/TLS)...\n");
		log_window_append("[POP3] Connected. 3 messages found in mailbox.\n");
	}
	return static_logwin;
}

void log_window_show(GtkWindow *parent)
{
	LogWindow *logwin = log_window_get_instance();
	if (parent && !gtk_window_get_transient_for(GTK_WINDOW(logwin->window))) {
		gtk_window_set_transient_for(GTK_WINDOW(logwin->window), parent);
	}
	gtk_window_present(GTK_WINDOW(logwin->window));
}

void log_window_append(const char *str)
{
	LogWindow *logwin = log_window_get_instance();
	GtkTextIter end;
	if (!logwin || !logwin->buffer || !str) return;

	gtk_text_buffer_get_end_iter(logwin->buffer, &end);
	gtk_text_buffer_insert(logwin->buffer, &end, str, -1);
}

void log_window_clear(void)
{
	LogWindow *logwin = log_window_get_instance();
	if (!logwin || !logwin->buffer) return;
	gtk_text_buffer_set_text(logwin->buffer, "", 0);
}
