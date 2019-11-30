#include "gameoverdlg.h"
#include "ui_gameoverdlg.h"

gameOverDlg::gameOverDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gameOverDlg)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(close()));
}

gameOverDlg::~gameOverDlg()
{
    delete ui;
}

void gameOverDlg::wins(int player)
{
    if(player == 1)
    {
        ui->label->setText(QString("Red Wins"));
        ui->label->setStyleSheet("color: rgba(255, 83, 77, 128);");
        ui->pushButton->setStyleSheet("border-left: 3px solid rgb(255, 83, 77);");
    }
    else if(player == 2)
    {
        ui->label->setText(QString("Blue Wins"));
        ui->label->setStyleSheet("color: rgba(37, 198, 252, 128);");
        ui->pushButton->setStyleSheet("border-right: 3px solid rgb(37, 198, 252);");
    }
}
