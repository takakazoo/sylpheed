/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __FILTER_DIALOG_GTK4_H__
#define __FILTER_DIALOG_GTK4_H__

#include <gtk/gtk.h>

void filter_dialog_show(GtkWindow *parent);
void filter_edit_dialog_show(GtkWindow *parent, const char *rule_name);

#endif /* __FILTER_DIALOG_GTK4_H__ */
