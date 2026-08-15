/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __LOGWINDOW_GTK4_H__
#define __LOGWINDOW_GTK4_H__

#include <gtk/gtk.h>

typedef struct _LogWindow LogWindow;

struct _LogWindow
{
	GtkWidget *window;
	GtkWidget *header_bar;
	GtkWidget *text_view;
	GtkTextBuffer *buffer;
};

LogWindow *log_window_get_instance(void);
void log_window_show(GtkWindow *parent);
void log_window_append(const char *str);
void log_window_clear(void);

#endif /* __LOGWINDOW_GTK4_H__ */
