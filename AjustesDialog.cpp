#include "AjustesDialog.h"
#include "ui_AjustesDialog.h"
#include <QColorDialog>

AjustesDialog::AjustesDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AjustesDialog)
{
    ui->setupUi(this);
}

AjustesDialog::~AjustesDialog()
{
    delete ui;
}

void AjustesDialog::on_colorToolButton_clicked()
{
    auto color = QColorDialog::getColor();
    ui->colorLineEdit->setText(color.name());
    mEditorTextoSettings.setColor(color);
}

void AjustesDialog::on_buttonBox_accepted()
{
    mEditorTextoSettings.setFuente(ui->fontComboBox->currentFont());
    mEditorTextoSettings.setTamanyo(ui->pointSizeSpinBox->value());
    accept();
}

void AjustesDialog::on_buttonBox_rejected()
{
    reject();
}
