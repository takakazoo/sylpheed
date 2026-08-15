/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __SUMMARYVIEW_GTK4_H__
#define __SUMMARYVIEW_GTK4_H__

#include <gtk/gtk.h>
#include "procmsg.h"

typedef struct _SummaryView SummaryView;

struct _SummaryView
{
	GtkWidget *container;
	GtkWidget *column_view;
	GListStore *store;
	GtkSingleSelection *selection;

	void (*message_selected_cb)(SummaryView *summaryview, const char *msg_id, gpointer user_data);
	gpointer user_data;
};

SummaryView *summary_view_create(void);
void summary_view_set_selected_callback(SummaryView *summaryview,
					void (*cb)(SummaryView *summaryview, const char *msg_id, gpointer user_data),
					gpointer user_data);
void summary_view_load_folder(SummaryView *summaryview, gpointer folder_item);

#endif /* __SUMMARYVIEW_GTK4_H__ */
