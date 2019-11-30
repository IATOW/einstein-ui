#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>

namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupDialog(QWidget *parent = 0);
    ~SetupDialog();

private:
    Ui::SetupDialog *ui;

private slots:
    void okButtonClicked();
    void cancelButtonClicked();

signals:
    void emitSettings(int rlayout, int blayout);
};

#endif // SETUPDIALOG_H
