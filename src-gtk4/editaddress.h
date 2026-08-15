/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __EDITADDRESS_GTK4_H__
#define __EDITADDRESS_GTK4_H__

#include <gtk/gtk.h>

void edit_address_dialog_show(GtkWindow *parent,
			      const char *name,
			      const char *email,
			      const char *remarks);

#endif /* __EDITADDRESS_GTK4_H__ */
