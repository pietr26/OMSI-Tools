#ifndef OCSCRIPT_H
#define OCSCRIPT_H

#include "OCBase.h"

template<class T>
class OCVariableDeclaration
{
public:
    QString varname;
    T value;

    /*
        QVariant value:
        if (value.type() == QVariant::Int) // int
        else if (value.type() == QVariant::Double || value.type() == QVariant::Float) // float
    */
};

class OCScript : public OCFile // osc // TODO
{
public:
    class Action
    {
    public:

    };

    class Variable : public Action
    {
    public:

    };

    class Number : public Action
    {
    public:

    };

    class Container
    {
    public:
        QList<Action> actions;
    };

    // void test()
    // {
    //     Container tmp;
    //     Variable var;
    //     Number num;
    //     tmp.actions.append(var);
    // }

    QList<Container> script;
};

#endif // OCSCRIPT_H
