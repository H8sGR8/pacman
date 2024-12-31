#pragma once

#include <QWidget>

class Point  : public QWidget{

    protected:
        int pointValue;
        int diameter;
        QColor color;
        virtual void paintEvent(QPaintEvent*);
        virtual int getPoints();
    public:
        virtual int getEaten();
        Point(QWidget *parent = 0);
};

class SmallPoint : public Point{
    public:
        SmallPoint(QWidget *parent = 0);
};

class BigPoint : public Point{
    public:
        BigPoint(QWidget *parent = 0);
};