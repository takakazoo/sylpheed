/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __MAINWINDOW_GTK4_H__
#define __MAINWINDOW_GTK4_H__

#include <gtk/gtk.h>
#include "folderview.h"
#include "summaryview.h"
#include "messageview.h"

typedef struct _MainWindow MainWindow;

struct _MainWindow
{
	GtkApplicationWindow *window;
	GtkWidget *header_bar;
	GtkWidget *paned_main;
	GtkWidget *paned_sub;

	/* 3 Panes Components */
	FolderView *folderview;
	SummaryView *summaryview;
	MessageView *messageview;

	GtkWidget *status_bar;
	GtkWidget *status_label;
};

MainWindow *main_window_create(GtkApplication *app);

#endif /* __MAINWINDOW_GTK4_H__ */
