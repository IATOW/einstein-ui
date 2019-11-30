#ifndef GAMEOVERDLG_H
#define GAMEOVERDLG_H

#include <QDialog>

namespace Ui {
class gameOverDlg;
}

class gameOverDlg : public QDialog
{
    Q_OBJECT

public:
    explicit gameOverDlg(QWidget *parent = 0);
    ~gameOverDlg();

    void wins(int player);
private:
    Ui::gameOverDlg *ui;

public slots:


signals:

};

#endif // GAMEOVERDLG_H
