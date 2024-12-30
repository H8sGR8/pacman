#include "pacman.h"

#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

Pacman::Pacman(QWidget* parent) : Sprite(parent){
    color = QColor("#ffff00");
}

void Pacman::paintEvent(QPaintEvent* /*event*/){
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(color));
    painter.drawPie(QRect(0,0,50,50),0,360*16);
}

void Pacman::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Up){
        if(currentDirection == NO_DIR) currentDirection = UP;
        if(currentDirection == DOWN) nextDirection = NO_DIR;
        else nextDirection = UP;
    }
    if(event->key() == Qt::Key_Left){
        if(currentDirection == NO_DIR) currentDirection = LEFT;
        if(currentDirection == RIGHT) nextDirection = NO_DIR;
        else nextDirection = LEFT;
    }
    if(event->key() == Qt::Key_Down){
        if(currentDirection == NO_DIR) currentDirection = DOWN;
        if(currentDirection == UP) nextDirection = NO_DIR;
        else nextDirection = DOWN;
    }
    if(event->key() == Qt::Key_Right){
        if(currentDirection == NO_DIR) currentDirection = RIGHT;
        if(currentDirection == LEFT) nextDirection = NO_DIR;
        else nextDirection = RIGHT;
    }
}