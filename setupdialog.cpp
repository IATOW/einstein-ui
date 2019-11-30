#include "setupdialog.h"
#include "ui_setupdialog.h"

SetupDialog::SetupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupDialog)
{
    ui->setupUi(this);

    connect(ui->okButton, SIGNAL(pressed()), this, SLOT(okButtonClicked()));
    connect(ui->cancelButton, SIGNAL(pressed()), this, SLOT(cancelButtonClicked()));
}

SetupDialog::~SetupDialog()
{
    delete ui;
}

void SetupDialog::okButtonClicked()
{
    emit emitSettings(ui->rlayoutText->text().toInt(), ui->blayoutText->text().toInt());
    this->close();
}

void SetupDialog::cancelButtonClicked()
{
    this->close();
}
