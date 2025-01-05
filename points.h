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
        Point(QWidget *parent = nullptr);
};

class SmallPoint : public Point{
    public:
        SmallPoint(QWidget *parent = nullptr);
};

class BigPoint : public Point{

    Q_OBJECT

    signals:
        void eaten();
    public:
        virtual int getEaten();
        BigPoint(QWidget *parent = nullptr);
};