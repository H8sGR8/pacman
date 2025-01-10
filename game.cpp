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
		QLabel *health;
		QLabel *gameOver;
		void updatePoints(int);
		void updateHealth(int);
	public:
		Map *map;
		Game(QWidget *parent = nullptr);
};

void Game::updatePoints(int newPoints){
	points->setText(QString::fromStdString("Points: " + to_string(newPoints)));
}

void Game::updateHealth(int newHealth){
	health->setText(QString::fromStdString("Health: " + to_string(newHealth)));
	if(newHealth == 0) gameOver->setVisible(true);
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
	health = new QLabel;
	connect(map->player, &Pacman::healthChanged, this, &Game::updateHealth);
	gameOver = new QLabel;
	points->setText("Points: 0");
	health->setText("Health: 3");
	gameOver->setText("Game Over :(");
	QGridLayout	*grid = new QGridLayout;
	grid->addWidget(map, 1, 1, 10, 3);
	grid->addWidget(points, 1, 4);
	grid->addWidget(health, 2, 4);
	grid->addWidget(quit, 5, 4);
	grid->addWidget(gameOver, 5, 2);
	setLayout(grid);
	gameOver->setVisible(false);
	gameOver->setStyleSheet("font: 40pt; color: red;");
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	Game game;
	game.map->player->setFocus();
	game.show();
	return app.exec();
}
