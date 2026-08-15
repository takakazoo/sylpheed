/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __FOLDERVIEW_GTK4_H__
#define __FOLDERVIEW_GTK4_H__

#include <gtk/gtk.h>
#include "folder.h"

typedef struct _FolderView FolderView;

struct _FolderView
{
	GtkWidget *container;
	GtkWidget *list_view;
	GtkStringList *model;
	GtkSingleSelection *selection;

	FolderItem *selected_item;
	void (*folder_selected_cb)(FolderView *folderview, FolderItem *item, gpointer user_data);
	gpointer user_data;
};

FolderView *folder_view_create(void);
void folder_view_set_selected_callback(FolderView *folderview,
				       void (*cb)(FolderView *folderview, FolderItem *item, gpointer user_data),
				       gpointer user_data);
void folder_view_refresh(FolderView *folderview);

#endif /* __FOLDERVIEW_GTK4_H__ */
