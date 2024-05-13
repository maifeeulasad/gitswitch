#include <gtk/gtk.h>
#include <vector>
#include <string>

class Contact {
public:
    Contact(const std::string& name, const std::string& email) : name(name), email(email) {}

    std::string getName() const { return name; }
    std::string getEmail() const { return email; }

private:
    std::string name;
    std::string email;
};

std::vector<Contact> contacts = {
    Contact("user1", "user1@gmail.com"),
    Contact("user1", "user@test.com"),
    Contact("user2", "hmmuser2hmm@gmail.com"),
};

static void activate(GtkApplication* app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *listbox;
    GtkWidget *row;
    GtkWidget *label_name;
    GtkWidget *label_email;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Git Switch");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    listbox = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(window), listbox);

    for (const auto& contact : contacts) {
        row = gtk_list_box_row_new();
        label_name = gtk_label_new(contact.getName().c_str());
        label_email = gtk_label_new(contact.getEmail().c_str());
        
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        gtk_box_pack_start(GTK_BOX(hbox), label_name, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(hbox), label_email, TRUE, TRUE, 0);
        
        gtk_container_add(GTK_CONTAINER(row), hbox);
        gtk_list_box_insert(GTK_LIST_BOX(listbox), row, -1);
    }

    gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

