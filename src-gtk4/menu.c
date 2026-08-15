/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "menu.h"
#include "mainwindow.h"
#include "about.h"
#include "compose.h"
#include "prefs_dialog.h"
#include "setup.h"
#include "addressbook.h"
#include "account_dialog.h"
#include "logwindow.h"
#include "filter_dialog.h"
#include "search_dialog.h"
#include <glib/gi18n.h>

static void action_get_mail(GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
	g_print("[Action] 受信 (Get mail)\n");
	log_window_append("[POP3] Connecting to server to check for new mail...\n");
}

static void action_get_all_mail(GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
	g_print("[Action] 全受信 (Get all mail)\n");
	log_window_append("[POP3/IMAP] Checking all accounts for new mail...\n");
}

static void action_compose(GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
	MainWindow *mainwin = (MainWindow *)user_data;
	ComposeWindow *compose = compose_window_new(mainwin ? GTK_WINDOW(mainwin->window) : NULL, NULL, NULL, NULL);
	compose_window_show(compose);
}

static void action_reply(GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
	MainWindow *mainwin = (MainWindow *)user_data;
	ComposeWindow *compose = compose_window_new(mainwin ? GTK_WINDOW(mainwin->window) : NULL,
						    "sender@example.com",
						    "Re: Welcome to Sylpheed GTK4 Preview!",
						    "\n\n--- Original Message ---\n> Sylpheed の GTK4 プレビュー版へようこそ！\n");
	compose_window_show(compose);
}

static void action_forward(GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
	MainWindow *mainwin = (MainWindow *)user_data;
	ComposeWindow *compose = compose_window_new(mainwin ? GTK_WINDOW(mainwin->window) : NULL,
						    NULL,
						    "Fw: Welcome to Sylpheed GTK4 Preview!",
						    "\n\n--- Forwarded Message ---\nFrom: Sylpheed Team <sylpheed@sraoss.jp>\nSubject: Welcome to Sylpheed GTK4 Preview!\n");
	compose_window_show(compose);
}

static void action_delete(GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
	g_print("[Action] 削除 (Delete)\n");
}

static void action_search(GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
	MainWindow *mainwin = (MainWindow *)user_data;
	search_dialog_show(mainwin ? GTK_WINDOW(mainwin->window) : NULL);
}

static void action_filter(GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
	MainWindow *mainwin = (MainWindow *)user_data;
	filter_dialog_show(mainwin ? GTK_WINDOW(mainwin->window) : NULL);
}

static void action_preferences(GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
	MainWindow *mainwin = (MainWindow *)user_data;
	prefs_dialog_show(mainwin ? GTK_WINDOW(mainwin->window) : NULL);
}

static void action_account_manager(GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
	MainWindow *mainwin = (MainWindow *)user_data;
	account_manager_dialog_show(mainwin ? GTK_WINDOW(mainwin->window) : NULL);
}

static void action_addressbook(GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
	MainWindow *mainwin = (MainWindow *)user_data;
	addressbook_window_show(mainwin ? GTK_WINDOW(mainwin->window) : NULL);
}

static void action_setup(GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
	MainWindow *mainwin = (MainWindow *)user_data;
	setup_wizard_show(mainwin ? GTK_WINDOW(mainwin->window) : NULL);
}

static void action_log(GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
	MainWindow *mainwin = (MainWindow *)user_data;
	log_window_show(mainwin ? GTK_WINDOW(mainwin->window) : NULL);
}

static void action_about(GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
	MainWindow *mainwin = (MainWindow *)user_data;
	about_dialog_show(mainwin ? GTK_WINDOW(mainwin->window) : NULL);
}

static void action_quit(GSimpleAction *action, GVariant *parameter, gpointer user_data)
{
	GtkApplication *app = (GtkApplication *)user_data;
	g_application_quit(G_APPLICATION(app));
}

static const GActionEntry app_entries[] = {
	{ "inc", action_get_mail, NULL, NULL, NULL },
	{ "inc-all", action_get_all_mail, NULL, NULL, NULL },
	{ "compose", action_compose, NULL, NULL, NULL },
	{ "reply", action_reply, NULL, NULL, NULL },
	{ "forward", action_forward, NULL, NULL, NULL },
	{ "delete", action_delete, NULL, NULL, NULL },
	{ "search", action_search, NULL, NULL, NULL },
	{ "filter", action_filter, NULL, NULL, NULL },
	{ "preferences", action_preferences, NULL, NULL, NULL },
	{ "account-manager", action_account_manager, NULL, NULL, NULL },
	{ "addressbook", action_addressbook, NULL, NULL, NULL },
	{ "setup", action_setup, NULL, NULL, NULL },
	{ "log", action_log, NULL, NULL, NULL },
	{ "about", action_about, NULL, NULL, NULL },
	{ "quit", action_quit, NULL, NULL, NULL }
};

void menu_init_actions(GtkApplication *app, gpointer mainwin)
{
	g_action_map_add_action_entries(G_ACTION_MAP(app), app_entries,
					G_N_ELEMENTS(app_entries), mainwin ? mainwin : (gpointer)app);

	/* Keyboard Shortcuts (Accelerators) */
	const char *accel_inc[] = { "<Primary>i", NULL };
	const char *accel_inc_all[] = { "<Primary><Shift>i", NULL };
	const char *accel_compose[] = { "<Primary>n", NULL };
	const char *accel_search[] = { "<Primary>f", NULL };
	const char *accel_quit[] = { "<Primary>q", NULL };
	const char *accel_delete[] = { "Delete", NULL };

	gtk_application_set_accels_for_action(app, "app.inc", accel_inc);
	gtk_application_set_accels_for_action(app, "app.inc-all", accel_inc_all);
	gtk_application_set_accels_for_action(app, "app.compose", accel_compose);
	gtk_application_set_accels_for_action(app, "app.search", accel_search);
	gtk_application_set_accels_for_action(app, "app.quit", accel_quit);
	gtk_application_set_accels_for_action(app, "app.delete", accel_delete);
}

GMenuModel *menu_create_main_menu(void)
{
	GMenu *menubar = g_menu_new();

	/* File Menu */
	GMenu *file_menu = g_menu_new();
	g_menu_append(file_menu, _("新着メールの受信"), "app.inc");
	g_menu_append(file_menu, _("全アカウントの新着メールを受信"), "app.inc-all");
	g_menu_append(file_menu, _("新規メッセージの作成"), "app.compose");
	g_menu_append(file_menu, _("アカウントの新規作成..."), "app.setup");
	g_menu_append(file_menu, _("終了"), "app.quit");
	g_menu_append_submenu(menubar, _("ファイル"), G_MENU_MODEL(file_menu));

	/* Edit Menu */
	GMenu *edit_menu = g_menu_new();
	g_menu_append(edit_menu, _("メッセージの検索..."), "app.search");
	g_menu_append(edit_menu, _("全般の設定..."), "app.preferences");
	g_menu_append_submenu(menubar, _("編集"), G_MENU_MODEL(edit_menu));

	/* Message Menu */
	GMenu *msg_menu = g_menu_new();
	g_menu_append(msg_menu, _("返信"), "app.reply");
	g_menu_append(msg_menu, _("転送"), "app.forward");
	g_menu_append(msg_menu, _("削除"), "app.delete");
	g_menu_append_submenu(menubar, _("メッセージ"), G_MENU_MODEL(msg_menu));

	/* Configuration Menu */
	GMenu *config_menu = g_menu_new();
	g_menu_append(config_menu, _("全般の設定..."), "app.preferences");
	g_menu_append(config_menu, _("アカウントの設定..."), "app.account-manager");
	g_menu_append(config_menu, _("アカウントの新規作成..."), "app.setup");
	g_menu_append_submenu(menubar, _("設定"), G_MENU_MODEL(config_menu));

	/* Tools Menu */
	GMenu *tool_menu = g_menu_new();
	g_menu_append(tool_menu, _("アドレス帳"), "app.addressbook");
	g_menu_append(tool_menu, _("振り分けの設定..."), "app.filter");
	g_menu_append(tool_menu, _("プロトコルログ"), "app.log");
	g_menu_append_submenu(menubar, _("ツール"), G_MENU_MODEL(tool_menu));

	/* Help Menu */
	GMenu *help_menu = g_menu_new();
	g_menu_append(help_menu, _("Sylpheedについて"), "app.about");
	g_menu_append_submenu(menubar, _("ヘルプ"), G_MENU_MODEL(help_menu));

	return G_MENU_MODEL(menubar);
}

GMenuModel *menu_create_app_menu(void)
{
	GMenu *menu = g_menu_new();
	GMenu *section1 = g_menu_new();
	g_menu_append(section1, _("新着メールの受信"), "app.inc");
	g_menu_append(section1, _("全アカウントの受信"), "app.inc-all");
	g_menu_append(section1, _("新規メッセージ作成"), "app.compose");
	g_menu_append(section1, _("アカウント新規作成"), "app.setup");
	g_menu_append_section(menu, NULL, G_MENU_MODEL(section1));

	GMenu *section2 = g_menu_new();
	g_menu_append(section2, _("メッセージ検索"), "app.search");
	g_menu_append(section2, _("振り分け設定"), "app.filter");
	g_menu_append(section2, _("アドレス帳"), "app.addressbook");
	g_menu_append(section2, _("アカウント設定"), "app.account-manager");
	g_menu_append(section2, _("プロトコルログ"), "app.log");
	g_menu_append(section2, _("全般の設定"), "app.preferences");
	g_menu_append_section(menu, NULL, G_MENU_MODEL(section2));

	GMenu *section3 = g_menu_new();
	g_menu_append(section3, _("Sylpheedについて"), "app.about");
	g_menu_append(section3, _("終了"), "app.quit");
	g_menu_append_section(menu, NULL, G_MENU_MODEL(section3));

	return G_MENU_MODEL(menu);
}
