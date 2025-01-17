#include "Contact.h"

Contact::Contact(const QString &name, const QString &email) : name(name), email(email) {}

QString Contact::getName() const {
    return name;
}

QString Contact::getEmail() const {
    return email;
}
