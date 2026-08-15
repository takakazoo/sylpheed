/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "filter_dialog.h"
#include <glib/gi18n.h>

#define TYPE_FILTER_RULE (filter_rule_get_type())
G_DECLARE_FINAL_TYPE(FilterRule, filter_rule, FILTER, RULE, GObject)

struct _FilterRule
{
	GObject parent_instance;
	char *name;
	char *condition;
	char *action;
	gboolean enabled;
};

G_DEFINE_TYPE(FilterRule, filter_rule, G_TYPE_OBJECT)

static void filter_rule_init(FilterRule *self) {}
static void filter_rule_finalize(GObject *object)
{
	FilterRule *self = (FilterRule *)object;
	g_free(self->name);
	g_free(self->condition);
	g_free(self->action);
	G_OBJECT_CLASS(filter_rule_parent_class)->finalize(object);
}

static void filter_rule_class_init(FilterRuleClass *klass)
{
	G_OBJECT_CLASS(klass)->finalize = filter_rule_finalize;
}

static FilterRule *filter_rule_new(const char *name, const char *condition, const char *action, gboolean enabled)
{
	FilterRule *rule = g_object_new(TYPE_FILTER_RULE, NULL);
	rule->name = g_strdup(name);
	rule->condition = g_strdup(condition);
	rule->action = g_strdup(action);
	rule->enabled = enabled;
	return rule;
}

void filter_edit_dialog_show(GtkWindow *parent, const char *rule_name)
{
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *vbox, *grid;
	GtkWidget *label;
	GtkWidget *entry_name, *entry_cond_val, *entry_target;
	GtkWidget *dropdown_header, *dropdown_op, *dropdown_action;
	GtkStringList *headers_model, *ops_model, *actions_model;

	win = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(win), _("振り分けルールの編集 - Sylpheed GTK4"));
	gtk_window_set_default_size(GTK_WINDOW(win), 600, 360);
	if (parent) {
		gtk_window_set_transient_for(GTK_WINDOW(win), parent);
		gtk_window_set_modal(GTK_WINDOW(win), TRUE);
	}

	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
	gtk_widget_set_margin_start(vbox, 16);
	gtk_widget_set_margin_end(vbox, 16);
	gtk_widget_set_margin_top(vbox, 16);
	gtk_widget_set_margin_bottom(vbox, 16);
	gtk_window_set_child(GTK_WINDOW(win), vbox);

	grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 12);
	gtk_box_append(GTK_BOX(vbox), grid);

	/* Rule Name */
	label = gtk_label_new(_("ルール名:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
	entry_name = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(entry_name), rule_name ? rule_name : "New Filter Rule");
	gtk_widget_set_hexpand(entry_name, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_name, 1, 0, 2, 1);

	/* Condition */
	label = gtk_label_new(_("条件:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);

	const char *headers[] = { "From (差出人)", "To (宛先)", "Subject (件名)", "Body (本文)", "Cc", NULL };
	headers_model = gtk_string_list_new(headers);
	dropdown_header = gtk_drop_down_new(G_LIST_MODEL(headers_model), NULL);
	gtk_grid_attach(GTK_GRID(grid), dropdown_header, 1, 1, 1, 1);

	const char *ops[] = { "を含む", "を含まない", "で始まる", "で終わる", NULL };
	ops_model = gtk_string_list_new(ops);
	dropdown_op = gtk_drop_down_new(G_LIST_MODEL(ops_model), NULL);
	gtk_grid_attach(GTK_GRID(grid), dropdown_op, 2, 1, 1, 1);

	/* Value */
	label = gtk_label_new(_("検索文字列:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);
	entry_cond_val = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(entry_cond_val), "dev@example.com");
	gtk_widget_set_hexpand(entry_cond_val, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_cond_val, 1, 2, 2, 1);

	/* Action */
	label = gtk_label_new(_("アクション:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 3, 1, 1);

	const char *actions[] = { "フォルダへ移動", "フォルダへコピー", "削除", "マーク", "迷惑メールに判定", NULL };
	actions_model = gtk_string_list_new(actions);
	dropdown_action = gtk_drop_down_new(G_LIST_MODEL(actions_model), NULL);
	gtk_grid_attach(GTK_GRID(grid), dropdown_action, 1, 3, 1, 1);

	entry_target = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(entry_target), "INBOX/Dev");
	gtk_widget_set_hexpand(entry_target, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_target, 2, 3, 1, 1);

	gtk_window_present(GTK_WINDOW(win));
}

/* List Callbacks */
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

static void on_bind_enabled(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	FilterRule *rule = (FilterRule *)gtk_list_item_get_item(list_item);
	if (rule && label) gtk_label_set_text(GTK_LABEL(label), rule->enabled ? "✅ 有効" : "⏸️ 無効");
}

static void on_bind_name(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	FilterRule *rule = (FilterRule *)gtk_list_item_get_item(list_item);
	if (rule && label) gtk_label_set_text(GTK_LABEL(label), rule->name);
}

static void on_bind_condition(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	FilterRule *rule = (FilterRule *)gtk_list_item_get_item(list_item);
	if (rule && label) gtk_label_set_text(GTK_LABEL(label), rule->condition);
}

static void on_bind_action(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	FilterRule *rule = (FilterRule *)gtk_list_item_get_item(list_item);
	if (rule && label) gtk_label_set_text(GTK_LABEL(label), rule->action);
}

static void on_add_rule(GtkButton *btn, gpointer user_data)
{
	GtkWindow *win = GTK_WINDOW(user_data);
	filter_edit_dialog_show(win, NULL);
}

static void on_edit_rule(GtkButton *btn, gpointer user_data)
{
	GtkWindow *win = GTK_WINDOW(user_data);
	filter_edit_dialog_show(win, "Microsoft 365 通知振り分け");
}

void filter_dialog_show(GtkWindow *parent)
{
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *btn_add, *btn_edit, *btn_delete;
	GtkWidget *vbox;
	GtkWidget *scrolled_win;
	GtkWidget *column_view;
	GListStore *store;
	GtkSingleSelection *selection;
	GtkListItemFactory *factory;
	GtkColumnViewColumn *col;

	win = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(win), _("振り分けの設定 - Sylpheed GTK4"));
	gtk_window_set_default_size(GTK_WINDOW(win), 720, 420);
	if (parent) {
		gtk_window_set_transient_for(GTK_WINDOW(win), parent);
		gtk_window_set_modal(GTK_WINDOW(win), TRUE);
	}

	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);

	btn_add = gtk_button_new_with_label(_("新規追加"));
	gtk_button_set_icon_name(GTK_BUTTON(btn_add), "list-add-symbolic");
	g_signal_connect(btn_add, "clicked", G_CALLBACK(on_add_rule), win);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_add);

	btn_edit = gtk_button_new_with_label(_("編集"));
	gtk_button_set_icon_name(GTK_BUTTON(btn_edit), "document-edit-symbolic");
	g_signal_connect(btn_edit, "clicked", G_CALLBACK(on_edit_rule), win);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_edit);

	btn_delete = gtk_button_new();
	gtk_button_set_icon_name(GTK_BUTTON(btn_delete), "user-trash-symbolic");
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_delete);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_window_set_child(GTK_WINDOW(win), vbox);

	store = g_list_store_new(TYPE_FILTER_RULE);
	g_list_store_append(store, filter_rule_new("Microsoft 365 通知振り分け", "From に @microsoft.com を含む", "INBOX/Microsoft へ移動", TRUE));
	g_list_store_append(store, filter_rule_new("開発ML振り分け", "Subject に [sylpheed-dev] を含む", "INBOX/Dev へ移動", TRUE));
	g_list_store_append(store, filter_rule_new("迷惑メール自動削除", "Subject に [SPAM] を含む", "ごみ箱へ移動", TRUE));

	selection = gtk_single_selection_new(G_LIST_MODEL(store));
	column_view = gtk_column_view_new(GTK_SELECTION_MODEL(selection));
	gtk_column_view_set_show_row_separators(GTK_COLUMN_VIEW(column_view), TRUE);

	/* Status Col */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_enabled), NULL);
	col = gtk_column_view_column_new(_("状態"), factory);
	gtk_column_view_column_set_fixed_width(col, 80);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(column_view), col);

	/* Name Col */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_name), NULL);
	col = gtk_column_view_column_new(_("ルール名"), factory);
	gtk_column_view_column_set_fixed_width(col, 200);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(column_view), col);

	/* Condition Col */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_condition), NULL);
	col = gtk_column_view_column_new(_("条件"), factory);
	gtk_column_view_column_set_fixed_width(col, 200);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(column_view), col);

	/* Action Col */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_action), NULL);
	col = gtk_column_view_column_new(_("アクション"), factory);
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
