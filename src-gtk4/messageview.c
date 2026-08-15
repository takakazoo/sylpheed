/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "messageview.h"
#include <glib/gi18n.h>

static void on_save_attachment(GtkButton *btn, gpointer user_data)
{
	const char *filename = (const char *)user_data;
	g_print("[MessageView] 添付ファイルの保存: %s\n", filename ? filename : "attachment");
}

MessageView *message_view_create(void)
{
	MessageView *msgview;
	GtkWidget *vbox;
	GtkWidget *header_frame, *header_grid;
	GtkWidget *label;
	GtkWidget *scrolled_win;

	msgview = g_new0(MessageView, 1);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	msgview->container = vbox;

	/* 1. Header Panel (From, To, Subject, Date) */
	header_frame = gtk_frame_new(NULL);
	gtk_widget_set_margin_start(header_frame, 8);
	gtk_widget_set_margin_end(header_frame, 8);
	gtk_widget_set_margin_top(header_frame, 6);
	gtk_widget_set_margin_bottom(header_frame, 4);

	header_grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(header_grid), 4);
	gtk_grid_set_column_spacing(GTK_GRID(header_grid), 10);
	gtk_widget_set_margin_start(header_grid, 10);
	gtk_widget_set_margin_end(header_grid, 10);
	gtk_widget_set_margin_top(header_grid, 8);
	gtk_widget_set_margin_bottom(header_grid, 8);
	gtk_frame_set_child(GTK_FRAME(header_frame), header_grid);
	msgview->header_box = header_frame;

	/* Subject */
	label = gtk_label_new(_("件名:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_widget_add_css_class(label, "dim-label");
	gtk_grid_attach(GTK_GRID(header_grid), label, 0, 0, 1, 1);
	msgview->header_subject = gtk_label_new("");
	gtk_label_set_xalign(GTK_LABEL(msgview->header_subject), 0.0);
	gtk_label_set_selectable(GTK_LABEL(msgview->header_subject), TRUE);
	gtk_widget_set_hexpand(msgview->header_subject, TRUE);
	gtk_grid_attach(GTK_GRID(header_grid), msgview->header_subject, 1, 0, 1, 1);

	/* From */
	label = gtk_label_new(_("差出人:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_widget_add_css_class(label, "dim-label");
	gtk_grid_attach(GTK_GRID(header_grid), label, 0, 1, 1, 1);
	msgview->header_from = gtk_label_new("");
	gtk_label_set_xalign(GTK_LABEL(msgview->header_from), 0.0);
	gtk_label_set_selectable(GTK_LABEL(msgview->header_from), TRUE);
	gtk_grid_attach(GTK_GRID(header_grid), msgview->header_from, 1, 1, 1, 1);

	/* Date */
	label = gtk_label_new(_("日時:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_widget_add_css_class(label, "dim-label");
	gtk_grid_attach(GTK_GRID(header_grid), label, 0, 2, 1, 1);
	msgview->header_date = gtk_label_new("");
	gtk_label_set_xalign(GTK_LABEL(msgview->header_date), 0.0);
	gtk_label_set_selectable(GTK_LABEL(msgview->header_date), TRUE);
	gtk_grid_attach(GTK_GRID(header_grid), msgview->header_date, 1, 2, 1, 1);

	gtk_box_append(GTK_BOX(vbox), header_frame);

	/* 2. Attachment Chips Bar */
	msgview->attach_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	gtk_widget_set_margin_start(msgview->attach_box, 10);
	gtk_widget_set_margin_end(msgview->attach_box, 10);
	gtk_widget_set_margin_bottom(msgview->attach_box, 6);
	gtk_widget_set_visible(msgview->attach_box, FALSE);

	label = gtk_label_new(_("📎 添付ファイル:"));
	gtk_box_append(GTK_BOX(msgview->attach_box), label);

	msgview->attach_chips_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
	gtk_box_append(GTK_BOX(msgview->attach_box), msgview->attach_chips_box);

	gtk_box_append(GTK_BOX(vbox), msgview->attach_box);

	/* 3. Message Body (TextView) */
	msgview->text_view = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(msgview->text_view), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(msgview->text_view), FALSE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(msgview->text_view), GTK_WRAP_WORD_CHAR);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(msgview->text_view), 12);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(msgview->text_view), 12);
	gtk_text_view_set_top_margin(GTK_TEXT_VIEW(msgview->text_view), 8);
	gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(msgview->text_view), 8);

	msgview->buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(msgview->text_view));

	/* Create Text Tags for Highlighting */
	msgview->tag_quote = gtk_text_buffer_create_tag(msgview->buffer, "quote",
							"foreground", "#1a73e8",
							NULL);
	msgview->tag_header = gtk_text_buffer_create_tag(msgview->buffer, "header",
							 "weight", PANGO_WEIGHT_BOLD,
							 NULL);

	scrolled_win = gtk_scrolled_window_new();
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_win), msgview->text_view);
	gtk_widget_set_vexpand(scrolled_win, TRUE);
	gtk_widget_set_hexpand(scrolled_win, TRUE);
	gtk_box_append(GTK_BOX(vbox), scrolled_win);

	/* Initial message */
	message_view_set_message(msgview,
		"Sylpheed Team <sylpheed@sraoss.jp>",
		"User <user@example.com>",
		"Welcome to Sylpheed GTK4 Preview!",
		"2026/08/15 21:00",
		"Sylpheed の GTK4 プレビュー版へようこそ！\n\n"
		"> GTK2 の軽快さと操作性を完全に踏襲しつつ、\n"
		"> GTK4 による高DPI対応、添付ファイルバー、クイック検索バーを統合しました。\n\n"
		"上のメール一覧をクリックすると、メッセージプレビューが連動して切り替わります。\n",
		"release-notes.pdf",
		"145 KB");

	return msgview;
}

void message_view_set_message(MessageView *msgview,
			      const char *from,
			      const char *to,
			      const char *subject,
			      const char *date,
			      const char *body,
			      const char *attachment_filename,
			      const char *attachment_size)
{
	GtkTextIter iter;
	gchar **lines;
	int i;

	if (!msgview) return;

	/* Set Header Labels */
	gtk_label_set_text(GTK_LABEL(msgview->header_from), from ? from : "");
	gtk_label_set_text(GTK_LABEL(msgview->header_subject), subject ? subject : "");
	gtk_label_set_text(GTK_LABEL(msgview->header_date), date ? date : "");

	/* Handle Attachment Chips */
	/* Clear existing chips */
	GtkWidget *child = gtk_widget_get_first_child(msgview->attach_chips_box);
	while (child) {
		GtkWidget *next = gtk_widget_get_next_sibling(child);
		gtk_box_remove(GTK_BOX(msgview->attach_chips_box), child);
		child = next;
	}

	if (attachment_filename && *attachment_filename) {
		GtkWidget *chip_btn;
		gchar *chip_label = g_strdup_printf("%s (%s)", attachment_filename, attachment_size ? attachment_size : "");
		chip_btn = gtk_button_new_with_label(chip_label);
		gtk_button_set_icon_name(GTK_BUTTON(chip_btn), "document-save-symbolic");
		g_signal_connect(chip_btn, "clicked", G_CALLBACK(on_save_attachment), (gpointer)attachment_filename);
		gtk_box_append(GTK_BOX(msgview->attach_chips_box), chip_btn);
		g_free(chip_label);
		gtk_widget_set_visible(msgview->attach_box, TRUE);
	} else {
		gtk_widget_set_visible(msgview->attach_box, FALSE);
	}

	/* Populate Body with Quote Highlighting */
	gtk_text_buffer_set_text(msgview->buffer, "", 0);
	gtk_text_buffer_get_start_iter(msgview->buffer, &iter);

	if (body) {
		lines = g_strsplit(body, "\n", -1);
		for (i = 0; lines[i] != NULL; i++) {
			GtkTextIter start_line = iter;
			gtk_text_buffer_insert(msgview->buffer, &iter, lines[i], -1);
			if (lines[i][0] == '>') {
				gtk_text_buffer_apply_tag(msgview->buffer, msgview->tag_quote, &start_line, &iter);
			}
			if (lines[i+1] != NULL) {
				gtk_text_buffer_insert(msgview->buffer, &iter, "\n", 1);
			}
		}
		g_strfreev(lines);
	}
}

void message_view_clear(MessageView *msgview)
{
	if (!msgview) return;
	gtk_label_set_text(GTK_LABEL(msgview->header_from), "");
	gtk_label_set_text(GTK_LABEL(msgview->header_subject), "");
	gtk_label_set_text(GTK_LABEL(msgview->header_date), "");
	gtk_widget_set_visible(msgview->attach_box, FALSE);
	gtk_text_buffer_set_text(msgview->buffer, "", 0);
}
