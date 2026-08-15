/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "search_dialog.h"
#include "summaryview.h"
#include <glib/gi18n.h>

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

void search_dialog_show(GtkWindow *parent)
{
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *btn_search;
	GtkWidget *vbox, *grid;
	GtkWidget *label;
	GtkWidget *entry_from, *entry_to, *entry_subject, *entry_body;
	GtkWidget *scrolled_win, *column_view;
	GListStore *store;
	GtkSingleSelection *selection;
	GtkListItemFactory *factory;
	GtkColumnViewColumn *col;

	win = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(win), _("メッセージの検索 - Sylpheed GTK4"));
	gtk_window_set_default_size(GTK_WINDOW(win), 760, 520);
	if (parent) {
		gtk_window_set_transient_for(GTK_WINDOW(win), parent);
	}

	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);

	btn_search = gtk_button_new_with_label(_("検索開始"));
	gtk_button_set_icon_name(GTK_BUTTON(btn_search), "system-search-symbolic");
	gtk_widget_add_css_class(btn_search, "suggested-action");
	gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), btn_search);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_widget_set_margin_start(vbox, 14);
	gtk_widget_set_margin_end(vbox, 14);
	gtk_widget_set_margin_top(vbox, 10);
	gtk_widget_set_margin_bottom(vbox, 10);
	gtk_window_set_child(GTK_WINDOW(win), vbox);

	/* Criteria Grid */
	grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 6);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 12);
	gtk_box_append(GTK_BOX(vbox), grid);

	/* From */
	label = gtk_label_new(_("差出人:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
	entry_from = gtk_entry_new();
	gtk_widget_set_hexpand(entry_from, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_from, 1, 0, 1, 1);

	/* To */
	label = gtk_label_new(_("宛先:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 2, 0, 1, 1);
	entry_to = gtk_entry_new();
	gtk_widget_set_hexpand(entry_to, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_to, 3, 0, 1, 1);

	/* Subject */
	label = gtk_label_new(_("件名:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
	entry_subject = gtk_entry_new();
	gtk_widget_set_hexpand(entry_subject, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_subject, 1, 1, 1, 1);

	/* Body */
	label = gtk_label_new(_("本文:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 2, 1, 1, 1);
	entry_body = gtk_entry_new();
	gtk_widget_set_hexpand(entry_body, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_body, 3, 1, 1, 1);

	gtk_box_append(GTK_BOX(vbox), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL));

	/* Results Table */
	label = gtk_label_new(_("検索結果:"));
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);
	gtk_box_append(GTK_BOX(vbox), label);

	store = g_list_store_new(TYPE_MSG_ITEM);
	selection = gtk_single_selection_new(G_LIST_MODEL(store));
	column_view = gtk_column_view_new(GTK_SELECTION_MODEL(selection));
	gtk_column_view_set_show_row_separators(GTK_COLUMN_VIEW(column_view), TRUE);

	/* From Col */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_from), NULL);
	col = gtk_column_view_column_new(_("差出人"), factory);
	gtk_column_view_column_set_fixed_width(col, 200);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(column_view), col);

	/* Subject Col */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_subject), NULL);
	col = gtk_column_view_column_new(_("件名"), factory);
	gtk_column_view_column_set_expand(col, TRUE);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(column_view), col);

	/* Date Col */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_date), NULL);
	col = gtk_column_view_column_new(_("日時"), factory);
	gtk_column_view_column_set_fixed_width(col, 150);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(column_view), col);

	scrolled_win = gtk_scrolled_window_new();
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_win), column_view);
	gtk_widget_set_vexpand(scrolled_win, TRUE);
	gtk_widget_set_hexpand(scrolled_win, TRUE);
	gtk_box_append(GTK_BOX(vbox), scrolled_win);

	gtk_window_present(GTK_WINDOW(win));
}
