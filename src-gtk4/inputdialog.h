/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __INPUTDIALOG_GTK4_H__
#define __INPUTDIALOG_GTK4_H__

#include <gtk/gtk.h>

char *input_dialog(GtkWindow *parent,
		   const char *title,
		   const char *message,
		   const char *default_string);

char *input_dialog_with_invisible(GtkWindow *parent,
				  const char *title,
				  const char *message,
				  const char *default_string);

#endif /* __INPUTDIALOG_GTK4_H__ */
