#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QProcess;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    const int u[8] = { -1,0,1,1,1,0,-1,-1 };
    const int v[8] = { 1,1,1,0,-1,-1,-1,0 };

    int chess[6][6] = { 0 };

    int dice;

    bool isAI[3] = { 0 };

    QProcess * engine1;
    QProcess * engine2;

    int move = 1;

    char lastMove[50];

    int Rlayout;
    int Blayout;

    bool is_game_started = false;


    //void updateBoard();
    void setDiceEnabled(bool flag);

    void receiveDice(int dice);

    void getAdjacent(int board[6][6], int pawn, int player, int & ad1, int & ad2);
    void out(const QString & str);
    void makeMove(int ox, int oy, int tx, int ty);

    void sendToEngine(const QString & cmd, QProcess * engine);
    void setEngineRedEnabled(bool flag);
    void setEngineBlueEnabled(bool flag);
    int isGameOver();
    void doGameOver(int player);

    void setup_red_layout(int layout);
    void setup_blue_layout(int layout);

public slots:
    void openSetupDlg();
    void onSettingsReceived(int rlayout, int blayour);

    void randomDice();
    void chooseDice();

    void receiveGridPressed(int player, int i, int j, int what);

    void receiveEngineDataR();
    void loadEngineRClicked();
    void unloadEngineRClicked();

    void receiveEngineDataB();
    void loadEngineBClicked();
    void unloadEngineBClicked();

    void newGame();

    void openHelpDlg();

    void openAboutDlg();

    void clearOutput();
    //void reset();
};

#endif // MAINWINDOW_H
