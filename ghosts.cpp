#include "ghosts.h"

#include <QPainter>

Ghost::Ghost(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Sprite(startingPoint, parent){
    this->player = player;
}

void Ghost::paintEvent(QPaintEvent* /*event*/){
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(color));
    painter.drawPie(QRect(0,0,40,40),0,180*16);
    painter.drawRect(0, 20, 40, 20);
    painter.setBrush(QColor("#ffffff"));
    painter.drawPie(QRect(10,10,8,10),0,360*16);
    painter.drawPie(QRect(22,10,8,10),0,360*16);
    painter.setBrush(QColor("#000000"));
    painter.drawPie(QRect(12,12,5,5),0,360*16);
    painter.drawPie(QRect(24,12,5,5),0,360*16);
}

void Ghost::getVunerable(){
    color = QColor("#111155");
    update();
}

void Ghost::moveSprite(){
    Sprite::moveSprite();
}

Pink::Pink(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor("#ff6688");
    orginalColor = color;
}

Orange::Orange(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor("#ff8800");
    orginalColor = color;
}

Cyan::Cyan(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor("#00ffff");
    orginalColor = color;
}

Red::Red(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor("#ff0000");
    orginalColor = color;
}