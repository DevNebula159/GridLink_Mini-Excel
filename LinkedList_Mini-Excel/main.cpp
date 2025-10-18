#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Network.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;
using namespace sf;

#define vu sf::Vector2u
#define v sf::Vector2f
#define c sf::Color
#define f (float)

Texture bgTexture;
Sprite bgSprite;

float width = 1920.f, height = 1080.f;
int cnt = 0, sum = 0;
float avg = 0;

Text cntText, sumText, avgText;

Font font;
Clock clk;

c outlineColor = c(200, 200, 200);
c brightColor = c(0, 102, 102);

void setTextInMiddle(Text& text) {
	FloatRect bounds = text.getLocalBounds();
	text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
}

class Node {
protected:
	Node* up, * down, * left, * right;
	string data;

	RectangleShape box;
	Text text;
public:
	Node(const string& data = "", v pos = v(), Node* left = nullptr, Node* right = nullptr, Node* up = nullptr, Node* down = nullptr) {
		this->left = left;
		this->right = right;
		this->up = up;
		this->down = down;
		this->data = data;

		box.setFillColor(c::Transparent);
		box.setOutlineColor(outlineColor);
		box.setOutlineThickness(-3.f);
		box.setSize(v(220.f, 150.f));
		box.setPosition(pos);

		text.setFillColor(outlineColor);
		text.setCharacterSize(30);
		text.setFont(font);
		text.setString(data);
		setTextInMiddle(text);
		text.setPosition(v(pos.x + (box.getGlobalBounds().getSize().x / 2.f), pos.y + (box.getGlobalBounds().getSize().y / 2.f)));
		text.setPosition(v(text.getPosition().x - 90.f, text.getPosition().y));
	}
	Node* getLeft() {
		return this->left;
	}
	Node* getRight() {
		return this->right;
	}
	Node* getUp() {
		return this->up;
	}
	Node* getDown() {
		return this->down;
	}
	string get() {
		return data;
	}

	void setLeft(Node* left) {
		this->left = left;
	}
	void setRight(Node* right) {
		this->right = right;
	}
	void setUp(Node* up) {
		this->up = up;
	}
	void setDown(Node* down) {
		this->down = down;
	}
	void set(const string& data) {
		this->data = data;
		text.setString(data);
	}

	bool isInteger() {
		int n = data.size();
		for (int i = 0; i < n; i++) {
			if (data[i] < '0' || data[i] > '9') return false;
		}
		return true;
	}

	void setPos(v pos) {
		box.setPosition(pos);
		text.setPosition(v(pos.x + (box.getGlobalBounds().getSize().x / 2.f), pos.y + (box.getGlobalBounds().getSize().y / 2.f)));
	}

	void setTextPos(v pos) {
		text.setPosition(pos);
	}

	Text getText() {
		return this->text;
	}
	v getPos() {
		return box.getPosition();
	}
	v getSize() {
		return box.getSize();
	}
	void makeBright() {
		box.setOutlineColor(brightColor);
		box.setFillColor(c(250, 250, 250, 40));

		text.setFillColor(brightColor);
	}
	void makeDull() {
		box.setOutlineColor(outlineColor);
		box.setFillColor(c::Transparent);
		text.setFillColor(outlineColor);
	}
	void display(RenderWindow& window) {
		window.draw(box);
		window.draw(text);
	}
};

class Grid {
	Node* head, * current;
	void moveTo(Node* temp) {
		current->makeDull();
		if (temp) current = temp;
		current->makeBright();
	}
public:
	Grid(int n) {
		head = current = new Node("_", v(30.f, 30.f));
		for (int i = 0; i < n; i++) {
			while (current->getLeft()) current = current->getLeft();
			if (i) {
				Node* newNode = new Node("_", v(current->getPos().x, current->getPos().y + current->getSize().y + 5.f));
				current->setDown(newNode);
				newNode->setUp(current);
				current = newNode;
			}

			for (int i = 1; i < n; i++) {
				Node* temp = new Node("_", v(current->getPos().x + current->getSize().x + 5.f, current->getPos().y));
				Node* up = current->getUp();
				if (up) up = up->getRight();
				current->setRight(temp);
				if (up) up->setDown(temp);

				temp->setLeft(current);
				temp->setUp(up);
				current = temp;
			}
		}
		moveTo(head);
	}
	~Grid() {
		current = head;
		while (current) {
			Node* tempDown = current->getDown();
			while (current) {
				Node* temp = current->getRight();
				delete current;
				current = temp;
			}
			current = tempDown;
		}
	}
	void moveUp() {
		moveTo(current->getUp());
	}
	void moveDown() {
		moveTo(current->getDown());
	}
	void moveRight() {
		moveTo(current->getRight());
	}
	void moveLeft() {
		moveTo(current->getLeft());
	}
	void set(const string& data) {
		this->current->set(data);
	}

	void takeInput(RenderWindow& window, Event& event) {
		Text tempText = current->getText();
		string data = current->get();
		bool done = false;
		while (!done) {
			float time = clk.getElapsedTime().asSeconds();
			while (window.pollEvent(event)) {
				if (event.type == Event::TextEntered && time >= 0.1f) {
					unsigned char code = event.text.unicode;
					if (code >= 32 && code != 127 && data.size() < 10) data += code;
					clk.restart();
				}
				else if (event.type == Event::KeyPressed && time >= 0.1f) {
					clk.restart();
					if (event.key.code == Keyboard::BackSpace) if (!data.empty()) data.pop_back();
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Enter) && time >= 0.2f) {
				done = true;
				clk.restart();
			}
			else if (Keyboard::isKeyPressed(Keyboard::Escape) && time >= 0.2f) {
				window.close();
				break;
			}

			tempText.setString(data);
			window.clear();
			window.draw(bgSprite);
			window.draw(tempText);
			window.draw(cntText);
			window.draw(sumText);
			window.draw(avgText);

			Node* temp = head;
			while (temp) {
				Node* down = temp->getDown();
				while (temp) {
					if (temp != current) temp->display(window);
					temp = temp->getRight();
				}
				temp = down;
			}

			window.display();
		}
		current->set(data);
	}

	bool isInteger() {
		return current->isInteger();
	}

	int toInteger() {
		return stoi(current->get());
	}

	void display(RenderWindow& window) {
		Node* temp = head;
		while (temp) {
			Node* down = temp->getDown();
			while (temp) {
				temp->display(window);
				temp = temp->getRight();
			}
			temp = down;
		}
	}
};

void keyInput(Grid& grid, RenderWindow& window) {
	float time = clk.getElapsedTime().asSeconds();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && time >= 0.2f) {
		window.close();
		clk.restart();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && time >= 0.2f) {
		grid.moveLeft();
		clk.restart();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && time >= 0.2f) {
		grid.moveRight();
		clk.restart();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && time >= 0.2f) {
		grid.moveUp();
		clk.restart();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && time >= 0.2f) {
		grid.moveDown();
		clk.restart();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && time >= 0.2f) {
		clk.restart();
		if (grid.isInteger()) {
			cnt++;
			sum += grid.toInteger();
			avg = f sum / cnt;

			cntText.setString("COUNT: " + to_string(cnt));
			sumText.setString("SUM: " + to_string(sum));
			avgText.setString("AVERAGE: " + to_string(avg));
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete) && time >= 0.2f) {
		clk.restart();
		cnt = 0;
		sum = 0;
		avg = 0;

		cntText.setString("COUNT: " + to_string(cnt));
		sumText.setString("SUM: " + to_string(sum));
		avgText.setString("AVERAGE: " + to_string(avg));
	}
}

void setTextDetails(Text& text) {
	text.setFont(font);
	text.setFillColor(outlineColor);
	text.setCharacterSize(50);
}

void loadData() {
	clk.restart();
	font.loadFromFile("fonts/round.otf");
	bgTexture.loadFromFile("bg/bg.png");
	bgSprite.setTexture(bgTexture);

	setTextDetails(sumText);
	setTextDetails(cntText);
	setTextDetails(avgText);

	cntText.setString("COUNT: " + to_string(cnt));
	cntText.setPosition(v(width / 2.f + 300.f, height / 2.f - 300.f));

	sumText.setString("SUM: " + to_string(sum));
	sumText.setPosition(v(width / 2.f + 300.f, height / 2.f - 200.f));

	avgText.setString("AVERAGE: " + to_string(avg));
	avgText.setPosition(v(width / 2.f + 300.f, height / 2.f - 100.f));

}

void drawEverything(Grid& grid, RenderWindow& window) {
	window.clear();
	window.draw(bgSprite);
	window.draw(cntText);
	window.draw(sumText);
	window.draw(avgText);
	grid.display(window);
}

int main() {
	int n = 5;

	Grid grid(n);

	RenderWindow window(VideoMode(width, height), "LinkedList_Mini-Excel", sf::Style::Fullscreen);
	window.setFramerateLimit(60);

	loadData();

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter && clk.getElapsedTime().asSeconds() >= 0.2f) {
				clk.restart();
				grid.takeInput(window, event);
			}
		}
		keyInput(grid, window);
		
		drawEverything(grid, window);
		window.display();
	}

	return 0;
}

// Escape: Close the application
// Enter: To Enter the value of a cell(and then press enter after completing value)