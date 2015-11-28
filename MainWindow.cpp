#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AjustesDialog.h"
#include "Application.h"
#include <QSqlQueryModel>
#include <QFileDialog>
#include <QFileInfo>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QClipboard>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mDb = QSqlDatabase::addDatabase("QSQLITE");
    mQueryModel = new QSqlQueryModel(this);
    mActionCloseDatabase = new QAction(tr("Cerrar"), this);
    mActionSelectTable = new QAction(tr("Seleccionar"), this);
    mActionCopyFromTable = new QAction(tr("Copiar"), this);
    ui->tableView->setModel(mQueryModel);
    ui->treeWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->treeWidget->addActions({mActionSelectTable, mActionCloseDatabase });
    ui->tableView->addActions({ mActionCopyFromTable });
    loadApplicationSettings();
    setWindowTitle(APPNAME);
    connectObjects();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    saveApplicationSettings();
    QMainWindow::closeEvent(e);
}

void MainWindow::on_actionNuevo_triggered()
{
    auto filename = QFileDialog::getSaveFileName(this, "Nuevo");
    if (filename.isEmpty()) {
        return;
    }
    addNewDatabase(filename);
}

void MainWindow::on_actionAbrir_triggered()
{
    auto filenames = QFileDialog::getOpenFileNames(this, "Abrir");
    for (const auto &filename : filenames) {
        addNewDatabase(filename);
    }
    ui->treeWidget->expandAll();
}

void MainWindow::on_actionQuitar_triggered()
{
    close();
}

void MainWindow::on_actionCopiar_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionCortar_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionPegar_triggered()
{
    ui->textEdit->paste();
}

QStringList MainWindow::getTablesFromDatabase(const QString &filename)
{
    QStringList tables;
    mDb.close();
    mDb.setDatabaseName(filename);
    if (mDb.open()) {
        QSqlQuery qry;
        qry.exec("SELECT name FROM sqlite_master WHERE type LIKE 'table'");
        while (qry.next()) {
            tables.append(qry.value(0).toString());
        }
    }
    return tables;
}

void MainWindow::addNewDatabase(const QString &filename)
{
    auto item = new QTreeWidgetItem;
    item->setIcon(0, QIcon(":/base_de_datos.png"));
    item->setText(0, filename);
    QList<QTreeWidgetItem *> tables;
    auto tableNames = getTablesFromDatabase(filename);
    for (const auto &tableName : tableNames) {
        tables.append(new QTreeWidgetItem({tableName}));
    }
    item->addChildren(tables);
    ui->treeWidget->addTopLevelItem(item);
}

bool MainWindow::isDatabase(const QModelIndex &index) const
{
    return index.data().toString().endsWith(".db");
}

bool MainWindow::isTable(const QModelIndex &index) const
{
    return !isDatabase(index);
}

void MainWindow::connectObjects()
{
    connect(mActionCloseDatabase, &QAction::triggered, [&]() {
        if (ui->treeWidget->topLevelItemCount() == 0) {
            return;
        }
        auto idx = ui->treeWidget->currentIndex();
        if (isDatabase(idx)) {
            delete ui->treeWidget->topLevelItem(idx.row());
        } else {
            QMessageBox::critical(this, tr("Error"), tr("No es base de datos"));
        }
    });

    connect(mActionSelectTable, &QAction::triggered, [&]() {
        if (ui->treeWidget->topLevelItemCount() == 0) {
            return;
        }
        auto idx = ui->treeWidget->currentIndex();
        if (isTable(idx)) {
            on_treeWidget_doubleClicked(idx);
        } else {
            QMessageBox::critical(this, tr("Error"), tr("No es tabla"));
        }
    });

    connect(mActionCopyFromTable, &QAction::triggered, [&]() {
        qApp->clipboard()->setText(ui->tableView->currentIndex().data()
                                   .toString());
    });
}

void MainWindow::removeDatabase(const QString &filename)
{
    const int count = ui->treeWidget->topLevelItemCount();
    for (int ix = 0; ix < count; ++ix) {
        auto db = ui->treeWidget->topLevelItem(ix);
        if (db->text(0) == filename) {
            delete db;
            return;
        }
    }
}

void MainWindow::updateCurrentDatabase()
{
    auto filename = mDb.databaseName();
    removeDatabase(filename);
    addNewDatabase(filename);
    ui->treeWidget->expandAll();
}

void MainWindow::loadApplicationSettings()
{
    QSettings settings;

    settings.beginGroup("editorTextoSettings");
    auto editorTextFont = settings.value("font").value<QFont>();
    int editorTextPointSize = settings.value("pointSize").toInt();
    auto editorTextColor = settings.value("color").value<QColor>();
    ui->textEdit->setFont(editorTextFont);
    ui->textEdit->setFontPointSize(editorTextPointSize);
    ui->textEdit->setTextColor(editorTextColor);
    mEditorTextoSettings.setFuente(editorTextFont);
    mEditorTextoSettings.setTamanyo(editorTextPointSize);
    mEditorTextoSettings.setColor(editorTextColor);
    settings.endGroup();

    settings.beginGroup("mainWindow");
    auto mainWindowGeometry = settings.value("geometry").value<QRect>();
    setGeometry(mainWindowGeometry);
    settings.endGroup();
}

void MainWindow::saveApplicationSettings()
{
    QSettings settings;

    settings.beginGroup("editorTextoSettings");
    settings.setValue("font", mEditorTextoSettings.getFuente());
    settings.setValue("pointSize", mEditorTextoSettings.getTamanyo());
    settings.setValue("color", mEditorTextoSettings.getColor());
    settings.endGroup();

    settings.beginGroup("mainWindow");
    settings.setValue("geometry", geometry());
    settings.endGroup();
}

QModelIndex MainWindow::getDatabaseFromTable(const QModelIndex &index) const
{
    if (!isTable(index)) {
        return index;
    }
    QModelIndex idx = index;
    while (!idx.data().toString().endsWith(".db")) {
        idx = ui->treeWidget->indexAbove(idx);
    }
    return idx;
}

void MainWindow::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    if (!isTable(index)) {
        return;
    }
    mDb.setDatabaseName(getDatabaseFromTable(index).data().toString());
    mDb.open();
    mQueryModel->setQuery("SELECT * FROM " + index.data().toString());
}

void MainWindow::on_actionEjecutar_SQL_triggered()
{
    if (!mDb.isOpen()) {
        ui->statusBar->showMessage(tr("Base de datos no está abierta"));
        return;
    }
    QSqlQuery q(ui->textEdit->toPlainText());
    mQueryModel->setQuery(q);
    auto lastError = q.lastError();
    if (lastError.isValid()) {
        ui->statusBar->showMessage(lastError.text());
    } else {
        ui->statusBar->showMessage(tr("OK"));
        updateCurrentDatabase();
    }
}

void MainWindow::on_actionAjustes_triggered()
{
    AjustesDialog d(this);
    if (d.exec() == QDialog::Rejected) {
        return;
    }
    mEditorTextoSettings = d.getEditorTextSettings();
    ui->textEdit->setFont(mEditorTextoSettings.getFuente());
    ui->textEdit->setFontPointSize(mEditorTextoSettings.getTamanyo());
    ui->textEdit->setTextColor(mEditorTextoSettings.getColor());
}

void MainWindow::on_actionAcerca_de_triggered()
{
    QMessageBox::about(this, tr("Acerca de %1").arg(APPNAME),
                       tr("Autor: Carlos Enrique Duarte Ortiz\n"
                          "Software: %1\n"
                          "Compañía: %2").arg(APPNAME).arg(ORGNAME));
}
