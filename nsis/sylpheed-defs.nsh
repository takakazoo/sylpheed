Unicode true

!define PRODUCT_NAME "Sylpheed"
!define PRODUCT_VERSION "3.8.0beta1"

!ifdef SYLPHEED_PRO
!define PRODUCT_PUBLISHER "SRA OSS, Inc. Japan"
!define PRODUCT_WEB_SITE "http://www.sraoss.co.jp/sylpheed-pro/"
!else
!define PRODUCT_PUBLISHER "Hiroyuki Yamamoto"
!define PRODUCT_WEB_SITE "http://sylpheed.sraoss.jp/"
!endif

!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\sylpheed.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define MEMENTO_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MEMENTO_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"

!ifdef SYLPHEED_PRO
!ifndef INST_NAME
!define INST_NAME "Sylpheed Pro"
!endif
!ifndef INST_VERSION
!define INST_VERSION "2.6"
!endif
!ifndef INST_FILENAME
!define INST_FILENAME "Sylpheed_Pro"
!endif
!else
!ifndef INST_NAME
!define INST_NAME ${PRODUCT_NAME}
!endif
!ifndef INST_VERSION
!define INST_VERSION ${PRODUCT_VERSION}
!endif
!ifndef INST_FILENAME
!define INST_FILENAME ${PRODUCT_NAME}
!endif
!endif

!define ORIG_WEB_SITE "http://sylpheed.sraoss.jp/"
