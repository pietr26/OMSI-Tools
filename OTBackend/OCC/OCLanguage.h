#ifndef OCLANGUAGE_H
#define OCLANGUAGE_H

#include "OCBase.h"

namespace OCLanguage
{

class Part
{
public:
    QString ident() const;
    void setIdent(const QString &newIdent);
    QString translation() const;
    void setTranslation(const QString &newTranslation);

private:
    QString _ident;
    QString _translation;
};

class Language : public OCBase::File // olf
{
public:
    QString ident() const;
    void setIdent(const QString &newIdent);
    QList<Part *> parts() const;
    void setParts(const QList<Part *> &newParts);

private:
    QString _ident;
    QList<Part*> _parts;
};

}

#endif // OCLANGUAGE_H
