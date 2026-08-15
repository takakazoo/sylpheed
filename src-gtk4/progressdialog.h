/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __PROGRESSDIALOG_GTK4_H__
#define __PROGRESSDIALOG_GTK4_H__

#include <gtk/gtk.h>

typedef struct _ProgressDialog ProgressDialog;

struct _ProgressDialog
{
	GtkWidget *window;
	GtkWidget *header_bar;
	GtkWidget *label_title;
	GtkWidget *label_status;
	GtkWidget *progress_bar;
	GtkWidget *btn_cancel;
	gboolean cancelled;
};

ProgressDialog *progress_dialog_create(GtkWindow *parent, const char *title);
void progress_dialog_set_value(ProgressDialog *dialog, double fraction);
void progress_dialog_set_status(ProgressDialog *dialog, const char *status);
void progress_dialog_destroy(ProgressDialog *dialog);

#endif /* __PROGRESSDIALOG_GTK4_H__ */
