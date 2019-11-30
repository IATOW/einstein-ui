#ifndef GRID_H
#define GRID_H

#include <QObject>
#include <QWidget>

class ChessGrid : public QWidget
{
    Q_OBJECT

private:
    //static const int SIZE = 100;

    /*
     * 0 : vacant cell
     * 1 : cell that can move, normal, manual
     * 2 : cell that can move, hover, manual
     * 3 : cell that can move, normal, AI
     * 4 : cell to move to, normal
     * 5 : cell to move to, hover
     *
     */

    int state = 0;
    bool active = false;
    int player;
    int number = 0;

public:
    explicit ChessGrid(QWidget *parent = nullptr);

    int mapx, mapy;

    void setNumber(int num) { number = num; }
    int getNumber() { return number; }
    void setState(int nstate) { state = nstate; }
    int getState() { return state; }

protected:
    void enterEvent(QEvent * event);
    void leaveEvent(QEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void paintEvent(QPaintEvent * event);

signals:
    // what == 1 : it is a can move cell
    // what == 2 : it is a to move cell
    void pressed(int player, int i, int j, int what);

public slots:
};

#endif // GRID_H
