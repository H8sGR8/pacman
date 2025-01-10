#include "points.h"
#include "defines.h"

#include <QPainter>

Point::Point(QWidget *parent): QWidget(parent){
    colected = false;
}

void Point::paintEvent(QPaintEvent* /*event*/){
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(color));
    painter.drawPie(QRect(0, 0, diameter, diameter), 0, FULL_CIRCLE * ANGLE_MULTIPLIER);
}

int Point::getPoints(){
    return pointValue;
}

int Point::getEaten(){
    setUpdatesEnabled(false);
    return getPoints();
}

BigPoint::BigPoint(QWidget *parent): Point(parent){
    color = QColor(LIGHT_YELLOWISH);
    diameter = BIG_POINT_DIAMETER;
    pointValue = BIG_POINT_VALUE;
}

int BigPoint::getEaten(){
    emit eaten();
    return Point::getEaten();
}

SmallPoint::SmallPoint(QWidget *parent): Point(parent){
    color = QColor(YELLOWISH);
    diameter = SMALL_POINT_DIAMETER;
    pointValue = SMALL_POINT_VALUE;
}