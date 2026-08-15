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
	GtkWidget *header_box;
	GtkWidget *header_from;
	GtkWidget *header_to;
	GtkWidget *header_subject;
	GtkWidget *header_date;

	/* Attachment Bar */
	GtkWidget *attach_box;
	GtkWidget *attach_chips_box;

	/* Body */
	GtkWidget *text_view;
	GtkTextBuffer *buffer;
	GtkTextTag *tag_quote;
	GtkTextTag *tag_header;
};

MessageView *message_view_create(void);
void message_view_set_message(MessageView *msgview,
			      const char *from,
			      const char *to,
			      const char *subject,
			      const char *date,
			      const char *body,
			      const char *attachment_filename,
			      const char *attachment_size);
void message_view_clear(MessageView *msgview);

#endif /* __MESSAGEVIEW_GTK4_H__ */
