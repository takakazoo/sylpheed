/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __SUMMARYVIEW_GTK4_H__
#define __SUMMARYVIEW_GTK4_H__

#include <gtk/gtk.h>
#include "procmsg.h"

#define TYPE_MSG_ITEM (msg_item_get_type())
G_DECLARE_FINAL_TYPE(MsgItem, msg_item, MSG, ITEM, GObject)

struct _MsgItem
{
	GObject parent_instance;
	char *from;
	char *to;
	char *subject;
	char *date;
	char *size;
	char *body;
	char *attach_file;
	char *attach_size;
	gboolean unread;
	gboolean marked;
};

typedef struct _SummaryView SummaryView;

struct _SummaryView
{
	GtkWidget *container;
	GtkWidget *search_entry;
	GtkWidget *column_view;
	GListStore *store;
	GtkFilterListModel *filter_model;
	GtkCustomFilter *filter;
	GtkSingleSelection *selection;

	void (*message_selected_cb)(SummaryView *summaryview, MsgItem *item, gpointer user_data);
	gpointer user_data;
};

SummaryView *summary_view_create(void);
void summary_view_set_selected_callback(SummaryView *summaryview,
					void (*cb)(SummaryView *summaryview, MsgItem *item, gpointer user_data),
					gpointer user_data);
void summary_view_load_folder(SummaryView *summaryview, gpointer folder_item);

#endif /* __SUMMARYVIEW_GTK4_H__ */
