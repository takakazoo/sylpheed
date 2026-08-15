/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __ALERTPANEL_GTK4_H__
#define __ALERTPANEL_GTK4_H__

#include <gtk/gtk.h>

typedef enum
{
	G_ALERTDEFAULT = 0,
	G_ALERTALTERNATE,
	G_ALERTOTHER,
	G_ALERTCANCEL,
	G_ALERTWAIT
} AlertValue;

void alertpanel_notice(GtkWindow *parent, const char *title, const char *msg);
void alertpanel_warning(GtkWindow *parent, const char *title, const char *msg);
void alertpanel_error(GtkWindow *parent, const char *title, const char *msg);
AlertValue alertpanel_full(GtkWindow *parent,
			  const char *title,
			  const char *msg,
			  const char *btn1,
			  const char *btn2,
			  const char *btn3);

#endif /* __ALERTPANEL_GTK4_H__ */
