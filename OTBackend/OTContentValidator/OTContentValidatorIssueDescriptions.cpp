#include "OTContentValidator.h"

#include <QString>
#include <QDebug>

/* This function is used to generate descriptions for all possible issues that can occur during the validation process.
 * It returns a pair of strings. The first string is a short description of the issue, the second string is a more detailed
 * description of the issue.
 *
 * In addition to the enum values, the function also takes a QStringList of arguments that can be passed to the function.
 * These will be used by some issues to generate a more individual description.
 */

QPair<QString, QString> OTContentValidatorIssue::issueDescription(const int &e, const QStringList &args) {
    QString shortDesc, longDesc, bothDesc;
    switch(e) {
    case FileMissing:
        shortDesc = QObject::tr("File not found");
        longDesc  = QObject::tr("File %1 not found").arg(issueArgument(args, 0));
        break;
    case FileNotOpen:
        shortDesc = QObject::tr("Could not open file");
        longDesc  = QObject::tr("Could not open file %1: %2").arg(issueArgument(args, 0)).arg(issueArgument(args, 1, "unknown error"));
        break;
    case InvalidIntegerValue:
        shortDesc = QObject::tr("Invalid integer value");
        longDesc  = QObject::tr("Invalid integer value: %1").arg(issueArgument(args, 0));
        break;
    case InvalidFloatValue:
        shortDesc = QObject::tr("Invalid float value");
        longDesc  = QObject::tr("Invalid float value: %1").arg(issueArgument(args, 0));
        break;
    case MissingMeshFile:
        shortDesc = QObject::tr("Missing mesh file");
        longDesc  = QObject::tr("Missing mesh file: %1").arg(issueArgument(args, 0));
        break;
    case MissingScriptFile:
        shortDesc = QObject::tr("Missing script file");
        longDesc  = QObject::tr("Missing script file: %1").arg(issueArgument(args, 0));
        break;
    case MissingTextureFile:
        shortDesc = QObject::tr("Missing texture file");
        longDesc  = QObject::tr("Missing texture file: %1").arg(issueArgument(args, 0));
        break;
    case MeshConfigWithoutMesh:
        bothDesc =  QObject::tr("Mesh configuration without leading [mesh]");
        break;
    case MaterialWithoutMesh:
        bothDesc =  QObject::tr("Material definition without leading [mesh]");
        break;
    case MaterialPropertyWithoutMaterial:
        bothDesc =  QObject::tr("Material property definition without leading [matl]/[matl_change]");
        break;
    case ProfileTextureIndexOutOfRange:
        shortDesc = QObject::tr("Invalid texture index");
        longDesc  = QObject::tr("Texture index is out of range: Min: 0; Max: %1").arg(issueArgument(args, 0, "unkown"));
        break;
    case ProfilePointWithoutProfile:
        bothDesc =  QObject::tr("Profile point without leading [profile]");
        break;
    case TooFewProfilePoints:
        shortDesc = QObject::tr("Too few profile points");
        longDesc  = QObject::tr("This profile doesn't have enough profile points (has %1, needs at least 2)").arg(issueArgument(args, 0, "unkown"));
        break;
    case InvalidPathType:
        shortDesc = QObject::tr("Invalid path type");
        longDesc  = QObject::tr("\"%1\" is not a valid path type (valid: 0; 1; 2; 3)").arg(issueArgument(args, 0, "unkown"));
        break;
    case InvalidPathDirection:
        shortDesc = QObject::tr("Invalid path direction");
        longDesc  = QObject::tr("\"%1\" is not a valid path direction (valid: 0; 1; 2)").arg(issueArgument(args, 0, "unkown"));
        break;
    default:
        qDebug() << "Missing issue description for " << e;
        bothDesc = QObject::tr("Unknown issue");
    }

    if(bothDesc.isEmpty())
        return QPair<QString, QString>(shortDesc, longDesc);
    else
        return QPair<QString, QString>(bothDesc, bothDesc);
}

QString OTContentValidatorIssue::issueArgument(const QStringList &args,
                                               const int &index,
                                               const QString &defaultValue) {
    if(index < 0 || index >= args.count())
        return defaultValue;
    else
        return args[index];
}
