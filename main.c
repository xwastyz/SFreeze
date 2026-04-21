#include <gtk/gtk.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

GtkWidget *status_label;
pid_t found_pids[128];
int found_count = 0;
bool frozen_mode = false;

void refresh_pids() {
    found_count = 0;
    DIR *dir = opendir("/proc");
    if (!dir) return;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL && found_count < 128) {
        if (entry->d_type == DT_DIR && atoi(entry->d_name) > 0) {
            char path[512], comm[256];
            snprintf(path, sizeof(path), "/proc/%s/comm", entry->d_name);
            FILE *f = fopen(path, "r");
            if (f) {
                if (fgets(comm, sizeof(comm), f)) {
                    comm[strcspn(comm, "\n")] = 0;
                    if (strcasecmp(comm, "sober") == 0 || strcasecmp(comm, "RobloxPlayerBet") == 0 || strcasecmp(comm, "sober-bin") == 0) {
                        found_pids[found_count++] = (pid_t)atoi(entry->d_name);
                    }
                }
                fclose(f);
            }
        }
    }
    closedir(dir);
}

static void update_ui_status_text() {
    char msg[128];
    if (frozen_mode) {
        gtk_label_set_text(GTK_LABEL(status_label), "In Frozen state DO NOT SPAM FREEZE");
    } else if (found_count > 0) {
        snprintf(msg, sizeof(msg), "Sober is active | Threads %d | Keybind: F1", found_count);
        gtk_label_set_text(GTK_LABEL(status_label), msg);
    } else {
        gtk_label_set_text(GTK_LABEL(status_label), "Sober Instance Not found, Is Sober running?");
    }
}

static gboolean resume_now(gpointer data) {
    for (int i = 0; i < found_count; i++) kill(found_pids[i], SIGCONT);
    frozen_mode = false;
    update_ui_status_text();
    return FALSE;
}

static void trigger_freeze() {
    refresh_pids();
    if (found_count > 0 && !frozen_mode) {
        frozen_mode = true;
        for (int i = 0; i < found_count; i++) kill(found_pids[i], SIGSTOP);
        update_ui_status_text();
        g_timeout_add(4000, resume_now, NULL);
    }
}

static gboolean ui_trigger_freeze(gpointer data) {
    trigger_freeze();
    return FALSE;
}

static void on_refresh_clicked(GtkWidget *widget, gpointer data) {
    refresh_pids();
    update_ui_status_text();
}

static void on_freeze_clicked(GtkWidget *widget, gpointer data) {
    trigger_freeze();
}

static void on_help_clicked(GtkWidget *widget, gpointer data) {
    GtkUriLauncher *launcher = gtk_uri_launcher_new("https://github.com/xwastyz/SFreeze/blob/main/README.md");
    gtk_uri_launcher_launch(launcher, GTK_WINDOW(gtk_widget_get_root(widget)), NULL, NULL, NULL);
    g_object_unref(launcher);
}

void* socket_listener(void* arg) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    servaddr.sin_port = htons(5555);

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) return NULL;

    char buffer[1024];
    while (1) {
        int n = recv(sockfd, buffer, sizeof(buffer), 0);
        if (n > 0) {
            g_idle_add(ui_trigger_freeze, NULL);
        }
    }
    return NULL;
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider,
                                      ".center-panel { background-color: rgba(20, 20, 20, 0.85); border: 1px solid rgba(255, 255, 255, 0.15); margin: 15px 15px 0px 15px; padding: 40px; }"
                                      "label { font-family: sans-serif; font-feature-settings: 'tnum'; }"
                                      "button { border-radius: 0px; padding: 6px 14px; }");

    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "SFreeze");
    gtk_window_set_default_size(GTK_WINDOW(window), 550, 350);

    GtkWidget *main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), main_vbox);

    GtkWidget *info_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_vexpand(info_area, TRUE);
    gtk_widget_add_css_class(info_area, "center-panel");
    gtk_box_append(GTK_BOX(main_vbox), info_area);

    GtkWidget *instr = gtk_label_new("Attaches automatically.\nIf not simply hit refresh in the buttom right!");
    gtk_label_set_justify(GTK_LABEL(instr), GTK_JUSTIFY_CENTER);
    gtk_widget_set_valign(instr, GTK_ALIGN_CENTER);
    gtk_widget_set_vexpand(instr, TRUE);
    gtk_box_append(GTK_BOX(info_area), instr);

    GtkWidget *bottom_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_margin_start(bottom_bar, 15);
    gtk_widget_set_margin_end(bottom_bar, 15);
    gtk_widget_set_margin_top(bottom_bar, 15);
    gtk_widget_set_margin_bottom(bottom_bar, 15);
    gtk_box_append(GTK_BOX(main_vbox), bottom_bar);

    status_label = gtk_label_new("Scanning...");
    gtk_widget_set_hexpand(status_label, TRUE);
    gtk_widget_set_halign(status_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(bottom_bar), status_label);

    GtkWidget *button_group = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_append(GTK_BOX(bottom_bar), button_group);

    GtkWidget *btn_help = gtk_button_new_with_label("Help");
    g_signal_connect(btn_help, "clicked", G_CALLBACK(on_help_clicked), NULL);
    gtk_box_append(GTK_BOX(button_group), btn_help);

    GtkWidget *btn_refresh = gtk_button_new_with_label("Refresh");
    g_signal_connect(btn_refresh, "clicked", G_CALLBACK(on_refresh_clicked), NULL);
    gtk_box_append(GTK_BOX(button_group), btn_refresh);

    GtkWidget *btn_freeze = gtk_button_new_with_label("Freeze");
    g_signal_connect(btn_freeze, "clicked", G_CALLBACK(on_freeze_clicked), NULL);
    gtk_box_append(GTK_BOX(button_group), btn_freeze);

    refresh_pids();
    update_ui_status_text();
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, socket_listener, NULL);
    GtkApplication *app = gtk_application_new("com.main.SFreeze", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    return g_application_run(G_APPLICATION(app), argc, argv);
}
