# Sylpheed ビルドガイド (Building Sylpheed)

Sylpheed をソースコードからビルド・実行、およびインストーラを作成する手順です。

---

## 1. Windows (MSYS2 / MinGW-w64) 環境でのビルド

### 1.1. 前提環境の準備

1. [MSYS2 公式サイト](https://www.msys2.org/) からインストーラをダウンロードし、インストールします（既定値: `C:\msys64`）。
2. **MSYS2 MINGW64** ターミナルを起動し、必要なツールと依存ライブラリをインストールします：

```bash
pacman -Syu --noconfirm
pacman -S --noconfirm \
    base-devel \
    mingw-w64-x86_64-toolchain \
    mingw-w64-x86_64-gtk2 \
    mingw-w64-x86_64-glib2 \
    mingw-w64-x86_64-openssl \
    mingw-w64-x86_64-gpgme \
    mingw-w64-x86_64-oniguruma \
    mingw-w64-x86_64-curl \
    mingw-w64-x86_64-nsis \
    mingw-w64-x86_64-nsis-nsisunz \
    p7zip
```

---

### 1.2. ビルド設定とコンパイル

MSYS2 MINGW64 ターミナルでリポジトリディレクトリに移動し、以下を実行します：

```bash
# configure スクリプトの生成と設定
./autogen.sh

# コンパイル
make CFLAGS='-O2 -g -Wno-incompatible-pointer-types -Wno-implicit-function-declaration' -j$(nproc)
```

---

### 1.3. 開発版の起動

MSYS2 のランタイム DLL へのパスを通して起動するためのスクリプトが用意されています：

- **バッチファイルで起動**:
  - `run_sylpheed.bat` をダブルクリック（または PowerShell / コマンドプロンプトから `.\run_sylpheed.bat`）
- **コンソール画面を出さずに起動**:
  - `run_sylpheed.vbs` をダブルクリック

---

### 1.4. Windows インストーラ (`.exe`) の作成

依存 DLL の収集、マニュアル、テーマ、多言語メッセージファイルの配置、および NSIS インストーラ（`Sylpheed-3.8.0beta1_setup.exe`）の生成を一括で行います：

- **Windows エクスプローラーから**:
  - `make_installer.bat` をダブルクリック
- **MSYS2 ターミナルから**:
  ```bash
  ./make_installer.sh
  ```

生成されたインストーラは `nsis/Sylpheed-3.8.0beta1_setup.exe` に出力されます。

---

## 2. Linux / BSD / Unix 環境でのビルド

### 2.1. 必要なパッケージ (Debian / Ubuntu の例)

```bash
sudo apt update
sudo apt install -y \
    build-essential \
    automake \
    autoconf \
    libtool \
    pkg-config \
    gettext \
    libgtk2.0-dev \
    libglib2.0-dev \
    libssl-dev \
    libgpgme-dev \
    libonig-dev \
    libcurl4-openssl-dev
```

### 2.2. ビルドとインストール

```bash
# configure スクリプトの生成
./autogen.sh

# ビルド
make -j$(nproc)

# インストール
sudo make install
```

---

## 3. 主な configure オプション

| オプション | 説明 |
| :--- | :--- |
| `--enable-ssl` | SSL/TLS (OpenSSL) サポートを有効化 (既定: 有効) |
| `--enable-gpgme` | GnuPG (GPGME) による暗号化/署名サポートを有効化 (既定: 有効) |
| `--enable-oniguruma` | 鬼雲 (Oniguruma) 正規表現ライブラリを有効化 (既定: 有効) |
| `--enable-threads` | マルチスレッド処理を有効化 (既定: 有効) |
| `--prefix=DIR` | インストール先ディレクトリの指定 (既定: `/usr/local` または `/mingw64`) |
