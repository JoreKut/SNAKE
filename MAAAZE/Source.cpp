#include <SFML/Graphics.hpp>
#include <iostream>
#include <deque>
#include <string>


int WIDTH = 1200;
int HEIGHT = 800;
int fraction_SIZE = 40;

bool GAME = true;
bool WIN = false;

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SNAKE");

sf::Color SNAKE_COLOR = sf::Color(0, 255, 0);
sf::Color FOOD_COLOR = sf::Color(255, 0, 0);

enum class Direction
{
	UP,
	DOWN,
	RIGHT,
	LEFT
};

class Fraction {
public:
	sf::RectangleShape fraction;
	int x, y;
	Fraction() {
		x = 0;
		y = 0;
		fraction.setPosition(sf::Vector2f(x, y));
		fraction.setFillColor(SNAKE_COLOR);
		fraction.setSize(sf::Vector2f(fraction_SIZE, fraction_SIZE));
	}
	
	void setPos(int x, int y) {
		this->x = x;
		this->y = y;
		move(0, 0);
	}

	void move(int dx, int dy) {
		this->x += dx;
		this->y += dy;

		if (y >= HEIGHT) y -= HEIGHT;
		else if (y < 0) y += HEIGHT;

		if (x >= WIDTH)  x -= WIDTH;
		else if (x < 0) x += WIDTH;

		fraction.setPosition(sf::Vector2f(x, y));
	}
};

class Food
{
public:
	int x;
	int y;
	sf::RectangleShape food;
	Food() {
		x = 0;
		y = 0;
		food.setFillColor(FOOD_COLOR);
		food.setSize(sf::Vector2f(fraction_SIZE, fraction_SIZE));
	};
	void GeneratePos(std::deque<Fraction> arr) {
		srand(time(0));
		bool correct = false;

		do {

			x = rand() % (int)(WIDTH / fraction_SIZE) * fraction_SIZE;
			y = rand() % (int)(HEIGHT / fraction_SIZE) * fraction_SIZE;
			correct = true;

			for (int i = 0; i < arr.size(); i++) 
				if (x == arr[i].x && y == arr[i].y) {
					correct = false;
					break;
				}

		} while (!correct);


		food.setPosition(sf::Vector2f(x, y));

	}
};

class Snake {
private:
	int start_fraction_count = 2;
	int startX = fraction_SIZE * 9;
	int startY = fraction_SIZE * 6;
public:
	
	std::deque<Fraction> snake;

	Snake() {

		for (int i = 0; i < start_fraction_count; i++) {
			add(Direction::RIGHT);
		}
	
	}

	void ReStart() {

		snake.clear(); 

		for (int i = 0; i < start_fraction_count; i++) {
			add(Direction::RIGHT);
		}

		Display();
	}

	void add(Direction currentDirection) {

		Fraction fr = Fraction();

		switch (currentDirection)
		{
		case Direction::UP:
			if (snake.empty()) {
				fr.setPos(startX, startY);
			}
			else 
				fr.setPos(snake.back().x, snake.back().y + fraction_SIZE);
			snake.push_back(fr);
			break;
		case Direction::DOWN:
			if (snake.empty()) {
				fr.setPos(startX, startY);
			}
			else
				fr.setPos(snake.back().x, snake.back().y - fraction_SIZE);
			snake.push_back(fr);
			break;
		case Direction::RIGHT:
			if (snake.empty()) {
				fr.setPos(startX, startY);
			}
			else
				fr.setPos(snake.back().x - fraction_SIZE, snake.back().y);
			snake.push_back(fr);
			break;
		case Direction::LEFT:
			if (snake.empty()) {
				fr.setPos(startX, startY);
			}
			else
				fr.setPos(snake.back().x + fraction_SIZE, snake.back().y);
			snake.push_back(fr);
			break;
		default:
			break;
		}

	}

	bool eat(Food food) {

		if (snake[0].x == food.x && snake[0].y == food.y)
			return true;

		return false;
	}
	bool isCollision() {

		for (int i = 1; i < snake.size(); i++) {
			if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
				return true;
		}
		return false;
	}

	#pragma region movement
	void UP() {

		Fraction last = snake.back();
		snake.pop_back();
		last.setPos(snake.front().x, snake.front().y - fraction_SIZE);
		snake.push_front(last);

		Display();
	}void DOWN() {
		Fraction last = snake.back();
		snake.pop_back();
		last.setPos(snake.front().x, snake.front().y + fraction_SIZE);
		snake.push_front(last);

		Display();
	}void LEFT() {
		Fraction last = snake.back();
		snake.pop_back();
		last.setPos(snake.front().x - fraction_SIZE, snake.front().y);
		snake.push_front(last);

		Display();
	}void RIGHT() {
		Fraction last = snake.back();
		snake.pop_back();
		last.setPos(snake.front().x + fraction_SIZE, snake.front().y);
		snake.push_front(last);
		Display();
	}
#pragma endregion

	void Display() {
		window.clear();
		for (Fraction fr : snake) {
			window.draw(fr.fraction);
		}
	}
};

void check(bool &UP, bool& DOWN, bool& LEFT, bool& RIGHT) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !DOWN) {
		UP = true;
		DOWN = false;
		RIGHT = false;
		LEFT = false;
	}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&!UP) {
		UP = false;
		DOWN = true;
		RIGHT = false;
		LEFT = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !LEFT) {
		UP = false;
		DOWN = false;
		RIGHT = true;
		LEFT = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&!RIGHT) {
		UP = false;
		DOWN = false;
		RIGHT = false;
		LEFT = true;
	}
}

int main()
{
	window.setFramerateLimit(15);

	sf::Event event;

	Snake sn = Snake();
	sn.Display();

	bool UP = false;
	bool DOWN = false;
	bool LEFT = false;
	bool RIGHT = true;
	
	#pragma region GAMEOVERtxt

	sf::Text text;
	sf::Font font;
	font.loadFromFile("Dosis-ExtraLight.ttf");

	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(72);
	text.setPosition(sf::Vector2f(WIDTH / 2 - 5 * 72 / 2, HEIGHT / 2 - 72));
#pragma endregion
#pragma region GAMEOVERtxt

	sf::Text score;
	score.setFont(font);
	score.setString(std::to_string(sn.snake.size()));
	score.setFillColor(sf::Color::White);
	score.setCharacterSize(30);
	score.setPosition(sf::Vector2f(5, 5));
#pragma endregion

	Food food = Food();
	food.GeneratePos(sn.snake);


	while (window.isOpen())
	{

		while (GAME)
		{
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed) window.close();

				check(UP, DOWN, LEFT, RIGHT);
			}

			if (UP){
				sn.UP();
				if (sn.eat(food)) {
					sn.add(Direction::UP);
					food.GeneratePos(sn.snake);
					score.setString(std::to_string(sn.snake.size()));
				}
				check(UP, DOWN, LEFT, RIGHT);
			}
			if (DOWN){
				sn.DOWN();
				if (sn.eat(food)) {
					sn.add(Direction::DOWN);
					food.GeneratePos(sn.snake);
					score.setString(std::to_string(sn.snake.size()));
				}
				check(UP, DOWN, LEFT, RIGHT);
			}
			if (RIGHT){
				sn.RIGHT();
				if (sn.eat(food)) {
					sn.add(Direction::RIGHT);
					food.GeneratePos(sn.snake);
					score.setString(std::to_string(sn.snake.size()));
				}
				check(UP, DOWN, LEFT, RIGHT);
			}
			if (LEFT){
				sn.LEFT();
				if (sn.eat(food)) {
					sn.add(Direction::LEFT);
					food.GeneratePos(sn.snake);
					score.setString(std::to_string(sn.snake.size()));
				}
				check(UP, DOWN, LEFT, RIGHT);
			}


			if (sn.isCollision())
				GAME = false;
			if (sn.snake.size() == (int)(WIDTH * HEIGHT / (fraction_SIZE * fraction_SIZE)))
				WIN = true;

			window.draw(food.food);
			window.draw(score);
			window.display();

			if (!GAME) {
				sn.Display();
				text.setString("GAME OVER\nYOUR SCORE : "+std::to_string(sn.snake.size()));
				window.draw(text);
				window.display();
			}
			if (WIN) {
				sn.Display();
				text.setString("YOU'RE WIN !");
				window.draw(text);
				window.display();
			}
		}
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				GAME = true;
				WIN = false;
				UP = false;
				DOWN = false;
				LEFT = false;
				RIGHT = false;
				sn.ReStart();
				food.GeneratePos(sn.snake);
				window.display();
			}
		}

	}
	return 0;
}