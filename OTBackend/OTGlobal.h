#ifndef OTGLOBAL_H
#define OTGLOBAL_H

/*!
    Collective include.

    This header used to hold thirteen unrelated classes on 1075 lines, and practically
    every file in the project includes it. The classes now live in one header each,
    next to this file; the Qt includes below stay so that no include anywhere else has
    to change.

    New code should include the single header it actually needs instead of this one.
*/

#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QTime>
#include <QDate>
#include <QApplication>
#include <QProcess>
#include <QFileDialog>
#include <QtNetwork>
#include <QDesktopServices>
#include <QScreen>
#include <QClipboard>
#include <QStyle>
#include <QStyleFactory>
#include <QStyleHints>

#include "DiscordGameSDK.h"
#include "OTPath.h"
#include "OTPlatform.h"

#include "OTFileMethods.h"
#include "OTBuildOptions.h"
#include "OTInformation.h"
#include "OTLinks.h"
#include "OTNetworkConnection.h"
#include "OTMiscellaneous.h"
#include "OTUpdater.h"
#include "OTFileOperations.h"
#include "OTSettings.h"
#include "OTMessage.h"
#include "OTDiskUsage.h"
#include "OTStrings.h"
#include "OTInAppMessage.h"

#endif // OTGLOBAL_H
