#pragma once

#include "pacman.h"
#include "ghostTypes.h"

#include <QGridLayout>
#include <QTimer>
using namespace std;

class Map : public QWidget{
    protected:
        ifstream mapFile;
        Red *blinky;
        Orange *clyde;
        Cyan *inky;
        Pink *pinky;
        QGridLayout *grid;
        virtual void connectObjects();
        virtual void createMap(vector<pair<int, int>>&);
        virtual void addBigPoints(vector<pair<int, int>>&);
        virtual void addSprites();
        virtual void setupMap();
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
        virtual void endGame();
    public:
        QTimer *gameTimer;
        Pacman *player;
        Map(QWidget *parent = nullptr);
};