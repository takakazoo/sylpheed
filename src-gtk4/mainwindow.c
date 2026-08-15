/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "mainwindow.h"
#include <glib/gi18n.h>

static void on_get_mail_clicked(GtkButton *btn, gpointer user_data)
{
	g_print("Get mail clicked\n");
}

static void on_compose_clicked(GtkButton *btn, gpointer user_data)
{
	g_print("Compose mail clicked\n");
}

MainWindow *main_window_create(GtkApplication *app)
{
	MainWindow *mainwin;
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *btn_get, *btn_compose;
	GtkWidget *paned_h, *paned_v;
	GtkWidget *folder_frame, *summary_frame, *message_frame;
	GtkWidget *label;
	GtkWidget *vbox;

	mainwin = g_new0(MainWindow, 1);

	/* Main Window */
	win = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(win), "Sylpheed (GTK4 Preview)");
	gtk_window_set_default_size(GTK_WINDOW(win), 1024, 700);
	mainwin->window = GTK_APPLICATION_WINDOW(win);

	/* Header Bar (GTK4 Modern Titlebar with Toolbar) */
	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);
	mainwin->header_bar = header_bar;

	/* Toolbar buttons */
	btn_get = gtk_button_new_with_label("受信");
	gtk_button_set_icon_name(GTK_BUTTON(btn_get), "mail-receive-symbolic");
	g_signal_connect(btn_get, "clicked", G_CALLBACK(on_get_mail_clicked), mainwin);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_get);

	btn_compose = gtk_button_new_with_label("作成");
	gtk_button_set_icon_name(GTK_BUTTON(btn_compose), "mail-message-new-symbolic");
	g_signal_connect(btn_compose, "clicked", G_CALLBACK(on_compose_clicked), mainwin);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_compose);

	/* Main vertical container */
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_window_set_child(GTK_WINDOW(win), vbox);

	/* 3-Pane Layout: Horizontal Paned (Left: FolderView, Right: Vertical Paned) */
	paned_h = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_paned_set_position(GTK_PANED(paned_h), 240);
	gtk_widget_set_vexpand(paned_h, TRUE);
	gtk_widget_set_hexpand(paned_h, TRUE);
	gtk_box_append(GTK_BOX(vbox), paned_h);
	mainwin->paned_main = paned_h;

	/* Left Pane: Folder Tree Placeholder */
	folder_frame = gtk_frame_new("フォルダ");
	label = gtk_label_new("フォルダツリー (GTK4 ColumnView/TreeListModel 移植予定)");
	gtk_frame_set_child(GTK_FRAME(folder_frame), label);
	gtk_paned_set_start_child(GTK_PANED(paned_h), folder_frame);
	mainwin->folder_view_box = folder_frame;

	/* Right Vertical Paned: Top (Summary/Mail List) + Bottom (Message View) */
	paned_v = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
	gtk_paned_set_position(GTK_PANED(paned_v), 300);
	gtk_paned_set_end_child(GTK_PANED(paned_h), paned_v);
	mainwin->paned_sub = paned_v;

	/* Top Right Pane: Summary/Mail List Placeholder */
	summary_frame = gtk_frame_new("メール一覧");
	label = gtk_label_new("メール一覧 (GTK4 GtkColumnView 移植予定)");
	gtk_frame_set_child(GTK_FRAME(summary_frame), label);
	gtk_paned_set_start_child(GTK_PANED(paned_v), summary_frame);
	mainwin->summary_view_box = summary_frame;

	/* Bottom Right Pane: Message View Placeholder */
	message_frame = gtk_frame_new("メッセージプレビュー");
	label = gtk_label_new("メッセージ本文 (GTK4 GtkTextView 移植予定)");
	gtk_frame_set_child(GTK_FRAME(message_frame), label);
	gtk_paned_set_end_child(GTK_PANED(paned_v), message_frame);
	mainwin->message_view_box = message_frame;

	/* Status Bar */
	mainwin->status_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
	gtk_widget_set_margin_start(mainwin->status_bar, 8);
	gtk_widget_set_margin_end(mainwin->status_bar, 8);
	gtk_widget_set_margin_top(mainwin->status_bar, 4);
	gtk_widget_set_margin_bottom(mainwin->status_bar, 4);
	mainwin->status_label = gtk_label_new("Sylpheed GTK4 Preview 準備完了");
	gtk_box_append(GTK_BOX(mainwin->status_bar), mainwin->status_label);
	gtk_box_append(GTK_BOX(vbox), mainwin->status_bar);

	return mainwin;
}
