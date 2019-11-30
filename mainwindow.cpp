#include <QPainter>
#include <QStyledItemDelegate>
#include <QProcess>
#include <QTextStream>
#include <QFileDialog>
#include <QDebug>


#include "chessgrid.h"
#include "board.h"
#include "setupdialog.h"
#include "gameoverdlg.h"
#include "aboutdlg.h"
#include "helpdlg.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->action_setup, SIGNAL(triggered(bool)), this, SLOT(openSetupDlg()));

    connect(ui->randomDiceButton, SIGNAL(pressed()), this, SLOT(randomDice()));
    connect(ui->chooseDiceButton, SIGNAL(pressed()), this, SLOT(chooseDice()));

    connect(ui->action_how_to_use, SIGNAL(triggered(bool)), this, SLOT(openHelpDlg()));

    connect(ui->actionabout, SIGNAL(triggered(bool)), this, SLOT(openAboutDlg()));

    connect(ui->action_clear, SIGNAL(triggered(bool)), this, SLOT(clearOutput()));

    connect(ui->loadEngineR, SIGNAL(pressed()), this, SLOT(loadEngineRClicked()));
    connect(ui->loadEngineB, SIGNAL(pressed()), this, SLOT(loadEngineBClicked()));

    connect(ui->unloadEngineR, SIGNAL(pressed()), this, SLOT(unloadEngineRClicked()));
    connect(ui->unloadEngineB, SIGNAL(pressed()), this, SLOT(unloadEngineBClicked()));

    connect(ui->action_new_game, SIGNAL(triggered(bool)), this, SLOT(newGame()));
    //connect(ui->action_reset, SIGNAL(triggered(bool)), this, SLOT(reset()));

    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 5; j++)
            connect(ui->board->getGrid(i, j), SIGNAL(pressed(int,int,int,int)), this, SLOT(receiveGridPressed(int,int,int,int)));

    QStyledItemDelegate * delegate = new QStyledItemDelegate();
    ui->comboBox->setItemDelegate(delegate);

    this->setDiceEnabled(false);
    //this->setEngineBlueEnabled(false);
    //this->setEngineRedEnabled(false);
    //receiveSettings(1, 123456, 123456);

    this->onSettingsReceived(123456, 123456);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openSetupDlg()
{
    SetupDialog * dlg = new SetupDialog(this);
    connect(dlg, SIGNAL(emitSettings(int,int)), this, SLOT(onSettingsReceived(int,int)));
    dlg->show();
}

void MainWindow::randomDice()
{
    srand(time(0));
    dice = rand() % 6 + 1;
    char buffer[5];
    sprintf(buffer, "%d", dice);
    ui->lcdNumber->display(buffer);

    receiveDice(dice);
}

void MainWindow::chooseDice()
{
    dice = ui->comboBox->currentText().toInt();
    char buffer[5];
    sprintf(buffer, "%d", dice);
    ui->lcdNumber->display(buffer);

    receiveDice(dice);
}

void MainWindow::setDiceEnabled(bool flag)
{
    ui->randomDiceButton->setEnabled(flag);
    ui->chooseDiceButton->setEnabled(flag);
    ui->comboBox->setEnabled(flag);
    ui->lcdNumber->setEnabled(flag);
}

void MainWindow::receiveDice(int dice)
{
    static int first = 1;

    setDiceEnabled(false);

    int ad1, ad2;

    getAdjacent(chess, dice * 10 + move, move, ad1, ad2);
    if(move == 1)
    {
        out(QString("<font color = \"#ff534d\">red:</font>"));
        out(QString("<font color = \"#ff534d\">dice at %1</font>").arg(dice));
    }
    else if(move == 2)
    {
        out(QString("<font color = \"#25c6fc\">blue:</font>"));
        out(QString("<font color = \"#25c6fc\">dice at %1</font>").arg(dice));
    }
    //out(QString("you can move %1 %2 %3\n").arg(dice).arg(ad1 / 10).arg(ad2 / 10));

    if(!isAI[move])
    {
        for(int i = 1; i <= 5; i++)
        {
            for(int j = 1; j <= 5; j++)
            {
                if(chess[i][j] == ad1 || chess[i][j] == ad2 || chess[i][j] == dice * 10 + move)
                {
                    ui->board->setGridState(i - 1, j - 1, 1);
                }
            }
        }
    }
    else
    {
        if(move == 1)
        {
            //out(lastMove);
            //engine1->write(lastMove);
            //engine1->write(QString("%1\r\n").arg(dice).toStdString().c_str());
            //engine1->waitForReadyRead();
            qDebug() << lastMove;
            if (first == 1) {
                first = 0;
                this->sendToEngine(QString("%1\r\n").arg(dice), engine1);
                //engine1->waitForReadyRead();
            } else {
                this->sendToEngine(lastMove, engine1);
                this->sendToEngine(QString("%1\r\n").arg(dice), engine1);
                //engine1->waitForReadyRead();
            }

            //engine1->waitForBytesWritten();
        }
        else if(move == 2)
        {
            //out(lastMove);
            //engine2->write(lastMove);
            qDebug() << lastMove;
            this->sendToEngine(lastMove, engine2);
            this->sendToEngine(QString("%1\r\n").arg(dice), engine2);
            //engine2->waitForReadyRead();
            //engine2->write(QString("%1\r\n").arg(dice).toStdString().c_str());
        }
        for(int i = 1; i <= 5; i++)
        {
            for(int j = 1; j <= 5; j++)
            {
                if(chess[i][j] == ad1 || chess[i][j] == ad2 || chess[i][j] == dice * 10 + move)
                {
                    ui->board->setGridState(i - 1, j - 1, 3);
                }
            }
        }
    }

    ui->board->update();
}

void MainWindow::getAdjacent(int board[6][6], int pawn, int player, int & ad1, int & ad2)
{
    ad1 = 0x3f3f3f3f;
    ad2 = -0x3f3f3f3f;

    //if(player == 1)
    //{
        for(int i = 1; i <= 5; i++)
        {
            for(int j = 1; j <= 5; j++)
            {
                if(chess[i][j] != 0 && chess[i][j] % 10 == player && chess[i][j] >= pawn && chess[i][j] - pawn < ad1 - pawn)
                    ad1 = chess[i][j];
                if(chess[i][j] != 0 && chess[i][j] % 10 == player && chess[i][j] <= pawn && pawn - chess[i][j] < pawn - ad2)
                    ad2 = chess[i][j];
            }
        }
    //}
}

void MainWindow::receiveGridPressed(int player, int i, int j, int what)
{
    static int ox, oy;

    //out(QString("pressed %1 %2\n").arg(i).arg(j));
    if(what == 1)
    {
        ox = i;
        oy = j;
        ui->board->clearState(4);
        //if(player == 2)
        //{
            for(int ii = 4 * player - 3; ii <= 4 * player - 1; ii++)
            {
                int x = i + u[ii];
                int y = j + v[ii];
                if(x >= 1 && x <= 5 && y >= 1 && y <= 5)
                {
                    ui->board->setGridState(x - 1, y - 1, 4);
                }
            }
        //}
    }
    else if(what == 2)
    {
        // move from (ox, oy) to (i, j)
        setDiceEnabled(true);

        if(move == 1)
            out(QString("<font color = \"#ff534d\">move %1%2 to %3%4<br></font>").arg(ox - 1).arg(oy - 1).arg(i - 1).arg(j - 1));
        else if(move == 2)
            out(QString("<font color = \"#25c6fc\">move %1%2 to %3%4<br></font>").arg(ox - 1).arg(oy - 1).arg(i - 1).arg(j - 1));

        sprintf(lastMove, "move %d %d %d\r\n", (ox - 1) * 10 + oy - 1, (i - 1) * 10 + j - 1, 0);

        makeMove(ox, oy, i, j);
        ui->board->updateGrid(chess);
        ui->board->clearState(5);
        ui->board->clearState(4);
        ui->board->clearState(1);

        move = 3 - move;
//        if(isAI[move])
//        {
//            if(move == 1)
//                sendToEngine(lastMove, engine1);
//            else if(move == 2)
//                sendToEngine(lastMove, engine2);
//        }

        int t = isGameOver();
        if(t)
            doGameOver(t);
    }
    else if(what == 0)
    {
        ui->board->clearState(5);
        ui->board->clearState(4);
    }

    ui->board->update();
}

void MainWindow::out(const QString & str)
{
    ui->textEdit->append(str);
}

void MainWindow::makeMove(int ox, int oy, int tx, int ty)
{
    chess[tx][ty] = chess[ox][oy];
    chess[ox][oy] = 0;
}

void MainWindow::sendToEngine(const QString & cmd, QProcess * engine)
{
    //engine1->write(str.toLatin1().constData(), str.length());
    engine->write(cmd.toLatin1().constData(), cmd.length());
    engine->waitForBytesWritten();

    if (engine == engine1) {
        qDebug() << "to 1: " << cmd;
    } else {
        qDebug() << "to 2: " << cmd;
    }
}

void MainWindow::receiveEngineDataR()
{
    static int flag = 0;

    char temp[1000];


    while (engine1->canReadLine()) {
        engine1->readLine(temp, 1000);
        qDebug() << "1: " << temp;
        if (flag == 0) {
            ui->redName->setText(temp);
            flag = 1;
        } else if (flag == 1) {
            int layout;
            sscanf(temp, "%d", &layout);
            this->Rlayout = layout;

            this->setup_red_layout(layout);

            flag = 2;
        } else if (flag == 2) {
            int from, to, dice;
            sscanf(temp, "move %d %d %d", &from, &to, &dice);
            makeMove(from / 10 + 1, from % 10 + 1, to / 10 + 1, to % 10 + 1);

            sprintf(lastMove, "move %d %d %d\r\n", from, to, dice);

            out(QString("<font color = \"#ff534d\">move %1 to %2<br></font>").arg(from).arg(to));

            while (engine1->canReadLine()) {
                engine1->readLine(temp, 1000);
                out(QString("<font color = \"#ff534d\">%1\n</font>").arg(temp));
            }

            this->move = 3 - this->move;

            ui->board->updateGrid(chess);
            ui->board->clearState(3);

            int t = isGameOver();
            if (t)
                doGameOver(t);

            setDiceEnabled(true);
        }
    }
}

void MainWindow::receiveEngineDataB()
{
    static int flag = 0;
    //QString data = engine2->readAll();
    //out(QString("%1\n").arg(flag));
    //out(data);

    char temp[1000];

    while (engine2->canReadLine()) {
        engine2->readLine(temp, 1000);
        qDebug() << "2: " << temp;
        if (flag == 0) {
            ui->blueName->setText(temp);
            flag = 1;
        } else if (flag == 1) {
            int layout;
            sscanf(temp, "%d", &layout);
            this->Blayout = layout;
            this->setup_blue_layout(layout);

            flag = 2;
        } else if (flag == 2) {
            int from, to, dice;
            sscanf(temp, "move %d %d %d", &from, &to, &dice);
            makeMove(from / 10 + 1, from % 10 + 1, to / 10 + 1, to % 10 + 1);

            sprintf(lastMove, "move %d %d %d\r\n", from, to, dice);

            out(QString("<font color = \"#25c6fc\">move %1 to %2<br></font>").arg(from).arg(to));

            while (engine2->canReadLine()) {
                engine2->readLine(temp, 1000);
                out(QString("<font color = \"#25c6fc\">%1\n</font>").arg(temp));
            }

            this->move = 3 - this->move;

            ui->board->updateGrid(chess);
            ui->board->clearState(3);

            int t = isGameOver();
            if (t)
                doGameOver(t);

            setDiceEnabled(true);
        }
    }
}

void MainWindow::loadEngineRClicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "open engine");

    if (filename == "") {
        return;
    }

    engine1 = new QProcess(this);

    connect(engine1, SIGNAL(readyReadStandardOutput()), this, SLOT(receiveEngineDataR()));

    engine1->start(filename);
    engine1->waitForStarted();

    sendToEngine("name?\r\n", engine1);
    engine1->waitForReadyRead();

    sendToEngine("redfirst\r\n", engine1);
    sendToEngine("red\r\n", engine1);
    engine1->waitForReadyRead();

    sendToEngine("begin\r\n", engine1);

    //sendToEngine("123456\r\n", engine1);
    //engine1->waitForReadyRead();

    //if((!isAI[1] || engine1) && (!isAI[2] || engine2))
    //    setDiceEnabled(true);

    isAI[1] = true;
}

void MainWindow::loadEngineBClicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "open engine");

    if (filename == "") {
        return;
    }

    engine2 = new QProcess(this);

    connect(engine2, SIGNAL(readyReadStandardOutput()), this, SLOT(receiveEngineDataB()));

    engine2->start(filename);
    engine2->waitForStarted();

    sendToEngine("name?\r\n", engine2);
    engine2->waitForReadyRead();

    sendToEngine("redfirst\r\n", engine2);
    sendToEngine("blue\r\n", engine2);
    engine2->waitForReadyRead();

    sendToEngine("begin\r\n", engine2);

    //sendToEngine("123456\r\n", engine2);

    //if((!isAI[1] || engine1) && (!isAI[2] || engine2))
    //    setDiceEnabled(true);
    isAI[2] = true;
}

void MainWindow::setEngineRedEnabled(bool flag)
{
    ui->loadEngineR->setEnabled(flag);
    ui->unloadEngineR->setEnabled(flag);
}

void MainWindow::setEngineBlueEnabled(bool flag)
{
    ui->loadEngineB->setEnabled(flag);
    ui->unloadEngineB->setEnabled(flag);
}

void MainWindow::unloadEngineBClicked()
{
    engine2->close();
    delete engine2;
    engine2 = nullptr;

    //setDiceEnabled(false);
    isAI[2] = false;
}

void MainWindow::unloadEngineRClicked()
{
    engine1->close();
    delete engine1;
    engine1 = nullptr;

    //setDiceEnabled(false);
    isAI[1] = false;
}

void MainWindow::newGame()
{
    setDiceEnabled(true);

    qDebug() << Rlayout << Blayout;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            this->chess[i][j] = 0;
        }
    }
    setup_blue_layout(this->Blayout);
    setup_red_layout(this->Rlayout);

    if (isAI[1]) {
        sendToEngine(QString("%1\r\n").arg(this->Blayout), engine1);
        //engine1->waitForBytesWritten();
    }

    if (isAI[2]) {
        sendToEngine(QString("%1\r\n").arg(this->Rlayout), engine2);
        //engine2->waitForBytesWritten();
    }

    ui->loadEngineB->setEnabled(false);
    ui->loadEngineR->setEnabled(false);
    ui->unloadEngineB->setEnabled(false);
    ui->unloadEngineR->setEnabled(false);

    ui->action_new_game->setEnabled(false);

    //qDebug() << "aaa";
}

int MainWindow::isGameOver()
{
    if(chess[1][1] % 10 == 2)
        return 2;
    if(chess[5][5] % 10 == 1)
        return 1;

    int numR = 0, numB = 0;
    for(int i = 1; i <= 5; i++)
    {
        for(int j = 1; j <= 5; j++)
        {
            if(chess[i][j] % 10 == 1)
                numR++;
            if(chess[i][j] % 10 == 2)
                numB++;
        }
    }

    if(numR == 0)
        return 2;
    if(numB == 0)
        return 1;
    return 0;
}

void MainWindow::doGameOver(int player)
{
    setDiceEnabled(false);

    ui->loadEngineB->setEnabled(true);
    ui->loadEngineR->setEnabled(true);
    ui->unloadEngineB->setEnabled(true);
    ui->unloadEngineR->setEnabled(true);

    ui->action_new_game->setEnabled(true);

    gameOverDlg * dlg = new gameOverDlg(this);

    dlg->wins(player);

    dlg->show();
}

void MainWindow::openHelpDlg()
{
    HelpDlg * dlg = new HelpDlg(this);

    dlg->show();
}

void MainWindow::openAboutDlg()
{
    AboutDlg * dlg = new AboutDlg(this);

    dlg->show();
}

void MainWindow::clearOutput()
{
    ui->textEdit->clear();
}

//void MainWindow::reset()
//{
//    if(isAI[1])
//    {
//        isAI[1] = false;
//        engine1->close();
//        engine1 = nullptr;
//        ui->redName->setText("red");
//    }

//    if(isAI[2])
//    {
//        isAI[2] = false;
//        engine2->close();
//        engine2 = nullptr;
//        ui->blueName->setText("blue");
//    }

//    receiveSettings(1, 123456, 123456);

//    setDiceEnabled(false);

//    for(int i = 1; i <= 5; i++)
//        ui->board->clearState(i);
//}

void MainWindow::onSettingsReceived(int rlayout, int blayout) {
    //memset(chess, 0, sizeof(chess));

    if (this->is_game_started) {
        return;
    }

    this->Rlayout = rlayout;
    this->Blayout = blayout;

    char temp[10];
    bool update_flag = false;

    if (this->isAI[1] == false) {
        sprintf(temp, "%d", rlayout);
        ui->layoutR->setText(temp);

        chess[3][1] = (rlayout % 10) * 10 + 1;
        rlayout /= 10;
        chess[2][2] = (rlayout % 10) * 10 + 1;
        rlayout /= 10;
        chess[2][1] = (rlayout % 10) * 10 + 1;
        rlayout /= 10;
        chess[1][3] = (rlayout % 10) * 10 + 1;
        rlayout /= 10;
        chess[1][2] = (rlayout % 10) * 10 + 1;
        rlayout /= 10;
        chess[1][1] = (rlayout % 10) * 10 + 1;

        update_flag = true;
    }

    if (this->isAI[2] == false) {
        sprintf(temp, "%d", blayout);
        ui->layoutB->setText(temp);

        chess[3][5] = (blayout % 10) * 10 + 2;
        blayout /= 10;
        chess[4][4] = (blayout % 10) * 10 + 2;
        blayout /= 10;
        chess[4][5] = (blayout % 10) * 10 + 2;
        blayout /= 10;
        chess[5][3] = (blayout % 10) * 10 + 2;
        blayout /= 10;
        chess[5][4] = (blayout % 10) * 10 + 2;
        blayout /= 10;
        chess[5][5] = (blayout % 10) * 10 + 2;

        update_flag = true;
    }

    if (update_flag) {
        ui->board->updateGrid(chess);
    }
}

void MainWindow::setup_red_layout(int layout) {
    ui->layoutR->setText(QString("%1").arg(layout));
    this->Rlayout = layout;

    chess[3][1] = (layout % 10) * 10 + 1;
    layout /= 10;
    chess[2][2] = (layout % 10) * 10 + 1;
    layout /= 10;
    chess[2][1] = (layout % 10) * 10 + 1;
    layout /= 10;
    chess[1][3] = (layout % 10) * 10 + 1;
    layout /= 10;
    chess[1][2] = (layout % 10) * 10 + 1;
    layout /= 10;
    chess[1][1] = (layout % 10) * 10 + 1;



    ui->board->updateGrid(this->chess);
}

void MainWindow::setup_blue_layout(int layout) {

    ui->layoutB->setText(QString("%1").arg(layout));
    this->Blayout = layout;

    chess[3][5] = (layout % 10) * 10 + 2;
    layout /= 10;
    chess[4][4] = (layout % 10) * 10 + 2;
    layout /= 10;
    chess[4][5] = (layout % 10) * 10 + 2;
    layout /= 10;
    chess[5][3] = (layout % 10) * 10 + 2;
    layout /= 10;
    chess[5][4] = (layout % 10) * 10 + 2;
    layout /= 10;
    chess[5][5] = (layout % 10) * 10 + 2;

    ui->board->updateGrid(this->chess);
}
