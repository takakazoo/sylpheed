/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "alertpanel.h"
#include <glib/gi18n.h>

void alertpanel_notice(GtkWindow *parent, const char *title, const char *msg)
{
	GtkAlertDialog *dialog = gtk_alert_dialog_new("%s", title ? title : _("通知"));
	gtk_alert_dialog_set_detail(dialog, msg ? msg : "");
	gtk_alert_dialog_show(dialog, parent);
	g_object_unref(dialog);
}

void alertpanel_warning(GtkWindow *parent, const char *title, const char *msg)
{
	GtkAlertDialog *dialog = gtk_alert_dialog_new("%s", title ? title : _("警告"));
	gtk_alert_dialog_set_detail(dialog, msg ? msg : "");
	gtk_alert_dialog_show(dialog, parent);
	g_object_unref(dialog);
}

void alertpanel_error(GtkWindow *parent, const char *title, const char *msg)
{
	GtkAlertDialog *dialog = gtk_alert_dialog_new("%s", title ? title : _("エラー"));
	gtk_alert_dialog_set_detail(dialog, msg ? msg : "");
	gtk_alert_dialog_show(dialog, parent);
	g_object_unref(dialog);
}

AlertValue alertpanel_full(GtkWindow *parent,
			  const char *title,
			  const char *msg,
			  const char *btn1,
			  const char *btn2,
			  const char *btn3)
{
	GtkAlertDialog *dialog = gtk_alert_dialog_new("%s", title ? title : _("確認"));
	const char *buttons[] = { btn1 ? btn1 : _("OK"), btn2, btn3, NULL };
	gtk_alert_dialog_set_detail(dialog, msg ? msg : "");
	gtk_alert_dialog_set_buttons(dialog, buttons);
	gtk_alert_dialog_show(dialog, parent);
	g_object_unref(dialog);
	return G_ALERTDEFAULT;
}
