/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __PRINTING_GTK4_H__
#define __PRINTING_GTK4_H__

#include <gtk/gtk.h>

void printing_print_message(GtkWindow *parent,
			    const char *from,
			    const char *to,
			    const char *subject,
			    const char *date,
			    const char *body);

#endif /* __PRINTING_GTK4_H__ */
