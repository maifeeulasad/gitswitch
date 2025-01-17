#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QVector>
#include <QString>
#include <rxcpp/rx.hpp>

class Contact {
public:
    Contact(const QString &name, const QString &email) : name(name), email(email) {}

    QString getName() const { return name; }
    QString getEmail() const { return email; }

private:
    QString name;
    QString email;
};

rxcpp::subjects::subject<QVector<Contact>> contactsSubject;
rxcpp::observable<QVector<Contact>> observableVector = contactsSubject.get_observable();
QVector<Contact> accumulatedContacts = {
    Contact("user1", "user1@gmail.com"),
    Contact("user1", "user@test.com"),
    Contact("user2", "hmmuser2hmm@gmail.com"),
};

class GitSwitch : public QMainWindow {
    Q_OBJECT

public:
    GitSwitch(QWidget *parent = nullptr) : QMainWindow(parent) {
        auto *centralWidget = new QWidget(this);
        auto *layout = new QVBoxLayout(centralWidget);

        listWidget = new QListWidget(this);
        layout->addWidget(listWidget);

        auto *addContactButton = new QPushButton("Add Contact", this);
        layout->addWidget(addContactButton);

        setCentralWidget(centralWidget);
        setWindowTitle("Qt Contacts");

        connect(addContactButton, &QPushButton::clicked, this, &GitSwitch::openDialog);

        observableVector.subscribe(
            [this](const QVector<Contact> &contacts) {
                listWidget->clear();
                for (const auto &contact : contacts) {
                    listWidget->addItem(contact.getName() + " - " + contact.getEmail());
                }
            }
        );

        // Initialize the list with accumulatedContacts
        contactsSubject.get_subscriber().on_next(accumulatedContacts);
    }

private slots:
    void openDialog() {
        QDialog dialog(this);
        dialog.setWindowTitle("Add Contact");

        QFormLayout form(&dialog);
        QLineEdit *nameEdit = new QLineEdit(&dialog);
        QLineEdit *emailEdit = new QLineEdit(&dialog);

        form.addRow("Name:", nameEdit);
        form.addRow("Email:", emailEdit);

        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
        form.addRow(buttons);

        connect(buttons, &QDialogButtonBox::accepted, &dialog, [&]() {
            QString name = nameEdit->text();
            QString email = emailEdit->text();
            if (!name.isEmpty() && !email.isEmpty()) {
                addContact(name, email);
            }
            dialog.accept();
        });

        connect(buttons, &QDialogButtonBox::rejected, &dialog, [&]() {
            dialog.reject();
        });

        dialog.exec();
    }

    void addContact(const QString &name, const QString &email) {
        accumulatedContacts.append(Contact(name, email));
        contactsSubject.get_subscriber().on_next(accumulatedContacts);
    }

private:
    QListWidget *listWidget;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    GitSwitch mainWindow;
    mainWindow.resize(400, 300);
    mainWindow.show();

    return app.exec();
}
