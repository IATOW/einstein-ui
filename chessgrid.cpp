#include <QPainter>

#include "chessgrid.h"

ChessGrid::ChessGrid(QWidget *parent) : QWidget(parent)
{
    //move(100, 100);
}

void ChessGrid::enterEvent(QEvent * event)
{
    if(state == 1)
        state = 2;

    if(state == 4)
        state = 5;

    update();
}

void ChessGrid::leaveEvent(QEvent * event)
{
    if(state == 2)
        state = 1;

    if(state == 5)
        state = 4;

    update();
}

void ChessGrid::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRect(1, 1, 98, 98);



    if(state == 1)
    {
        // cell can move
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(237, 208, 190));
        painter.drawRect(1, 1, 98, 98);
    }

    if(state == 2)
    {
        // cell can move, hover
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(29, 176, 184, 128));
        painter.drawRect(1, 1, 98, 98);
    }

    if(state == 3)
    {
        // cell can move, AI
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(224, 38, 22, 128));
        painter.drawRect(1, 1, 98, 98);
    }

    if(state == 4)
    {
        // cell to move
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(208, 233, 255, 200));
        painter.drawRect(20, 20, 60, 60);
    }

    if(state == 5)
    {
        // cell to move, hover
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(114, 109, 209, 128));
        painter.drawRect(20, 20, 60, 60);
    }

    if(number > 0)
    {
        int side = number % 10;
        if(side == 1)
        {
            painter.setBrush(QColor(224, 54, 54, 128));
        }
        else if(side == 2)
        {
            painter.setBrush(QColor(37, 198, 252, 128));
        }
        painter.drawRect(1, 1, 98, 98);

        QFont font;
        font.setPointSize(17);
        font.setFamily("Consolas");
        painter.setFont(font);
        QTextOption option(Qt::AlignCenter);
        painter.setPen(Qt::white);
        painter.drawText(QRectF(1,1,98,98), QString("%1").arg(number / 10), option);
    }


}

void ChessGrid::mousePressEvent(QMouseEvent * event)
{
    if(state == 1 || state == 2)
    {
        emit pressed(number % 10, mapx, mapy, 1);
    }

    if(state == 4 || state == 5)
    {
        emit pressed(number % 10, mapx, mapy, 2);
    }

    if(state == 0)
    {
        emit pressed(-1, -1, -1, 0);
    }
}
