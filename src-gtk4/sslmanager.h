/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __SSLMANAGER_GTK4_H__
#define __SSLMANAGER_GTK4_H__

#include <gtk/gtk.h>

gboolean ssl_manager_verify_cert_dialog(GtkWindow *parent,
					const char *host,
					int port,
					const char *fingerprint_sha256,
					const char *subject_dn,
					const char *issuer_dn);

#endif /* __SSLMANAGER_GTK4_H__ */
