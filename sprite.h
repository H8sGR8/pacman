#pragma once

#include <QWidget>
#include <fstream>
using namespace std;

class Sprite : public QWidget{
    protected:
        ifstream map;
        int simpleMap[31][28];
        typedef struct{
            int x;
            int y;
        } position;
        position previousPosition;
        int nextDirection; 
        QColor color;
        bool start;
        int step;
        bool play;
        virtual void paintEvent(QPaintEvent* /*event*/);
        virtual void stopSprite();
        virtual void makeStep();
        virtual void changeCords();
        virtual void changeDirection();
        virtual void teleport();
        virtual int getTileInFront(int);
    public slots:
        void moveSprite();
    public:
        position cords;
        int currentDirection;
        virtual int getXOfNTileInFront(int, int);
        virtual int getYOfNTileInFront(int, int);
        virtual void setStartPos(int, int);
        Sprite(pair<int, int>, QWidget* parent = nullptr);
};