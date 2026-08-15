/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "compose.h"
#include <glib/gi18n.h>

static void on_send_clicked(GtkButton *btn, gpointer user_data)
{
	ComposeWindow *compose = (ComposeWindow *)user_data;
	const char *to = gtk_editable_get_text(GTK_EDITABLE(compose->entry_to));
	const char *subject = gtk_editable_get_text(GTK_EDITABLE(compose->entry_subject));

	g_print("[Compose] 送信実行: To=%s, Subject=%s\n", to, subject);

	/* Close window on send */
	gtk_window_destroy(GTK_WINDOW(compose->window));
}

static void on_draft_clicked(GtkButton *btn, gpointer user_data)
{
	g_print("[Compose] 下書き保存\n");
}

static void on_attach_clicked(GtkButton *btn, gpointer user_data)
{
	ComposeWindow *compose = (ComposeWindow *)user_data;
	GtkFileDialog *dialog = gtk_file_dialog_new();
	gtk_file_dialog_set_title(dialog, _("添付ファイルの選択"));

	/* Phase 4 file dialog placeholder */
	gtk_string_list_append(compose->attach_model, "📎 添付ファイル (sample.pdf, 120 KB)");
	gtk_widget_set_visible(compose->attach_box, TRUE);
}

ComposeWindow *compose_window_new(GtkWindow *parent, const char *to, const char *subject, const char *body)
{
	ComposeWindow *compose;
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *btn_send, *btn_draft, *btn_attach;
	GtkWidget *vbox;
	GtkWidget *grid;
	GtkWidget *label;
	GtkWidget *scrolled_win;

	compose = g_new0(ComposeWindow, 1);

	/* Window */
	win = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(win), _("メッセージの作成 - Sylpheed GTK4"));
	gtk_window_set_default_size(GTK_WINDOW(win), 800, 600);
	if (parent) {
		gtk_window_set_transient_for(GTK_WINDOW(win), parent);
	}
	compose->window = win;

	/* Header Bar */
	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);
	compose->header_bar = header_bar;

	/* Send Button (Suggested Action Style) */
	btn_send = gtk_button_new_with_label(_("送信"));
	gtk_button_set_icon_name(GTK_BUTTON(btn_send), "mail-send-symbolic");
	gtk_widget_add_css_class(btn_send, "suggested-action");
	g_signal_connect(btn_send, "clicked", G_CALLBACK(on_send_clicked), compose);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_send);

	/* Draft Button */
	btn_draft = gtk_button_new_with_label(_("下書き"));
	gtk_button_set_icon_name(GTK_BUTTON(btn_draft), "document-save-symbolic");
	g_signal_connect(btn_draft, "clicked", G_CALLBACK(on_draft_clicked), compose);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_draft);

	/* Attach Button */
	btn_attach = gtk_button_new();
	gtk_button_set_icon_name(GTK_BUTTON(btn_attach), "mail-attachment-symbolic");
	gtk_widget_set_tooltip_text(btn_attach, _("ファイルの添付"));
	g_signal_connect(btn_attach, "clicked", G_CALLBACK(on_attach_clicked), compose);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_attach);

	/* Main VBox */
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
	gtk_widget_set_margin_start(vbox, 12);
	gtk_widget_set_margin_end(vbox, 12);
	gtk_widget_set_margin_top(vbox, 8);
	gtk_widget_set_margin_bottom(vbox, 8);
	gtk_window_set_child(GTK_WINDOW(win), vbox);

	/* Header Fields (Grid) */
	grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 6);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 12);
	gtk_box_append(GTK_BOX(vbox), grid);

	/* From */
	label = gtk_label_new(_("差出人:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
	compose->entry_from = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(compose->entry_from), "User <user@example.com>");
	gtk_widget_set_hexpand(compose->entry_from, TRUE);
	gtk_grid_attach(GTK_GRID(grid), compose->entry_from, 1, 0, 1, 1);

	/* To */
	label = gtk_label_new(_("宛先:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
	compose->entry_to = gtk_entry_new();
	if (to) gtk_editable_set_text(GTK_EDITABLE(compose->entry_to), to);
	gtk_widget_set_hexpand(compose->entry_to, TRUE);
	gtk_grid_attach(GTK_GRID(grid), compose->entry_to, 1, 1, 1, 1);

	/* Subject */
	label = gtk_label_new(_("件名:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);
	compose->entry_subject = gtk_entry_new();
	if (subject) gtk_editable_set_text(GTK_EDITABLE(compose->entry_subject), subject);
	gtk_widget_set_hexpand(compose->entry_subject, TRUE);
	gtk_grid_attach(GTK_GRID(grid), compose->entry_subject, 1, 2, 1, 1);

	/* Attachment Box (Hidden initially) */
	compose->attach_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
	gtk_widget_set_visible(compose->attach_box, FALSE);
	compose->attach_model = gtk_string_list_new(NULL);
	compose->attach_list = gtk_list_view_new(GTK_SELECTION_MODEL(gtk_single_selection_new(G_LIST_MODEL(compose->attach_model))), NULL);
	gtk_box_append(GTK_BOX(compose->attach_box), compose->attach_list);
	gtk_box_append(GTK_BOX(vbox), compose->attach_box);

	/* Separator */
	gtk_box_append(GTK_BOX(vbox), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL));

	/* Body Editor (TextView) */
	compose->text_view = gtk_text_view_new();
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(compose->text_view), GTK_WRAP_WORD_CHAR);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(compose->text_view), 8);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(compose->text_view), 8);
	gtk_text_view_set_top_margin(GTK_TEXT_VIEW(compose->text_view), 8);
	gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(compose->text_view), 8);

	compose->text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(compose->text_view));
	if (body) {
		gtk_text_buffer_set_text(compose->text_buffer, body, -1);
	}

	scrolled_win = gtk_scrolled_window_new();
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_win), compose->text_view);
	gtk_widget_set_vexpand(scrolled_win, TRUE);
	gtk_widget_set_hexpand(scrolled_win, TRUE);
	gtk_box_append(GTK_BOX(vbox), scrolled_win);

	return compose;
}

void compose_window_show(ComposeWindow *compose)
{
	if (!compose || !compose->window) return;
	gtk_window_present(GTK_WINDOW(compose->window));
}
