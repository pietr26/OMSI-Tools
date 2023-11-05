#include "wbugdoc.h"
#include "ui_wbugdoc.h"

wBugDoc::wBugDoc(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wBugDoc)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - BugDoc");

    // Load prefs
    setStyleSheet(set.read("main", "theme").toString());
    if (set.read(objectName(), "screenshotScale").toInt() == 0) set.write(objectName(), "screenshotScale", 2);

    ui->centralwidget->setEnabled(false);
    ui->lblUnsavedChanges->setVisible(false);

    if (RegisterHotKey(NULL, 1, MOD_ALT, 'B')) {
        qInfo() << "GlobalShortcut: ALT + B";
    } else {
        qWarning() << "GlobalShortcut: ALT + B";
    }

    ui->statusbar->addPermanentWidget(ui->pgbProgress);
    ui->statusbar->addPermanentWidget(ui->btnWait);
    ui->pgbProgress->setVisible(false);

    qInfo().noquote() << objectName() + " started";
}

wBugDoc::~wBugDoc()
{
    UnregisterHotKey(NULL, 1);
    delete ui;
}

void wBugDoc::on_actionBack_to_home_triggered()
{
    close();
    emit backToHome();
}


void wBugDoc::on_actionClose_triggered()
{
    QApplication::quit();
}

void wBugDoc::on_actionLoad_triggered()
{
    ui->centralwidget->setEnabled(false);
    QString result = QFileDialog::getExistingDirectory(this, "Select / create BugDoc folder...", (projectFolder.isEmpty() ? "C:/Users/pietr/Desktop" : projectFolder));

    if (!QDir(result).exists())
    {
        QMessageBox::warning(this, "Directory not found","The selected directory doesn't exists.");
        return;
    }

    projectFolder = result;
    dbHandler.dbPath = projectFolder + "/data/data.db";
    dbHandler.setupDatabase("CREATE TABLE bugs (ID INTEGER, title TEXT, description TEXT, location TEXT, picturePath TEXT, PRIMARY KEY(ID AUTOINCREMENT))");

    ui->centralwidget->setEnabled(true);
}

void wBugDoc::loadUI()
{
    QSqlQueryModel *bugModel = new QSqlQueryModel();
    dbHandler.openDB();
    bugModel->setQuery(dbHandler.doAction("SELECT ID, title FROM bugs"));
    ui->tvwBugs->setModel(bugModel);
    dbHandler.closeDB();
}

void wBugDoc::loadSingleItem()
{
    int id = ui->tvwBugs->selectionModel()->selectedRows().at(0).data().toInt();

    dbHandler.openDB();
    QSqlQueryModel *singleBugModel = new QSqlQueryModel();
    singleBugModel->setQuery(dbHandler.doAction(QString("SELECT * FROM bugs WHERE ID = %1").arg(id)));

    ui->ledID->setText(singleBugModel->index(0, 0).data().toString());
    ui->ledTitle->setText(singleBugModel->index(0, 1).data().toString());
    ui->pteDescription->setPlainText(singleBugModel->index(0, 2).data().toString());
    ui->ledLocation->setText(singleBugModel->index(0, 3).data().toString());
    ui->ledPicturePath->setText(singleBugModel->index(0, 4).data().toString());
    loadPicture();

    dbHandler.closeDB();
    ui->lblUnsavedChanges->setVisible(false);
}

void wBugDoc::clearSingleBug()
{
    ui->ledID->clear();
    ui->ledTitle->clear();
    ui->pteDescription->clear();
    ui->ledLocation->clear();
    ui->ledPicturePath->clear();
    ui->lblPicture->setPixmap(QPixmap());

    ui->lblUnsavedChanges->setVisible(false);
}

void wBugDoc::loadPicture()
{
    ui->lblPicture->setPixmap(QPixmap(ui->ledPicturePath->text()));
}

void wBugDoc::on_tvwBugs_activated(const QModelIndex &index)
{
    Q_UNUSED(index);
    loadSingleItem();
}

void wBugDoc::on_btnSaveEntry_clicked()
{
    dbHandler.openDB();
    dbHandler.doAction(QString("UPDATE bugs SET title = \"%2\", description = \"%3\", location = \"%4\", picturePath = \"%5\" WHERE ID = %1").arg(ui->ledID->text(), ui->ledTitle->text(), ui->pteDescription->toPlainText(), ui->ledLocation->text(), ui->ledPicturePath->text()));
    dbHandler.closeDB();

    ui->lblUnsavedChanges->setVisible(false);

    int currentRow = ui->tvwBugs->currentIndex().row();
    loadUI();
    ui->tvwBugs->setCurrentIndex(ui->tvwBugs->model()->index(currentRow, 0));
}

void wBugDoc::on_ledID_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->lblUnsavedChanges->setVisible(true);
}

void wBugDoc::on_ledTitle_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->lblUnsavedChanges->setVisible(true);
}

void wBugDoc::on_pteDescription_textChanged()
{
    ui->lblUnsavedChanges->setVisible(true);
}

void wBugDoc::on_ledLocation_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->lblUnsavedChanges->setVisible(true);
}

void wBugDoc::on_ledPicturePath_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->lblUnsavedChanges->setVisible(true);
    loadPicture();
}


void wBugDoc::on_btnDelete_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm deletion", "Really delete the selected bug entry?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if (reply != QMessageBox::Yes) return;

    int currentRow = ui->tvwBugs->currentIndex().row();

    dbHandler.openDB();
    QSqlQueryModel *singleBugModel = new QSqlQueryModel();
    singleBugModel->setQuery(dbHandler.doAction(QString("DELETE FROM bugs WHERE ID = %1").arg(ui->tvwBugs->selectionModel()->selectedRows().at(0).data().toInt())));
    dbHandler.closeDB();

    loadUI();
    clearSingleBug();

    try
    {
        ui->tvwBugs->selectRow(currentRow);
    }
    catch(...) { }
}


void wBugDoc::on_btnAdd_clicked()
{
    dbHandler.openDB();
    dbHandler.doAction("INSERT INTO bugs (title) VALUES (\"\")");
    dbHandler.closeDB();

    loadUI();
    ui->tvwBugs->selectRow(ui->tvwBugs->model()->rowCount() - 1);
    loadSingleItem();
}


void wBugDoc::on_btnWait_clicked()
{
    showMinimized();

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            QScreen *screen = QGuiApplication::primaryScreen();

            if (!screen) return;
            QPixmap originalScreenshot = screen->grabWindow(0);

            QPixmap screenshot = originalScreenshot.scaled(QSize(originalScreenshot.width() / set.read(objectName(), "screenshotScale").toInt(), originalScreenshot.height() / set.read(objectName(), "screenshotScale").toInt()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            const QString screenshotPath = projectFolder + QString("/data/bug_%1.jpg").arg(QDateTime().currentDateTime().toString("yyyyMMdd_hhmmss"));

            if (!screenshot.save(screenshotPath, "JPEG")) QMessageBox::warning(this, "Speicherfehler", "Der Screenshot konnte nicht gespeichert werden. Prozedur bitte wiederholen!");

            showNormal();
            on_btnAdd_clicked();
            qApp->processEvents();

            ui->ledPicturePath->setText(screenshotPath);

            on_btnSaveEntry_clicked();

            break;
        }
    }
}

void wBugDoc::on_actionHTML_triggered()
{
    dbHandler.openDB();

    QString html = "<html><body>";

    QSqlQueryModel *countModel = new QSqlQueryModel();
    countModel->setQuery(dbHandler.doAction("SELECT COUNT(*) FROM bugs"));
    int bugCount = countModel->index(0, 0).data().toInt();

    QSqlQueryModel *bugsModel = new QSqlQueryModel();
    bugsModel->setQuery(dbHandler.doAction("SELECT * FROM bugs"));

    for (int i = 0; i < bugCount; i++)
    {
        html += "<h2>" + (bugsModel->index(i, 1).data().toString() == "" ? "(ohne Titel)" : bugsModel->index(i, 1).data().toString()) + "</h2>";
        html += "<p><strong>Ort:</strong> " + (bugsModel->index(i, 3).data().toString().isEmpty() ? "-" : bugsModel->index(i, 3).data().toString()) + "</p>";
        html += "<p><strong>Beschreibung:</strong>" + (bugsModel->index(i, 2).data().toString().contains("\n") ? "<br>" : QString(" ")) + (bugsModel->index(i, 2).data().toString().isEmpty() ? "-" : bugsModel->index(i, 2).data().toString()) + "</p>";
        if (QFile(bugsModel->index(i, 4).data().toString()).exists())
        {
            html += "<p><br></p>";
            html += "<p><img src=\"" + bugsModel->index(i, 4).data().toString() + "\" width='600'></p>";
        }

        html += "<hr>";
    }

    dbHandler.closeDB();

    html += "</body></html>";
    html = html.replace("\n", "<br>");
    qInfo().noquote() << html;

    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize::Custom);
    printer.setOutputFileName(projectFolder + "/output_" + QDateTime().currentDateTime().toString("yyyyMMdd_hhmmss") + ".pdf");

    document.print(&printer);

    QMessageBox::information(this, "Print finished", "HTML printed successfully:\n" + printer.outputFileName());

}

void wBugDoc::on_actionPreferences_triggered()
{
    WPREFERENCES = new wPreferences(this, "devTools");
    WPREFERENCES->setWindowModality(Qt::ApplicationModal);
    WPREFERENCES->show();
}


void wBugDoc::on_btnWebDiskCopy_clicked()
{
    ui->pgbProgress->setVisible(true);
    ui->statusbar->showMessage("Upload picture, getting HTML...");
    on_btnSaveEntry_clicked();

    QString html;

    dbHandler.openDB();
    QSqlQueryModel *bugsModel = new QSqlQueryModel();
    bugsModel->setQuery(dbHandler.doAction(QString("SELECT * FROM bugs WHERE ID = %1").arg(ui->tvwBugs->selectionModel()->selectedRows().at(0).data().toInt())));

    html += "<h2>" + (bugsModel->index(0, 1).data().toString() == "" ? "(ohne Titel)" : bugsModel->index(0, 1).data().toString()) + "</h2>";
    html += "<p><strong>Ort:</strong> " + (bugsModel->index(0, 3).data().toString().isEmpty() ? "-" : bugsModel->index(0, 3).data().toString()) + "</p>";
    html += "<p><strong>Beschreibung:</strong>" + (bugsModel->index(0, 2).data().toString().contains("\n") ? "<br>" : QString(" ")) + (bugsModel->index(0, 2).data().toString().isEmpty() ? "-" : bugsModel->index(0, 2).data().toString()) + "</p>";

    if (QFile(bugsModel->index(0, 4).data().toString()).exists())
    {
        html += "<p><br></p>";

        QFile *picture = new QFile(bugsModel->index(0, 4).data().toString());

        QString pictureUrl = nc.post(OTLinks::bugDocUploads, picture);

        qInfo() << pictureUrl;

        html += "<p><img src=\"" + pictureUrl + "\"></p>";
    }

    qInfo() << html;

    dbHandler.closeDB();

    ui->statusbar->clearMessage();
    ui->pgbProgress->setVisible(false);
}

