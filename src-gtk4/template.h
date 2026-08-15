/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __TEMPLATE_GTK4_H__
#define __TEMPLATE_GTK4_H__

#include <gtk/gtk.h>

void template_dialog_show(GtkWindow *parent);
void template_edit_dialog_show(GtkWindow *parent, const char *name);

#endif /* __TEMPLATE_GTK4_H__ */
