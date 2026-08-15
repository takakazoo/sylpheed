/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __SEND_MESSAGE_GTK4_H__
#define __SEND_MESSAGE_GTK4_H__

#include <gtk/gtk.h>
#include "prefs_account.h"

int send_message(GtkWindow *parent,
		 PrefsAccount *account,
		 const char *to,
		 const char *subject,
		 const char *body,
		 GList *attachments);

#endif /* __SEND_MESSAGE_GTK4_H__ */
