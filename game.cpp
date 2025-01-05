#include "map.h"
#include "pacman.h"

#include <QApplication>
#include <QGridLayout>
#include <QPushButton>
#include <QDesktopWidget>
#include <QPalette>
#include <QLabel>
#include <string>

class Game : public QWidget{

	private:
		QPushButton *quit;
		QLabel *points;
		void updatePoints(int);
	public:
		Map *map;
		Game(QWidget *parent = nullptr);
};

void Game::updatePoints(int newPoints){
	points->setText(QString::fromStdString("Points: " + to_string(newPoints)));
}

Game::Game(QWidget *parent) : QWidget(parent)
{
	setPalette(QPalette(QColor("#2f2f2f")));
	resize(1200, 1000);
	move(QApplication::desktop()->geometry().center() - rect().center());
	map = new Map();
	quit = new QPushButton("QUIT");
	connect(quit, &QPushButton::clicked, qApp, &QApplication::quit);
	points = new QLabel;
	connect(map->player, &Pacman::pointsChanged, this, &Game::updatePoints);
	points->setText("Points: 0");
	QGridLayout	*grid = new QGridLayout;
	grid->addWidget(map, 1, 1, 2, 2);
	grid->addWidget(points, 1, 3);
	grid->addWidget(quit, 2, 3);
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
