/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __ACCOUNT_DIALOG_GTK4_H__
#define __ACCOUNT_DIALOG_GTK4_H__

#include <gtk/gtk.h>

void account_manager_dialog_show(GtkWindow *parent);
void account_edit_dialog_show(GtkWindow *parent, const char *account_name);

#endif /* __ACCOUNT_DIALOG_GTK4_H__ */
