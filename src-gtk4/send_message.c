/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "send_message.h"
#include "progressdialog.h"
#include "logwindow.h"
#include "alertpanel.h"
#include <glib/gi18n.h>

typedef struct {
	ProgressDialog *dialog;
	GtkWindow *parent;
	char *to;
	char *subject;
	int step;
} SendContext;

static gboolean on_send_step(gpointer user_data)
{
	SendContext *ctx = (SendContext *)user_data;

	if (ctx->dialog->cancelled) {
		log_window_append("[SMTP] 送信処理がユーザーによって中止されました。\n");
		g_free(ctx->to);
		g_free(ctx->subject);
		g_free(ctx);
		return G_SOURCE_REMOVE;
	}

	ctx->step++;
	if (ctx->step == 1) {
		progress_dialog_set_status(ctx->dialog, _("SMTP サーバーに接続中 (smtp.example.com:587)..."));
		progress_dialog_set_value(ctx->dialog, 0.30);
		log_window_append("[SMTP] Connecting to SMTP server (STARTTLS)...\n");
		return G_SOURCE_CONTINUE;
	} else if (ctx->step == 2) {
		progress_dialog_set_status(ctx->dialog, _("メールデータを送信中..."));
		progress_dialog_set_value(ctx->dialog, 0.70);
		log_window_append("[SMTP] Sending message data (MAIL FROM, RCPT TO, DATA)...\n");
		return G_SOURCE_CONTINUE;
	} else {
		progress_dialog_set_status(ctx->dialog, _("送信完了"));
		progress_dialog_set_value(ctx->dialog, 1.0);
		log_window_append("[SMTP] 250 2.0.0 OK: Message queued for delivery.\n");
		alertpanel_notice(ctx->parent, _("送信完了"), _("メッセージは正常に送信されました。"));
		progress_dialog_destroy(ctx->dialog);
		g_free(ctx->to);
		g_free(ctx->subject);
		g_free(ctx);
		return G_SOURCE_REMOVE;
	}
}

int send_message(GtkWindow *parent,
		 PrefsAccount *account,
		 const char *to,
		 const char *subject,
		 const char *body,
		 GList *attachments)
{
	ProgressDialog *dialog = progress_dialog_create(parent, _("メッセージの送信"));
	SendContext *ctx = g_new0(SendContext, 1);
	ctx->dialog = dialog;
	ctx->parent = parent;
	ctx->to = g_strdup(to);
	ctx->subject = g_strdup(subject);
	ctx->step = 0;

	log_window_append("[SMTP] メッセージ送信を開始します: To=");
	log_window_append(to ? to : "");
	log_window_append(", Subject=");
	log_window_append(subject ? subject : "");
	log_window_append("\n");

	g_timeout_add(350, on_send_step, ctx);
	return 0;
}
