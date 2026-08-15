bin_PROGRAMS = sylpheed-gtk4

sylpheed_gtk4_SOURCES = \
	main.c \
	mainwindow.c \
	mainwindow.h \
	folderview.c \
	folderview.h \
	summaryview.c \
	summaryview.h \
	messageview.c \
	messageview.h \
	compose.c \
	compose.h \
	setup.c \
	setup.h \
	account_dialog.c \
	account_dialog.h \
	addressbook.c \
	addressbook.h \
	prefs_dialog.c \
	prefs_dialog.h \
	filter_dialog.c \
	filter_dialog.h \
	search_dialog.c \
	search_dialog.h \
	logwindow.c \
	logwindow.h \
	menu.c \
	menu.h \
	about.c \
	about.h

sylpheed_gtk4_CPPFLAGS = \
	-I$(top_srcdir)/libsylph \
	-I$(top_builddir)/libsylph \
	$(GTK4_CFLAGS) \
	$(LIBSYLPH_CFLAGS) \
	-DSYSCONFDIR=\""$(sysconfdir)"\" \
	-DLOCALEDIR=\""$(localedir)"\" \
	-DMANUALDIR=\""$(manualdir)"\" \
	-DFAQDIR=\""$(faqdir)"\" \
	-DTARGET_ALIAS=\""$(target_triplet)"\"

sylpheed_gtk4_LDADD = \
	$(top_builddir)/libsylph/libsylph-0.la \
	$(GTK4_LIBS) \
	$(LIBSYLPH_LIBS) \
	$(INTLLIBS)

if NATIVE_WIN32
sylpheed_gtk4_LDFLAGS = -mwindows
endif
