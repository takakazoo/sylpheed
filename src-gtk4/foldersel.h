/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __FOLDERSEL_GTK4_H__
#define __FOLDERSEL_GTK4_H__

#include <gtk/gtk.h>
#include "folder.h"

char *foldersel_folder_sel(GtkWindow *parent, const char *title, const char *default_folder);

#endif /* __FOLDERSEL_GTK4_H__ */
