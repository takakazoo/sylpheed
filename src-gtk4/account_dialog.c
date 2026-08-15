/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "account_dialog.h"
#include "setup.h"
#include <glib/gi18n.h>

#define TYPE_ACCT_ENTRY (acct_entry_get_type())
G_DECLARE_FINAL_TYPE(AcctEntry, acct_entry, ACCT, ENTRY, GObject)

struct _AcctEntry
{
	GObject parent_instance;
	char *name;
	char *protocol;
	char *server;
	gboolean is_default;
};

G_DEFINE_TYPE(AcctEntry, acct_entry, G_TYPE_OBJECT)

static void acct_entry_init(AcctEntry *self) {}
static void acct_entry_finalize(GObject *object)
{
	AcctEntry *self = (AcctEntry *)object;
	g_free(self->name);
	g_free(self->protocol);
	g_free(self->server);
	G_OBJECT_CLASS(acct_entry_parent_class)->finalize(object);
}

static void acct_entry_class_init(AcctEntryClass *klass)
{
	G_OBJECT_CLASS(klass)->finalize = acct_entry_finalize;
}

static AcctEntry *acct_entry_new(const char *name, const char *protocol, const char *server, gboolean is_default)
{
	AcctEntry *entry = g_object_new(TYPE_ACCT_ENTRY, NULL);
	entry->name = g_strdup(name);
	entry->protocol = g_strdup(protocol);
	entry->server = g_strdup(server);
	entry->is_default = is_default;
	return entry;
}

/* Edit Dialog */
void account_edit_dialog_show(GtkWindow *parent, const char *account_name)
{
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *notebook;
	GtkWidget *vbox_basic, *grid;
	GtkWidget *label;
	GtkWidget *entry_name, *entry_addr, *entry_srv, *entry_smtp, *entry_user;

	win = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(win), _("アカウントの設定 - Sylpheed GTK4"));
	gtk_window_set_default_size(GTK_WINDOW(win), 640, 480);
	if (parent) {
		gtk_window_set_transient_for(GTK_WINDOW(win), parent);
		gtk_window_set_modal(GTK_WINDOW(win), TRUE);
	}

	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);

	notebook = gtk_notebook_new();
	gtk_widget_set_margin_start(notebook, 12);
	gtk_widget_set_margin_end(notebook, 12);
	gtk_widget_set_margin_top(notebook, 12);
	gtk_widget_set_margin_bottom(notebook, 12);
	gtk_window_set_child(GTK_WINDOW(win), notebook);

	/* Basic Tab */
	vbox_basic = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
	gtk_widget_set_margin_start(vbox_basic, 16);
	gtk_widget_set_margin_end(vbox_basic, 16);
	gtk_widget_set_margin_top(vbox_basic, 16);
	gtk_widget_set_margin_bottom(vbox_basic, 16);

	grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 12);
	gtk_box_append(GTK_BOX(vbox_basic), grid);

	label = gtk_label_new(_("アカウント名:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
	entry_name = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(entry_name), account_name ? account_name : "Main Account");
	gtk_widget_set_hexpand(entry_name, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_name, 1, 0, 1, 1);

	label = gtk_label_new(_("メールアドレス:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
	entry_addr = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(entry_addr), "user@example.com");
	gtk_widget_set_hexpand(entry_addr, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_addr, 1, 1, 1, 1);

	label = gtk_label_new(_("受信サーバー:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);
	entry_srv = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(entry_srv), "imap.example.com");
	gtk_widget_set_hexpand(entry_srv, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_srv, 1, 2, 1, 1);

	label = gtk_label_new(_("送信サーバー (SMTP):"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 3, 1, 1);
	entry_smtp = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(entry_smtp), "smtp.example.com");
	gtk_widget_set_hexpand(entry_smtp, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_smtp, 1, 3, 1, 1);

	label = gtk_label_new(_("ユーザー名:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 4, 1, 1);
	entry_user = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(entry_user), "user@example.com");
	gtk_widget_set_hexpand(entry_user, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_user, 1, 4, 1, 1);

	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox_basic, gtk_label_new(_("基本")));

	/* SSL / TLS Tab */
	GtkWidget *vbox_ssl = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_widget_set_margin_start(vbox_ssl, 16);
	gtk_widget_set_margin_top(vbox_ssl, 16);
	GtkWidget *chk_ssl_pop = gtk_check_button_new_with_label(_("受信に SSL/TLS (ポート 993/995) を使用する"));
	gtk_check_button_set_active(GTK_CHECK_BUTTON(chk_ssl_pop), TRUE);
	GtkWidget *chk_ssl_smtp = gtk_check_button_new_with_label(_("送信 (SMTP) に STARTTLS (ポート 587) を使用する"));
	gtk_check_button_set_active(GTK_CHECK_BUTTON(chk_ssl_smtp), TRUE);
	gtk_box_append(GTK_BOX(vbox_ssl), chk_ssl_pop);
	gtk_box_append(GTK_BOX(vbox_ssl), chk_ssl_smtp);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox_ssl, gtk_label_new(_("SSL/TLS")));

	gtk_window_present(GTK_WINDOW(win));
}

/* Column Callbacks */
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

static void on_bind_default(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	AcctEntry *entry = (AcctEntry *)gtk_list_item_get_item(list_item);
	if (entry && label) gtk_label_set_text(GTK_LABEL(label), entry->is_default ? "⭐ 既定" : "");
}

static void on_bind_name(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	AcctEntry *entry = (AcctEntry *)gtk_list_item_get_item(list_item);
	if (entry && label) gtk_label_set_text(GTK_LABEL(label), entry->name);
}

static void on_bind_proto(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	AcctEntry *entry = (AcctEntry *)gtk_list_item_get_item(list_item);
	if (entry && label) gtk_label_set_text(GTK_LABEL(label), entry->protocol);
}

static void on_bind_server(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	AcctEntry *entry = (AcctEntry *)gtk_list_item_get_item(list_item);
	if (entry && label) gtk_label_set_text(GTK_LABEL(label), entry->server);
}

static void on_add_account(GtkButton *btn, gpointer user_data)
{
	GtkWindow *win = GTK_WINDOW(user_data);
	setup_wizard_show(win);
}

static void on_edit_account(GtkButton *btn, gpointer user_data)
{
	GtkWindow *win = GTK_WINDOW(user_data);
	account_edit_dialog_show(win, "Microsoft 365 / Outlook");
}

void account_manager_dialog_show(GtkWindow *parent)
{
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *btn_add, *btn_edit, *btn_delete, *btn_set_default;
	GtkWidget *vbox;
	GtkWidget *scrolled_win;
	GtkWidget *column_view;
	GListStore *store;
	GtkSingleSelection *selection;
	GtkListItemFactory *factory;
	GtkColumnViewColumn *col;

	win = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(win), _("アカウントの編集 - Sylpheed GTK4"));
	gtk_window_set_default_size(GTK_WINDOW(win), 680, 400);
	if (parent) {
		gtk_window_set_transient_for(GTK_WINDOW(win), parent);
		gtk_window_set_modal(GTK_WINDOW(win), TRUE);
	}

	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);

	/* Left Buttons */
	btn_add = gtk_button_new_with_label(_("新規追加"));
	gtk_button_set_icon_name(GTK_BUTTON(btn_add), "list-add-symbolic");
	g_signal_connect(btn_add, "clicked", G_CALLBACK(on_add_account), win);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_add);

	btn_edit = gtk_button_new_with_label(_("編集"));
	gtk_button_set_icon_name(GTK_BUTTON(btn_edit), "document-edit-symbolic");
	g_signal_connect(btn_edit, "clicked", G_CALLBACK(on_edit_account), win);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_edit);

	btn_delete = gtk_button_new();
	gtk_button_set_icon_name(GTK_BUTTON(btn_delete), "user-trash-symbolic");
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_delete);

	btn_set_default = gtk_button_new_with_label(_("既定に設定"));
	gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), btn_set_default);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_window_set_child(GTK_WINDOW(win), vbox);

	store = g_list_store_new(TYPE_ACCT_ENTRY);
	g_list_store_append(store, acct_entry_new("Microsoft 365 / Outlook", "IMAP4 (OAuth2)", "outlook.office365.com", TRUE));
	g_list_store_append(store, acct_entry_new("Gmail Account", "POP3 (OAuth2)", "pop.gmail.com", FALSE));
	g_list_store_append(store, acct_entry_new("Company Mail", "IMAP4 (SSL/TLS)", "mail.company.local", FALSE));

	selection = gtk_single_selection_new(G_LIST_MODEL(store));
	column_view = gtk_column_view_new(GTK_SELECTION_MODEL(selection));
	gtk_column_view_set_show_row_separators(GTK_COLUMN_VIEW(column_view), TRUE);

	/* Default Col */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_default), NULL);
	col = gtk_column_view_column_new(_("既定"), factory);
	gtk_column_view_column_set_fixed_width(col, 70);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(column_view), col);

	/* Name Col */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_name), NULL);
	col = gtk_column_view_column_new(_("アカウント名"), factory);
	gtk_column_view_column_set_fixed_width(col, 200);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(column_view), col);

	/* Protocol Col */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_proto), NULL);
	col = gtk_column_view_column_new(_("プロトコル"), factory);
	gtk_column_view_column_set_fixed_width(col, 140);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(column_view), col);

	/* Server Col */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_server), NULL);
	col = gtk_column_view_column_new(_("サーバー"), factory);
	gtk_column_view_column_set_expand(col, TRUE);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(column_view), col);

	scrolled_win = gtk_scrolled_window_new();
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_win), column_view);
	gtk_widget_set_vexpand(scrolled_win, TRUE);
	gtk_widget_set_hexpand(scrolled_win, TRUE);
	gtk_box_append(GTK_BOX(vbox), scrolled_win);

	gtk_window_present(GTK_WINDOW(win));
}
