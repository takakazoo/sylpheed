/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __INC_GTK4_H__
#define __INC_GTK4_H__

#include <gtk/gtk.h>
#include "prefs_account.h"

void inc_mail(GtkWindow *parent, PrefsAccount *account);
void inc_all_account_mail(GtkWindow *parent);
void inc_autocheck_timer_init(GtkApplication *app);

#endif /* __INC_GTK4_H__ */
