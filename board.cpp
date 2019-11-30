#include <QPainter>

#include "chessgrid.h"
#include "board.h"

Board::Board(QWidget *parent) : QWidget(parent)
{
    this->resize(5 * GRID, 5 * GRID);

    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            grid[i][j] = new ChessGrid(this);
            grid[i][j]->move(j * GRID, i * GRID);
            grid[i][j]->resize(GRID, GRID);
            grid[i][j]->mapx = i + 1;
            grid[i][j]->mapy = j + 1;
        }
    }
}

void Board::updateGrid(int chess[6][6])
{
    for(int i = 1; i <= 5; i++)
    {
        for(int j = 1; j <= 5; j++)
        {
            grid[i - 1][j - 1]->setNumber(chess[i][j]);
        }
    }

    update();
}

void Board::paintEvent(QPaintEvent * event)
{
    static const QColor color(237, 208, 190);

    QPainter painter(this);

    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, 500, 500);
}

void Board::setGridState(int i, int j, int state)
{
    grid[i][j]->setState(state);
}

void Board::clearState(int state)
{
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            if(grid[i][j]->getState() == state)
                grid[i][j]->setState(0);
        }
    }
}
