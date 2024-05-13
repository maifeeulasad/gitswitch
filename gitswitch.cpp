#include <gtk/gtk.h>
#include <vector>
#include <string>
#include <rxcpp/rx.hpp>

class Contact
{
public:
    Contact(const std::string &name, const std::string &email) : name(name), email(email) {}

    std::string getName() const { return name; }
    std::string getEmail() const { return email; }

private:
    std::string name;
    std::string email;
};

rxcpp::subjects::subject<std::vector<Contact>> contactsSubject;
rxcpp::observable<std::vector<Contact>> observableVector = contactsSubject.get_observable();
std::vector<Contact> accumulatedContacts = {
    Contact("user1", "user1@gmail.com"),
    Contact("user1", "user@test.com"),
    Contact("user2", "hmmuser2hmm@gmail.com"),
};

static GtkWidget *window;
static GtkWidget *listbox;

rxcpp::subscriber<std::vector<Contact>> updateGridObserver = rxcpp::make_subscriber<std::vector<Contact>>(
    [](const std::vector<Contact> &items)
    {
        gtk_container_foreach(GTK_CONTAINER(listbox), (GtkCallback)gtk_widget_destroy, NULL);

        for (const auto &contact : accumulatedContacts)
        {
            GtkWidget *row = gtk_list_box_row_new();
            GtkWidget *label_name = gtk_label_new(contact.getName().c_str());
            GtkWidget *label_email = gtk_label_new(contact.getEmail().c_str());

            GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
            gtk_box_pack_start(GTK_BOX(hbox), label_name, TRUE, TRUE, 0);
            gtk_box_pack_start(GTK_BOX(hbox), label_email, TRUE, TRUE, 0);

            gtk_container_add(GTK_CONTAINER(row), hbox);
            gtk_list_box_insert(GTK_LIST_BOX(listbox), row, -1);
        }

        gtk_widget_show_all(window);
    });

static void add_contact(const std::string &name, const std::string &email)
{
    accumulatedContacts.push_back(Contact(name, email));
    contactsSubject.get_subscriber().on_next(accumulatedContacts);
}

static void open_dialog(GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog;
    GtkWidget *grid;
    GtkWidget *label_name;
    GtkWidget *label_email;
    GtkWidget *name_entry;
    GtkWidget *email_entry;
    GtkWidget *button_add_contact;

    dialog = gtk_dialog_new_with_buttons("Add Contact", GTK_WINDOW(data),
                                         GTK_DIALOG_MODAL,
                                         "Cancel", GTK_RESPONSE_CANCEL,
                                         "Add", GTK_RESPONSE_ACCEPT,
                                         NULL);

    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), grid);

    label_name = gtk_label_new("Name:");
    gtk_grid_attach(GTK_GRID(grid), label_name, 0, 0, 1, 1);

    name_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 0, 1, 1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(name_entry), "Enter name");

    label_email = gtk_label_new("Email:");
    gtk_grid_attach(GTK_GRID(grid), label_email, 0, 1, 1, 1);

    email_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), email_entry, 1, 1, 1, 1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(email_entry), "Enter email");

    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT)
    {
        const gchar *name = gtk_entry_get_text(GTK_ENTRY(name_entry));
        const gchar *email = gtk_entry_get_text(GTK_ENTRY(email_entry));
        add_contact(name, email);
    }

    gtk_widget_destroy(dialog);
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *grid;
    GtkWidget *button_add_contact;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Git Switch");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    listbox = gtk_list_box_new();
    gtk_grid_attach(GTK_GRID(grid), listbox, 0, 0, 1, 1);

    button_add_contact = gtk_button_new_with_label("Add Contact");
    gtk_grid_attach(GTK_GRID(grid), button_add_contact, 0, 1, 1, 1);
    g_signal_connect(button_add_contact, "clicked", G_CALLBACK(open_dialog), window);

    gtk_container_foreach(GTK_CONTAINER(listbox), (GtkCallback)gtk_widget_destroy, NULL);

    for (const auto &contact : accumulatedContacts)
    {
        GtkWidget *row = gtk_list_box_row_new();
        GtkWidget *label_name = gtk_label_new(contact.getName().c_str());
        GtkWidget *label_email = gtk_label_new(contact.getEmail().c_str());

        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        gtk_box_pack_start(GTK_BOX(hbox), label_name, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(hbox), label_email, TRUE, TRUE, 0);

        gtk_container_add(GTK_CONTAINER(row), hbox);
        gtk_list_box_insert(GTK_LIST_BOX(listbox), row, -1);
    }

    gtk_widget_show_all(window);

    observableVector.subscribe(
        [&accumulatedContacts](const std::vector<Contact> &items)
        {
            accumulatedContacts = items;
            updateGridObserver.on_next(accumulatedContacts);
        });
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
