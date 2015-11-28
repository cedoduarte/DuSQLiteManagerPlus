#ifndef AJUSTESDIALOG_H
#define AJUSTESDIALOG_H

#include <QDialog>
#include "EditorTextoSettings.h"

namespace Ui
{
class AjustesDialog;
}

class AjustesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AjustesDialog(QWidget *parent = nullptr);
    ~AjustesDialog();

    EditorTextoSettings getEditorTextSettings() const
    {
        return mEditorTextoSettings;
    }
private slots:
    void on_colorToolButton_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
private:
    Ui::AjustesDialog *ui;
    EditorTextoSettings mEditorTextoSettings;
};

#endif // AJUSTESDIALOG_H
