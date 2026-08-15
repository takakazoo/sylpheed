/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "sslmanager.h"
#include <glib/gi18n.h>

gboolean ssl_manager_verify_cert_dialog(GtkWindow *parent,
					const char *host,
					int port,
					const char *fingerprint_sha256,
					const char *subject_dn,
					const char *issuer_dn)
{
	GtkWidget *win;
	GtkWidget *header_bar;
	GtkWidget *btn_accept, *btn_reject;
	GtkWidget *vbox, *grid;
	GtkWidget *label;
	gchar *msg;

	win = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(win), _("SSL/TLS 証明書の確認 - Sylpheed GTK4"));
	gtk_window_set_default_size(GTK_WINDOW(win), 580, 360);
	if (parent) {
		gtk_window_set_transient_for(GTK_WINDOW(win), parent);
		gtk_window_set_modal(GTK_WINDOW(win), TRUE);
	}

	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(win), header_bar);

	btn_reject = gtk_button_new_with_label(_("拒否"));
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_reject);

	btn_accept = gtk_button_new_with_label(_("一時的に受け入れる"));
	gtk_widget_add_css_class(btn_accept, "suggested-action");
	gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), btn_accept);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
	gtk_widget_set_margin_start(vbox, 16);
	gtk_widget_set_margin_end(vbox, 16);
	gtk_widget_set_margin_top(vbox, 16);
	gtk_widget_set_margin_bottom(vbox, 16);
	gtk_window_set_child(GTK_WINDOW(win), vbox);

	msg = g_strdup_printf(_("ホスト %s (ポート %d) の SSL 証明書を検証できませんでした。\nこの証明書を受け入れて通信を継続しますか？"),
			      host ? host : "unknown", port);
	label = gtk_label_new(msg);
	g_free(msg);
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);
	gtk_box_append(GTK_BOX(vbox), label);

	grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 6);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 12);
	gtk_box_append(GTK_BOX(vbox), grid);

	label = gtk_label_new(_("発行先 (Subject):"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_widget_add_css_class(label, "dim-label");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
	label = gtk_label_new(subject_dn ? subject_dn : "CN=mail.example.com");
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);
	gtk_grid_attach(GTK_GRID(grid), label, 1, 0, 1, 1);

	label = gtk_label_new(_("発行者 (Issuer):"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_widget_add_css_class(label, "dim-label");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
	label = gtk_label_new(issuer_dn ? issuer_dn : "CN=Let's Encrypt Authority X3");
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);
	gtk_grid_attach(GTK_GRID(grid), label, 1, 1, 1, 1);

	label = gtk_label_new(_("SHA-256 指紋:"));
	gtk_label_set_xalign(GTK_LABEL(label), 1.0);
	gtk_widget_add_css_class(label, "dim-label");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);
	label = gtk_label_new(fingerprint_sha256 ? fingerprint_sha256 : "3F:A1:9B:7C:E5:42:10:...");
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);
	gtk_grid_attach(GTK_GRID(grid), label, 1, 2, 1, 1);

	gtk_window_present(GTK_WINDOW(win));

	return TRUE;
}
