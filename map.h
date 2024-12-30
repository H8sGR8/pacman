#pragma once

#include <QTimer>
#include <QWidget>
#include "pacman.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

class Map : public QWidget{

    private:
        ifstream mapFile;
        int maptTemplate[93][84];
    protected:
        class Block : public QWidget{
            protected:
                QColor color;
                virtual void paintEvent(QPaintEvent*);
            public:
                Block(QWidget *parent = 0);
        };
        class Wall : public Block{
            public:
                Wall(QWidget *parent = 0);
        };
        class Path : public Block{
            public:
                Path(QWidget *parent = 0);
        };
        class Point  : public QWidget{
            protected:
                int diameter;
                QColor color;
                virtual void paintEvent(QPaintEvent*);
            public:
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
    public:
        QTimer *gameTimer;
        Pacman *player;
        Map(QWidget *parent = 0);
};