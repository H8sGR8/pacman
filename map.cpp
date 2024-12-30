#include "map.h"

#include <QColor>
#include <QPainter>
#include <QGridLayout>

Map::Block::Block(QWidget *parent) : QWidget(parent){
    setMaximumSize(30, 30);
    setMinimumSize(30, 30);
}

void Map::Block::paintEvent(QPaintEvent* /*event*/){
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(color));
    painter.drawRect(0, 0, 30, 30);
}

Map::Wall::Wall(QWidget *parent) : Block(parent){
    color = QColor("#111155");
}

Map::Path::Path(QWidget *parent) : Block(parent){
    color = QColor("#0d0d1d");
}

Map::Map(QWidget *parent) : QWidget(parent){
    setMinimumSize(850, 940);
    setMaximumSize(850, 940);
    player = new Pacman();
    QGridLayout *grid = new QGridLayout;
    for(int i = 0; i < 31; i++){
        for(int j = 0; j < 28; j++){
            if(maptTemplate[i][j] == 1) grid->addWidget(new Wall, i, j);
            else grid->addWidget(new Path, i, j);
        }
    }
    grid->addWidget(player, 1, 1);
    grid->setHorizontalSpacing(0);
    grid->setVerticalSpacing(0);
    setLayout(grid);
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, player, &Pacman::moveSprite);
    gameTimer->start(10);
}