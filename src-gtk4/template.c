/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "template.h"
#include "alertpanel.h"
#include <glib/gi18n.h>

#define TYPE_TMPL_ITEM (tmpl_item_get_type())
G_DECLARE_FINAL_TYPE(TmplItem, tmpl_item, TMPL, ITEM, GObject)

struct _TmplItem
{
	GObject parent_instance;
	char *name;
	char *subject;
	char *body;
};

G_DEFINE_TYPE(TmplItem, tmpl_item, G_TYPE_OBJECT)

static void tmpl_item_init(TmplItem *self) {}
static void tmpl_item_finalize(GObject *object)
{
	TmplItem *self = (TmplItem *)object;
	g_free(self->name);
	g_free(self->subject);
	g_free(self->body);
	G_OBJECT_CLASS(tmpl_item_parent_class)->finalize(object);
}

static void tmpl_item_class_init(TmplItemClass *klass)
{
	G_OBJECT_CLASS(klass)->finalize = tmpl_item_finalize;
}

static TmplItem *tmpl_item_new(const char *name, const char *subject, const char *body)
{
	TmplItem *item = g_object_new(TYPE_TMPL_ITEM, NULL);
	item->name = g_strdup(name);
	item->subject = g_strdup(subject);
	item->body = g_strdup(body);
	return item;
}

static void on_tmpl_save_clicked(GtkButton *btn, gpointer user_data)
{
	GtkWidget *win = GTK_WIDGET(user_data);
	g_print("[Template] テンプレートを保存しました。\n");
	gtk_window_destroy(GTK_WINDOW(win));
}

void template_edit_dialog_show(GtkWindow *parent, const char *name)
{
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *btn_save;
	GtkWidget *vbox, *grid;
	GtkWidget *label;
	GtkWidget *entry_name, *entry_subject;
	GtkWidget *scrolled_win, *text_view;
	GtkTextBuffer *buffer;

	win = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(win), _("テンプレートの編集 - Sylpheed GTK4"));
	gtk_window_set_default_size(GTK_WINDOW(win), 580, 420);
	if (parent) {
		gtk_window_set_transient_for(GTK_WINDOW(win), parent);
		gtk_window_set_modal(GTK_WINDOW(win), TRUE);
	}

	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);

	btn_save = gtk_button_new_with_label(_("保存"));
	gtk_widget_add_css_class(btn_save, "suggested-action");
	g_signal_connect(btn_save, "clicked", G_CALLBACK(on_tmpl_save_clicked), win);
	gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), btn_save);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_widget_set_margin_start(vbox, 16);
	gtk_widget_set_margin_end(vbox, 16);
	gtk_widget_set_margin_top(vbox, 16);
	gtk_widget_set_margin_bottom(vbox, 16);
	gtk_window_set_child(GTK_WINDOW(win), vbox);

	grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 12);
	gtk_box_append(GTK_BOX(vbox), grid);

	/* Name */
	label = gtk_label_new(_("テンプレート名:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
	entry_name = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(entry_name), name ? name : "ビジネス定型文");
	gtk_widget_set_hexpand(entry_name, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_name, 1, 0, 1, 1);

	/* Subject */
	label = gtk_label_new(_("件名:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
	entry_subject = gtk_entry_new();
	gtk_editable_set_text(GTK_EDITABLE(entry_subject), "ご連絡ありがとうございます (%s)");
	gtk_widget_set_hexpand(entry_subject, TRUE);
	gtk_grid_attach(GTK_GRID(grid), entry_subject, 1, 1, 1, 1);

	label = gtk_label_new(_("本文:"));
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);
	gtk_box_append(GTK_BOX(vbox), label);

	text_view = gtk_text_view_new();
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(text_view), 8);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(text_view), 8);
	gtk_text_view_set_top_margin(GTK_TEXT_VIEW(text_view), 8);
	gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(text_view), 8);

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
	gtk_text_buffer_set_text(buffer,
		"%t 様\n\n"
		"いつもお世話になっております。山田です。\n\n"
		"いただいたメール（件名: %s）を確認いたしました。\n\n"
		"-- \n"
		"山田 太郎 <user@example.com>\n", -1);

	scrolled_win = gtk_scrolled_window_new();
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_win), text_view);
	gtk_widget_set_vexpand(scrolled_win, TRUE);
	gtk_widget_set_hexpand(scrolled_win, TRUE);
	gtk_box_append(GTK_BOX(vbox), scrolled_win);

	gtk_window_present(GTK_WINDOW(win));
}

static void on_setup_label(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_label_new(NULL);
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);
	gtk_widget_set_margin_start(label, 8);
	gtk_widget_set_margin_end(label, 8);
	gtk_widget_set_margin_top(label, 4);
	gtk_widget_set_margin_bottom(label, 4);
	gtk_list_item_set_child(list_item, label);
}

static void on_bind_name(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	TmplItem *item = (TmplItem *)gtk_list_item_get_item(list_item);
	if (item && label) gtk_label_set_text(GTK_LABEL(label), item->name);
}

static void on_bind_subject(GtkSignalListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	TmplItem *item = (TmplItem *)gtk_list_item_get_item(list_item);
	if (item && label) gtk_label_set_text(GTK_LABEL(label), item->subject);
}

static void on_add_tmpl(GtkButton *btn, gpointer user_data)
{
	GtkWindow *win = GTK_WINDOW(user_data);
	template_edit_dialog_show(win, NULL);
}

static void on_edit_tmpl(GtkButton *btn, gpointer user_data)
{
	GtkWindow *win = GTK_WINDOW(user_data);
	template_edit_dialog_show(win, "ビジネス返信定型文");
}

void template_dialog_show(GtkWindow *parent)
{
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *btn_add, *btn_edit, *btn_delete;
	GtkWidget *scrolled_win;
	GtkWidget *column_view;
	GListStore *store;
	GtkSingleSelection *selection;
	GtkListItemFactory *factory;
	GtkColumnViewColumn *col;

	win = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(win), _("テンプレートの設定 - Sylpheed GTK4"));
	gtk_window_set_default_size(GTK_WINDOW(win), 640, 380);
	if (parent) {
		gtk_window_set_transient_for(GTK_WINDOW(win), parent);
		gtk_window_set_modal(GTK_WINDOW(win), TRUE);
	}

	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);

	btn_add = gtk_button_new_with_label(_("新規追加"));
	gtk_button_set_icon_name(GTK_BUTTON(btn_add), "list-add-symbolic");
	g_signal_connect(btn_add, "clicked", G_CALLBACK(on_add_tmpl), win);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_add);

	btn_edit = gtk_button_new_with_label(_("編集"));
	gtk_button_set_icon_name(GTK_BUTTON(btn_edit), "document-edit-symbolic");
	g_signal_connect(btn_edit, "clicked", G_CALLBACK(on_edit_tmpl), win);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_edit);

	btn_delete = gtk_button_new();
	gtk_button_set_icon_name(GTK_BUTTON(btn_delete), "user-trash-symbolic");
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_delete);

	store = g_list_store_new(TYPE_TMPL_ITEM);
	g_list_store_append(store, tmpl_item_new("ビジネス返信定型文", "ご連絡ありがとうございます (%s)", "本文..."));
	g_list_store_append(store, tmpl_item_new("社内日報", "【日報】%d 山田", "本日の業務内容..."));

	selection = gtk_single_selection_new(G_LIST_MODEL(store));
	column_view = gtk_column_view_new(GTK_SELECTION_MODEL(selection));
	gtk_column_view_set_show_row_separators(GTK_COLUMN_VIEW(column_view), TRUE);

	/* Name Col */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_name), NULL);
	col = gtk_column_view_column_new(_("テンプレート名"), factory);
	gtk_column_view_column_set_fixed_width(col, 220);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(column_view), col);

	/* Subject Col */
	factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(on_setup_label), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(on_bind_subject), NULL);
	col = gtk_column_view_column_new(_("件名パターン"), factory);
	gtk_column_view_column_set_expand(col, TRUE);
	gtk_column_view_column_set_resizable(col, TRUE);
	gtk_column_view_append_column(GTK_COLUMN_VIEW(column_view), col);

	scrolled_win = gtk_scrolled_window_new();
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_win), column_view);
	gtk_widget_set_vexpand(scrolled_win, TRUE);
	gtk_widget_set_hexpand(scrolled_win, TRUE);
	gtk_window_set_child(GTK_WINDOW(win), scrolled_win);

	gtk_window_present(GTK_WINDOW(win));
}
