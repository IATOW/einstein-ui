#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QWidget>

#include "chessgrid.h"

class Board : public QWidget
{
    Q_OBJECT

private:
    static const int GRID = 100;

    ChessGrid * grid[5][5];


protected:
    void paintEvent(QPaintEvent * event) override;
    //void enterEvent(QEvent * event) override;
    //void leaveEvent(QEvent * event) override;
    //void mousePressEvent(QMouseEvent * event) override;

public:
    explicit Board(QWidget *parent = nullptr);

    void updateGrid(int chess[6][6]);
    void setGridState(int i, int j, int state);
    ChessGrid * getGrid(int i, int j) { return grid[i][j]; }
    void clearState(int state);

signals:

public slots:
};

#endif // BOARD_H
