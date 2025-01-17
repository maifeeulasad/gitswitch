#include <QApplication>
#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <vector>
#include <QString>
#include <QWidget>

class Contact {
public:
    Contact(const QString &name, const QString &email) : name(name), email(email) {}

    QString getName() const { return name; }
    QString getEmail() const { return email; }

private:
    QString name;
    QString email;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        auto *centralWidget = new QWidget(this);
        auto *mainLayout = new QVBoxLayout(centralWidget);

        contactListWidget = new QListWidget(this);
        mainLayout->addWidget(contactListWidget);

        auto *buttonLayout = new QHBoxLayout();
        auto *addContactButton = new QPushButton("Add Contact", this);
        buttonLayout->addWidget(addContactButton);
        mainLayout->addLayout(buttonLayout);

        setCentralWidget(centralWidget);

        // Initial data
        contacts.push_back(Contact("user1", "user1@gmail.com"));
        contacts.push_back(Contact("user1", "user@test.com"));
        contacts.push_back(Contact("user2", "hmmuser2hmm@gmail.com"));

        // Update the list view with the initial data
        updateContactList();

        // Connect signals
        connect(addContactButton, &QPushButton::clicked, this, &MainWindow::openAddContactDialog);
    }

private slots:
    void addContact(const QString &name, const QString &email) {
        contacts.emplace_back(name, email);
        updateContactList();
    }

    void openAddContactDialog() {
        QDialog dialog(this);
        dialog.setWindowTitle("Add Contact");
        auto *layout = new QVBoxLayout(&dialog);

        auto *nameLabel = new QLabel("Name:", &dialog);
        auto *nameEdit = new QLineEdit(&dialog);

        auto *emailLabel = new QLabel("Email:", &dialog);
        auto *emailEdit = new QLineEdit(&dialog);

        layout->addWidget(nameLabel);
        layout->addWidget(nameEdit);
        layout->addWidget(emailLabel);
        layout->addWidget(emailEdit);

        auto *buttonLayout = new QHBoxLayout();
        auto *cancelButton = new QPushButton("Cancel", &dialog);
        auto *addButton = new QPushButton("Add", &dialog);

        buttonLayout->addWidget(cancelButton);
        buttonLayout->addWidget(addButton);
        layout->addLayout(buttonLayout);

        connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
        connect(addButton, &QPushButton::clicked, [&]() {
            addContact(nameEdit->text(), emailEdit->text());
            dialog.accept();
        });

        dialog.exec();
    }

private:
    void updateContactList() {
        contactListWidget->clear();
        for (const auto &contact : contacts) {
            contactListWidget->addItem(contact.getName() + " - " + contact.getEmail());
        }
    }

    QListWidget *contactListWidget;
    std::vector<Contact> contacts;
};