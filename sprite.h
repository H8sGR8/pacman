#pragma once

#include <QWidget>
#include <fstream>
using namespace std;

#define NO_DIR 0
#define UP 1
#define LEFT 2
#define RIGHT 3
#define DOWN 4

#define STEP 5

class Sprite : public QWidget{
    protected:
        ifstream map;
        int simpleMap[31][28];
        typedef struct{
            int x;
            int y;
        } position;
        position cords;
        position previousPosition;
        int currentDirection = NO_DIR;
        int nextDirection = NO_DIR; 
        QColor color;
        bool start = true;
        pair<int, int> startingPoint;
        int step;
        virtual void paintEvent(QPaintEvent* /*event*/);
        virtual void setStartPos(int, int);
        virtual void stopSprite();
        virtual void makeStep();
        virtual void changeCords();
        virtual void changeDirection();
    public slots:
        void moveSprite();
    public:
        Sprite(pair<int, int>, QWidget* parent = 0);
};