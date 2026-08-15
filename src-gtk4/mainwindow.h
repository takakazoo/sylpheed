/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __MAINWINDOW_GTK4_H__
#define __MAINWINDOW_GTK4_H__

#include <gtk/gtk.h>

typedef struct _MainWindow MainWindow;

struct _MainWindow
{
	GtkApplicationWindow *window;
	GtkWidget *header_bar;
	GtkWidget *paned_main;
	GtkWidget *paned_sub;

	/* 3 Panes */
	GtkWidget *folder_view_box;
	GtkWidget *summary_view_box;
	GtkWidget *message_view_box;

	GtkWidget *status_bar;
	GtkWidget *status_label;
};

MainWindow *main_window_create(GtkApplication *app);

#endif /* __MAINWINDOW_GTK4_H__ */
