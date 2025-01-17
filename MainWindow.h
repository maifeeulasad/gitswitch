#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <vector>
#include "Contact.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void addContact(const QString &name, const QString &email);
    void openAddContactDialog();

private:
    void updateContactList();

    QListWidget *contactListWidget;
    std::vector<Contact> contacts;
};

#endif // MAINWINDOW_H
