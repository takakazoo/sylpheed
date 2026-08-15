/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "printing.h"
#include <glib/gi18n.h>

typedef struct {
	char *header_text;
	char *body_text;
} PrintData;

static void on_draw_page(GtkPrintOperation *operation,
			 GtkPrintContext *context,
			 int page_nr,
			 gpointer user_data)
{
	PrintData *data = (PrintData *)user_data;
	cairo_t *cr = gtk_print_context_get_cairo_context(context);
	PangoLayout *layout;
	PangoFontDescription *desc;
	gchar *full_text;

	full_text = g_strdup_printf("%s\n\n------------------------------------------------------------\n\n%s",
				    data->header_text ? data->header_text : "",
				    data->body_text ? data->body_text : "");

	layout = gtk_print_context_create_pango_layout(context);
	desc = pango_font_description_from_string("Sans 10");
	pango_layout_set_font_description(layout, desc);
	pango_font_description_free(desc);

	pango_layout_set_text(layout, full_text, -1);
	g_free(full_text);

	cairo_move_to(cr, 20.0, 20.0);
	pango_cairo_show_layout(cr, layout);
	g_object_unref(layout);
}

void printing_print_message(GtkWindow *parent,
			    const char *from,
			    const char *to,
			    const char *subject,
			    const char *date,
			    const char *body)
{
	GtkPrintOperation *print_op;
	PrintData *data;

	data = g_new0(PrintData, 1);
	data->header_text = g_strdup_printf("From: %s\nTo: %s\nSubject: %s\nDate: %s",
					    from ? from : "", to ? to : "",
					    subject ? subject : "", date ? date : "");
	data->body_text = g_strdup(body ? body : "");

	print_op = gtk_print_operation_new();
	gtk_print_operation_set_n_pages(print_op, 1);

	g_signal_connect(print_op, "draw-page", G_CALLBACK(on_draw_page), data);

	gtk_print_operation_run(print_op, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG, parent, NULL);

	g_free(data->header_text);
	g_free(data->body_text);
	g_free(data);
	g_object_unref(print_op);
}
