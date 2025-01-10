#pragma once

#include "pacman.h"

#include <QTimer>
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
                Block(QWidget *parent = nullptr);
        };
        class Wall : public Block{
            public:
                Wall(QWidget *parent = nullptr);
        };
        class Path : public Block{
            public:
                Path(QWidget *parent = nullptr);
        };
        class Blockade : public Block{
            public:
                Blockade(QWidget *parent = nullptr);
        };
    public slots:
        void endGame();
    public:
        QTimer *gameTimer;
        Pacman *player;
        Map(QWidget *parent = nullptr);
};