#include <QApplication>
#include <QGridLayout>
#include <QPushButton>
#include <QDesktopWidget>
#include <QPalette>

#include "map.h"
#include "pacman.h"

class Game : public QWidget
{
public:
	Map *map;
	Game(QWidget *parent = 0);
};

Game::Game(QWidget *parent) : QWidget(parent)
{
	QPushButton *quit = new QPushButton("QUIT");
	connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
	setPalette(QPalette(QColor("#2f2f2f")));
	resize(1200, 1000);
	move(QApplication::desktop()->geometry().center() - rect().center());
	map = new Map();
	QGridLayout	*grid = new QGridLayout;
	grid->addWidget(map, 0, 0, 1, 1);
	grid->addWidget(quit, 0, 2);
	setLayout(grid);
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	Game game;
	game.map->player->setFocus();
	game.show();
	return app.exec();
}
