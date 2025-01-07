#pragma once

#include <QWidget>
#include <fstream>
using namespace std;

#define NO_DIR 0
#define UP 1
#define LEFT 2
#define DOWN 3
#define RIGHT 4

#define STEP 5

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
    public slots:
        void moveSprite();
    public:
        position cords;
        int currentDirection;
        virtual void setStartPos(int, int);
        Sprite(pair<int, int>, QWidget* parent = nullptr);
};