#include "points.h"

#include <QPainter>

Point::Point(QWidget *parent): QWidget(parent){
    colected = false;
}

void Point::paintEvent(QPaintEvent* /*event*/){
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(color));
    painter.drawPie(QRect(0, 0, diameter, diameter), 0, 360*16);
}

int Point::getPoints(){
    return pointValue;
}

int Point::getEaten(){
    setUpdatesEnabled(false);
    return getPoints();
}

BigPoint::BigPoint(QWidget *parent): Point(parent){
    color = QColor("#aaaa00");
    diameter = 30;
    pointValue = 50;
}

int BigPoint::getEaten(){
    emit eaten();
    return Point::getEaten();
}

SmallPoint::SmallPoint(QWidget *parent): Point(parent){
    color = QColor("#777722");
    diameter = 10;
    pointValue = 10;
}