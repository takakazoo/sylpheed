/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __COMPOSE_GTK4_H__
#define __COMPOSE_GTK4_H__

#include <gtk/gtk.h>
#include "prefs_account.h"

typedef struct _ComposeWindow ComposeWindow;

struct _ComposeWindow
{
	GtkWidget *window;
	GtkWidget *header_bar;

	/* Header fields */
	GtkWidget *entry_from;
	GtkWidget *entry_to;
	GtkWidget *entry_cc;
	GtkWidget *entry_bcc;
	GtkWidget *entry_subject;

	/* Attachment area */
	GtkWidget *attach_box;
	GtkWidget *attach_list;
	GtkStringList *attach_model;

	/* Body Editor */
	GtkWidget *text_view;
	GtkTextBuffer *text_buffer;

	PrefsAccount *account;
};

ComposeWindow *compose_window_new(GtkWindow *parent, const char *to, const char *subject, const char *body);
void compose_window_show(ComposeWindow *compose);

#endif /* __COMPOSE_GTK4_H__ */
