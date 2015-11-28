#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include "EditorTextoSettings.h"

namespace Ui
{
class MainWindow;
}

class QSqlQueryModel;
class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *e) override;
private slots:
    void on_actionNuevo_triggered();
    void on_actionAbrir_triggered();
    void on_actionQuitar_triggered();
    void on_actionCopiar_triggered();
    void on_actionCortar_triggered();
    void on_actionPegar_triggered();
    void on_treeWidget_doubleClicked(const QModelIndex &index);
    void on_actionEjecutar_SQL_triggered();
    void on_actionAjustes_triggered();
    void on_actionAcerca_de_triggered();
private:
    QStringList getTablesFromDatabase(const QString &filename);
    QModelIndex getDatabaseFromTable(const QModelIndex &index) const;
    void addNewDatabase(const QString &filename);
    bool isDatabase(const QModelIndex &index) const;
    bool isTable(const QModelIndex &index) const;
    void connectObjects();
    void removeDatabase(const QString &filename);
    void updateCurrentDatabase();
    void loadApplicationSettings();
    void saveApplicationSettings();

    Ui::MainWindow *ui;
    QSqlDatabase mDb;
    QSqlQueryModel *mQueryModel;
    QAction *mActionCloseDatabase;
    QAction *mActionSelectTable;
    QAction *mActionCopyFromTable;
    EditorTextoSettings mEditorTextoSettings;
};

#endif // MAINWINDOW_H
