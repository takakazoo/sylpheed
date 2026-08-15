/*
 * Sylpheed-GTK4 -- a GTK4 based, lightweight and fast e-mail client
 */

#ifndef __MENU_GTK4_H__
#define __MENU_GTK4_H__

#include <gtk/gtk.h>

void menu_init_actions(GtkApplication *app, gpointer mainwin);
GMenuModel *menu_create_main_menu(void);
GMenuModel *menu_create_app_menu(void);

#endif /* __MENU_GTK4_H__ */
