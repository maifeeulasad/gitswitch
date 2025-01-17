#ifndef CONTACT_H
#define CONTACT_H

#include <QString>

class Contact {
public:
    Contact(const QString &name, const QString &email);
    QString getName() const;
    QString getEmail() const;

private:
    QString name;
    QString email;
};

#endif // CONTACT_H
