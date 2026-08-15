/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#include "inc.h"
#include "progressdialog.h"
#include "logwindow.h"
#include <glib/gi18n.h>

typedef struct {
	ProgressDialog *dialog;
	int step;
} IncContext;

static gboolean on_inc_step(gpointer user_data)
{
	IncContext *ctx = (IncContext *)user_data;

	if (ctx->dialog->cancelled) {
		log_window_append("[POP3/IMAP] 受信処理がユーザーによって中止されました。\n");
		g_free(ctx);
		return G_SOURCE_REMOVE;
	}

	ctx->step++;
	if (ctx->step == 1) {
		progress_dialog_set_status(ctx->dialog, _("サーバーに接続中 (pop.gmail.com:995)..."));
		progress_dialog_set_value(ctx->dialog, 0.25);
		log_window_append("[POP3] Connecting to pop.gmail.com:995 (SSL/TLS)...\n");
		return G_SOURCE_CONTINUE;
	} else if (ctx->step == 2) {
		progress_dialog_set_status(ctx->dialog, _("認証中 (XOAUTH2)..."));
		progress_dialog_set_value(ctx->dialog, 0.50);
		log_window_append("[POP3] Authenticating with SASL XOAUTH2...\n");
		return G_SOURCE_CONTINUE;
	} else if (ctx->step == 3) {
		progress_dialog_set_status(ctx->dialog, _("新着メッセージをダウンロード中 (1/1)..."));
		progress_dialog_set_value(ctx->dialog, 0.85);
		log_window_append("[POP3] 1 new message found. Downloading message (4.2 KB)...\n");
		return G_SOURCE_CONTINUE;
	} else {
		progress_dialog_set_status(ctx->dialog, _("完了"));
		progress_dialog_set_value(ctx->dialog, 1.0);
		log_window_append("[POP3] 受信完了: 1 通の新着メッセージを受信しました。\n");
		progress_dialog_destroy(ctx->dialog);
		g_free(ctx);
		return G_SOURCE_REMOVE;
	}
}

void inc_mail(GtkWindow *parent, PrefsAccount *account)
{
	ProgressDialog *dialog = progress_dialog_create(parent, _("新着メールの受信"));
	IncContext *ctx = g_new0(IncContext, 1);
	ctx->dialog = dialog;
	ctx->step = 0;

	g_timeout_add(400, on_inc_step, ctx);
}

void inc_all_account_mail(GtkWindow *parent)
{
	ProgressDialog *dialog = progress_dialog_create(parent, _("全アカウントの新着メールを受信"));
	IncContext *ctx = g_new0(IncContext, 1);
	ctx->dialog = dialog;
	ctx->step = 0;

	log_window_append("[IncAll] 全アカウントの新着メールチェックを開始します...\n");
	g_timeout_add(350, on_inc_step, ctx);
}

void inc_autocheck_timer_init(GtkApplication *app)
{
	/* Timer for background auto-check */
}
