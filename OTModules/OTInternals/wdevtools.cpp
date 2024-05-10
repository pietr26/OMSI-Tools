#include "wdevtools.h"
#include "ui_wdevtools.h"

wDevTools::wDevTools(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wDevTools)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    resize(misc.sizeWindow(0.5, 0.75));
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - DevTools");

    setOutputVisible(true);

    qInfo().noquote() << objectName() + " started - Note: Internal module, may contains bugs.";
}

wDevTools::~wDevTools()
{
    delete ui;
}

/// Get list of Items
void wDevTools::on_btnGetListOfFiles_clicked()
{
    qDebug() << "Start to get list of files...";
    QStringList files = QFileDialog::getOpenFileNames(this, "Select files to analyse", set.read("main", "mainDir").toString(), omsiFilesFilter);
    files.sort();
    QString resultStr;

    unsigned int i = 0;
    ui->pgbProgress->setMaximum(files.size());

    foreach (QString current, files)
    {
        ui->pgbProgress->setValue(i);
        i++;
        qApp->processEvents();

        current.remove(0, cutCount);
        resultStr += current + "\n";
    }
    ui->pgbProgress->setValue(files.size());

    if (pushToOutput(resultStr))
        qInfo() << "Successfully got filelist!";
    else
    {
        qCritical() << "Could not push filelist to GUI / to file!";
        qDebug().noquote() << "resultStr:" << resultStr;
    }
}

/// Get keywords of files
void wDevTools::on_btnGetKeywords_clicked()
{
    if (ui->pteGetLinesThatContains->toPlainText().isEmpty())
    {
        QMessageBox::information(this, "Run to Hintertupfingen", "The line filter field ist empty! Janine will kill you!\n\n[Stop.]");
        return;
    }

    QStringList files = QFileDialog::getOpenFileNames(this, "Select files to analyse", set.read("main", "mainDir").toString(), omsiFilesFilter);

    qInfo().noquote() << "Start to get keywords from " + QString::number(files.size()) + " files...";

    unsigned int errorCount = 0;
    QStringList result;

    unsigned int i = 0;
    ui->pgbProgress->setMaximum(files.size());

    QStringList containArgs;
    QTextStream in(new QString(ui->pteGetLinesThatContains->toPlainText()));
    while (!in.atEnd()) containArgs.append(in.readLine());

    foreach (QString current, files)
    {
        ui->pgbProgress->setValue(i);
        i++;
        qApp->processEvents();


        QFile currentFile(current);
        if (!currentFile.open(QFile::ReadOnly | QFile::Text))
            errorCount++;
        else
        {
            QTextStream in(&currentFile);
            QString line;
            while (!in.atEnd())
            {
                line = in.readLine();

                bool contains = (line.contains(containArgs.at(0))) ? true : false;
                for (int i = 1; i < containArgs.size(); i++)
                    contains = contains && ((line.contains(containArgs.at(i))) ? true : false);

                if (contains)
                    result << line + "\n";
            }
        }
    }
    ui->pgbProgress->setValue(files.size());

    result.removeDuplicates();
    result.sort();

    if (errorCount == 0)
    {
        qInfo() << "All files read successfully.";
        result << "<All files could be read>";
    }
    else
    {
        qWarning().noquote() << "<ERROR: " + QString::number(errorCount) + " file(s) could not be read!";
        result << "<ERROR: " + QString::number(errorCount) + " file(s) could not be read!";
    }


    QString resultStr;
    foreach (QString current, result)
        resultStr += current;

    if (pushToOutput(resultStr))
        qInfo() << "Successfully got keywords";
    else
    {
        qCritical() << "Could not push keyword list to GUI / to file!";
        qDebug().noquote() << "resultStr:" << resultStr;
    }
}

/// Select the file output path
void wDevTools::on_tbnOutputPathSelection_clicked()
{
    ui->ledOutputPath->setText(QFileDialog::getExistingDirectory(this, "Select path..."));
}

/// Pushes text to view
bool wDevTools::pushToOutput(QString result)
{
    try
    {
        if (ui->rbnOutputToFile->isChecked())
        {
            QFile file(ui->ledOutputPath->text() + "/Output.txt");
            if (!file.open(QFile::WriteOnly | QFile::Text))
            {
                QMessageBox::critical(this, "Error while opening file", "There was an error while opening the file. The progress will be stopped.");
                return false;
            }
            else
            {
                QTextStream out (&file);
                out << result;
            }
            file.close();
            ui->pteOutput->setPlainText("<Output to file " + QFileInfo(file).absoluteFilePath() + ">");
        }
        else
            ui->pteOutput->setPlainText(result);

        return true;
    }
    catch (...)
    {
        return false;
    }

}

/// Controls visible of elements
void wDevTools::setOutputVisible(bool mode)
{
    if (mode)
    {
        ui->pteOutput->setVisible(true);
        ui->ledOutputPath->setVisible(false);
        ui->tbnOutputPathSelection->setVisible(false);
    }
    else
    {
        ui->pteOutput->setVisible(false);
        ui->ledOutputPath->setVisible(true);
        ui->tbnOutputPathSelection->setVisible(true);
    }
}

/// Counts lines
void wDevTools::on_btnLineCounter_clicked()
{
    countLinesFiles.append(QFileDialog::getOpenFileNames(this, "Select files to analyse", set.read("main", "mainDir").toString(), omsiFilesFilter));
    ui->btnLineCounterStart->setText(QString("Start (%1)").arg(countLinesFiles.size()));
}

void wDevTools::on_btnLineCounterStart_clicked()
{
    qInfo().noquote() << "Start to get line count from " + QString::number(countLinesFiles.size()) + " file(s)...";
    unsigned int errorCount = 0;
    unsigned int lines = 0;
    QString result;

    unsigned int i = 0;
    ui->pgbProgress->setMaximum(countLinesFiles.size());

    foreach(QString current, countLinesFiles)
    {
        ui->pgbProgress->setValue(i);
        i++;
        qApp->processEvents();

        QFile file(current);
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            qWarning().noquote() << "File " + QFileInfo(file).absoluteFilePath() + "could not be read!";
            errorCount++;
        }
        else
        {
            QTextStream in (&file);
            while (!in.atEnd())
            {
                in.readLine();
                lines++;
            }
        }
    }
    ui->pgbProgress->setValue(countLinesFiles.size());

    result = QString::number(lines) + "\n";
    if (errorCount == 0)
    {
        qInfo() << "All files read successfully.";
        result.append("<All files could be read>");
    }
    else if (errorCount == 1)
    {
        qWarning() << "1 file could not be read!";
        result.append("<1 file could not be read>");
    }
    else
    {
        qWarning().noquote() << "<" + QString::number(errorCount) + " files could not be read!";
        result.append("<" + QString::number(errorCount) + " files could not be read!");
    }

    if (pushToOutput(result))
        qInfo() << "Successfully got line count!";
    else
    {
        qCritical() << "Could not push line count to GUI / to file!";
        qDebug().noquote() << "resultStr:" << result;
    }

    countLinesFiles.clear();
    ui->btnLineCounterStart->setText("Start");
}

/// Catches click at 'Output to GUI'
void wDevTools::on_rbnOutputToGUI_clicked()
{
    setOutputVisible(true);
}

/// Catches click at 'Output to File'
void wDevTools::on_rbnOutputToFile_clicked()
{
    setOutputVisible(false);
}

/// Lists alls sound files from a sound.cfg
void wDevTools::on_btnSoundFileLister_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Select files to analyse", set.read("main", "mainDir").toString(), "OMSI sound files (*.cfg)");

    QStringList result;

    foreach (QString current, files)
    {
        QFile file(current);
        file.open(QFile::ReadOnly | QFile::Text);

        QTextStream in (&file);
        QString line;


        while (!in.atEnd())
        {
            line = in.readLine();
            if (line == "[sound]" || line == "[loopsound]")
                result << in.readLine();
        }
    }

    result.removeDuplicates();
    result.sort();

    QString resultStr;
    foreach (QString current, result)
        resultStr += current + "\n";

    pushToOutput(resultStr);
}

