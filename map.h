#pragma once

#include "pacman.h"

#include <QTimer>
#include <QWidget>
using namespace std;

class Map : public QWidget{

    private:
        ifstream mapFile;
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
    public:
        QTimer *gameTimer;
        Pacman *player;
        Map(QWidget *parent = 0);
};