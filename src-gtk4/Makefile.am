bin_PROGRAMS = sylpheed-gtk4

sylpheed_gtk4_SOURCES = \
	main.c \
	mainwindow.c \
	mainwindow.h

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
