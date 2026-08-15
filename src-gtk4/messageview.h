/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __MESSAGEVIEW_GTK4_H__
#define __MESSAGEVIEW_GTK4_H__

#include <gtk/gtk.h>

typedef struct _MessageView MessageView;

struct _MessageView
{
	GtkWidget *container;
	GtkWidget *text_view;
	GtkTextBuffer *buffer;
};

MessageView *message_view_create(void);
void message_view_set_text(MessageView *msgview, const char *text);
void message_view_clear(MessageView *msgview);

#endif /* __MESSAGEVIEW_GTK4_H__ */
