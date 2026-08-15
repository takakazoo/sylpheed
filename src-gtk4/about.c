/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "about.h"
#include <glib/gi18n.h>

void about_dialog_show(GtkWindow *parent)
{
	const char *authors[] = {
		"Hiroyuki Yamamoto <hiro-y@kcn.ne.jp>",
		"Takakazu (Windows/GTK4 Maintainer)",
		"Sylpheed Contributors",
		NULL
	};

	gtk_show_about_dialog(parent,
		"program-name", "Sylpheed (GTK4 Preview)",
		"version", "4.0.0-alpha",
		"comments", _("A lightweight and fast e-mail client with GTK4"),
		"copyright", "Copyright (C) 1999-2026 Hiroyuki Yamamoto and Contributors",
		"license-type", GTK_LICENSE_GPL_2_0,
		"website", "https://sylpheed.sraoss.jp/",
		"website-label", "Sylpheed Official Website",
		"authors", authors,
		"logo-icon-name", "mail-client",
		NULL);
}
