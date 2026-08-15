/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "mainwindow.h"
#include "menu.h"
#include <glib/gi18n.h>

MainWindow *main_window_create(GtkApplication *app)
{
	MainWindow *mainwin;
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *btn_get, *btn_compose, *btn_reply, *btn_forward, *btn_delete;
	GtkWidget *btn_menu;
	GtkWidget *paned_h, *paned_v;
	GtkWidget *folder_frame, *summary_frame, *message_frame;
	GtkWidget *label;
	GtkWidget *vbox;
	GtkWidget *menubar;
	GMenuModel *app_menu_model;
	GMenuModel *main_menu_model;

	mainwin = g_new0(MainWindow, 1);

	/* Register Global Application Actions */
	menu_init_actions(app, mainwin);

	/* Main Window */
	win = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(win), "Sylpheed (GTK4 Preview)");
	gtk_window_set_default_size(GTK_WINDOW(win), 1080, 720);
	mainwin->window = GTK_APPLICATION_WINDOW(win);

	/* Header Bar (Modern Titlebar with Tools) */
	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);
	mainwin->header_bar = header_bar;

	/* Left Tool Buttons */
	btn_get = gtk_button_new_with_label(_("受信"));
	gtk_button_set_icon_name(GTK_BUTTON(btn_get), "mail-receive-symbolic");
	gtk_actionable_set_action_name(GTK_ACTIONABLE(btn_get), "app.inc");
	gtk_widget_set_tooltip_text(btn_get, _("新着メールの受信 (Ctrl+I)"));
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_get);

	btn_compose = gtk_button_new_with_label(_("作成"));
	gtk_button_set_icon_name(GTK_BUTTON(btn_compose), "mail-message-new-symbolic");
	gtk_actionable_set_action_name(GTK_ACTIONABLE(btn_compose), "app.compose");
	gtk_widget_set_tooltip_text(btn_compose, _("新規メッセージの作成 (Ctrl+N)"));
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_compose);

	btn_reply = gtk_button_new();
	gtk_button_set_icon_name(GTK_BUTTON(btn_reply), "mail-reply-sender-symbolic");
	gtk_actionable_set_action_name(GTK_ACTIONABLE(btn_reply), "app.reply");
	gtk_widget_set_tooltip_text(btn_reply, _("返信"));
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_reply);

	btn_forward = gtk_button_new();
	gtk_button_set_icon_name(GTK_BUTTON(btn_forward), "mail-forward-symbolic");
	gtk_actionable_set_action_name(GTK_ACTIONABLE(btn_forward), "app.forward");
	gtk_widget_set_tooltip_text(btn_forward, _("転送"));
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_forward);

	btn_delete = gtk_button_new();
	gtk_button_set_icon_name(GTK_BUTTON(btn_delete), "user-trash-symbolic");
	gtk_actionable_set_action_name(GTK_ACTIONABLE(btn_delete), "app.delete");
	gtk_widget_set_tooltip_text(btn_delete, _("削除 (Delete)"));
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_delete);

	/* Right Hamburger Menu Button */
	app_menu_model = menu_create_app_menu();
	btn_menu = gtk_menu_button_new();
	gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(btn_menu), "open-menu-symbolic");
	gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(btn_menu), app_menu_model);
	gtk_widget_set_tooltip_text(btn_menu, _("メインメニュー"));
	gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), btn_menu);

	/* Main vertical layout */
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_window_set_child(GTK_WINDOW(win), vbox);

	/* Optional Top Menu Bar (for traditional desktop feel) */
	main_menu_model = menu_create_main_menu();
	menubar = gtk_popover_menu_bar_new_from_model(main_menu_model);
	gtk_box_append(GTK_BOX(vbox), menubar);

	/* 3-Pane Layout */
	paned_h = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_paned_set_position(GTK_PANED(paned_h), 260);
	gtk_widget_set_vexpand(paned_h, TRUE);
	gtk_widget_set_hexpand(paned_h, TRUE);
	gtk_box_append(GTK_BOX(vbox), paned_h);
	mainwin->paned_main = paned_h;

	/* Left Pane: Folder Tree */
	folder_frame = gtk_frame_new(_("フォルダ"));
	label = gtk_label_new("📁 メールボックス / フォルダツリー\n(Phase 3 でデータ連携予定)");
	gtk_frame_set_child(GTK_FRAME(folder_frame), label);
	gtk_paned_set_start_child(GTK_PANED(paned_h), folder_frame);
	mainwin->folder_view_box = folder_frame;

	/* Right Vertical Paned */
	paned_v = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
	gtk_paned_set_position(GTK_PANED(paned_v), 300);
	gtk_paned_set_end_child(GTK_PANED(paned_h), paned_v);
	mainwin->paned_sub = paned_v;

	/* Top Right Pane: Summary View */
	summary_frame = gtk_frame_new(_("メール一覧"));
	label = gtk_label_new("✉️ 件名 / 送信者 / 日時 カラム一覧\n(Phase 3 で GtkColumnView 連携予定)");
	gtk_frame_set_child(GTK_FRAME(summary_frame), label);
	gtk_paned_set_start_child(GTK_PANED(paned_v), summary_frame);
	mainwin->summary_view_box = summary_frame;

	/* Bottom Right Pane: Message View */
	message_frame = gtk_frame_new(_("メッセージプレビュー"));
	label = gtk_label_new("📄 メッセージ本文 / ヘッダ表示\n(Phase 4 で GtkTextView 連携予定)");
	gtk_frame_set_child(GTK_FRAME(message_frame), label);
	gtk_paned_set_end_child(GTK_PANED(paned_v), message_frame);
	mainwin->message_view_box = message_frame;

	/* Status Bar */
	mainwin->status_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
	gtk_widget_set_margin_start(mainwin->status_bar, 12);
	gtk_widget_set_margin_end(mainwin->status_bar, 12);
	gtk_widget_set_margin_top(mainwin->status_bar, 4);
	gtk_widget_set_margin_bottom(mainwin->status_bar, 4);

	mainwin->status_label = gtk_label_new(_("Sylpheed GTK4 準備完了 | ショートカット: 受信(Ctrl+I) 作成(Ctrl+N) 検索(Ctrl+F) 終了(Ctrl+Q)"));
	gtk_box_append(GTK_BOX(mainwin->status_bar), mainwin->status_label);
	gtk_box_append(GTK_BOX(vbox), mainwin->status_bar);

	return mainwin;
}
