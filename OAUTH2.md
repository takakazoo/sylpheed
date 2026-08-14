# Sylpheed OAuth 2.0 認証設定ガイド

Sylpheed 3.8 以降では、OAuth 2.0 認証（XOAUTH2）を利用して **Gmail** や **Microsoft 365 / Outlook.com** と安全に送受信を行うことができます。

---

## 1. 概要と事前準備

OAuth 2.0 認証を利用するには、Google または Microsoft の開発者ポータルで「クライアント ID」と「クライアント シークレット」を取得し、設定ファイル `oauth2.ini` に記述して配置する必要があります。

### `oauth2.ini` の配置場所
Sylpheed は以下の順序で `oauth2.ini` を検索して読み込みます：

- **Windows**:
  - `%APPDATA%\Sylpheed\oauth2.ini`（推奨：ユーザー個別設定）
  - または Sylpheed インストールフォルダ内の `oauth2.ini`
- **Linux / BSD / macOS**:
  - `~/.sylpheed-2.0/oauth2.ini`

---

## 2. Google (Gmail) の設定手順

### 2.1. クライアント ID / シークレットの取得

1. [Google Cloud Console](https://console.cloud.google.com/) にアクセスし、Google アカウントでログインします。
2. 新しいプロジェクトを作成します（例: `Sylpheed-Mail`）。
3. **「APIとサービス」 > 「OAuth 同意画面」** を開きます：
   - User Type: 「外部」を選択（組織利用の場合は「内部」）
   - アプリ名、ユーザーサポートメール、デベロッパー連絡先情報を入力して保存
   - スコープ追加で `https://mail.google.com/` を追加
   - テストユーザーに自身の Gmail アドレスを追加
4. **「APIとサービス」 > 「認証情報」** を開きます：
   - 「認証情報を作成」 > 「OAuth クライアント ID」をクリック
   - アプリケーションの種類: **「デスクトップ アプリ」** を選択
   - 名前を入力して「作成」をクリック
5. 表示された **クライアント ID** と **クライアント シークレット** をコピーします。

---

## 3. Microsoft (Microsoft 365 / Outlook.com) の設定手順

### 3.1. クライアント ID / シークレットの取得

1. [Microsoft Entra 管理センター (Azure Portal)](https://portal.azure.com/#blade/Microsoft_AAD_RegisteredApps/ApplicationsListBlade) にアクセスします。
2. **「新規登録」** をクリックします：
   - 名前: `Sylpheed`
   - サポートされているアカウントの種類:
     - 一般の Outlook.com / Hotmail を含む場合: **「任意の組織ディレクトリ内のアカウントと、個人の Microsoft アカウント」** を選択
     - 組織内のみの場合: 「この組織ディレクトリのみに含まれるアカウント」を選択
   - リダイレクト URI: プラットフォームに **「パブリック クライアント/ネイティブ (モバイルとデスクトップ)」** を選択し、`http://localhost:8089/` を入力
3. 登録後、概要画面の **アプリケーション (クライアント) ID** をコピーします。
4. **「証明書とシークレット」** を開き、「新しいクライアント シークレット」を作成してコピーします。
5. **「API のアクセス許可」** を開き、Microsoft Graph または Office 365 Exchange Online の以下のアクセス許可（委任されたアクセス許可）を追加します：
   - `offline_access`
   - `https://outlook.office.com/IMAP.AccessAsUser.All`
   - `https://outlook.office.com/POP.AccessAsUser.All`
   - `https://outlook.office.com/SMTP.Send`

---

## 4. `oauth2.ini` の作成と記述例

取得したクライアント ID とシークレットを反映した `oauth2.ini` ファイルを作成します。

```ini
# Gmail 用設定
[*@gmail.com]
auth_uri=https://accounts.google.com/o/oauth2/auth
token_uri=https://oauth2.googleapis.com/token
redirect_uri=http://localhost:8089/
client_id=<取得した Google クライアント ID>
client_secret=<取得した Google クライアント シークレット>
scope=https://mail.google.com/
local_port=8089

# Microsoft 365 (組織アカウント) 用設定
[*@*.onmicrosoft.com]
auth_uri=https://login.microsoftonline.com/common/oauth2/v2.0/authorize
token_uri=https://login.microsoftonline.com/common/oauth2/v2.0/token
redirect_uri=http://localhost:8089/
client_id=<取得した Microsoft クライアント ID>
client_secret=<取得した Microsoft クライアント シークレット>
scope=offline_access https://outlook.office.com/POP.AccessAsUser.All https://outlook.office.com/IMAP.AccessAsUser.All https://outlook.office.com/SMTP.Send
local_port=8089

# Outlook.com / Hotmail 用設定
[*@outlook.com]
auth_uri=https://login.microsoftonline.com/common/oauth2/v2.0/authorize
token_uri=https://login.microsoftonline.com/common/oauth2/v2.0/token
redirect_uri=http://localhost:8089/
client_id=<取得した Microsoft クライアント ID>
client_secret=<取得した Microsoft クライアント シークレット>
scope=offline_access https://outlook.office.com/POP.AccessAsUser.All https://outlook.office.com/IMAP.AccessAsUser.All https://outlook.office.com/SMTP.Send
local_port=8089

[*@hotmail.com]
auth_uri=https://login.microsoftonline.com/common/oauth2/v2.0/authorize
token_uri=https://login.microsoftonline.com/common/oauth2/v2.0/token
redirect_uri=http://localhost:8089/
client_id=<取得した Microsoft クライアント ID>
client_secret=<取得した Microsoft クライアント シークレット>
scope=offline_access https://outlook.office.com/POP.AccessAsUser.All https://outlook.office.com/IMAP.AccessAsUser.All https://outlook.office.com/SMTP.Send
local_port=8089
```

---

## 5. 初回認証の流れ

1. `oauth2.ini` を配置後、Sylpheed のセットアップウィザードまたは「アカウントの設定」で認証方式に **「OAuth2」** を選択します。
2. 初回の送受信時にデフォルトブラウザが自動起動し、Google または Microsoft のログイン・同意画面が表示されます。
3. ブラウザでアクセスを「許可」すると、ローカルサーバ（ポート `8089`）経由で認証トークンが Sylpheed（`syl-auth-helper`）に保存されます。
4. 以降はリフレッシュトークンにより自動的に認証が更新されます。
