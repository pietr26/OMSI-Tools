#include "OCLanguage.h"

namespace OCLanguage {

QString Part::translation() const
{
    return _translation;
}

void Part::setTranslation(const QString &newTranslation)
{
    _translation = newTranslation;
}

QString Language::ident() const
{
    return _ident;
}

void Language::setIdent(const QString &newIdent)
{
    _ident = newIdent;
}

QList<Part *> Language::parts() const
{
    return _parts;
}

void Language::setParts(const QList<Part *> &newParts)
{
    _parts = newParts;
}

QString Part::ident() const
{
    return _ident;
}

void Part::setIdent(const QString &newIdent)
{
    _ident = newIdent;
}

}
