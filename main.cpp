/*
 * main.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: Tyler
 */

#include <iostream>
#include <SFML/Window.hpp>
#include <GL/gl.h>
#include <cmath>
#include <glm/glm.hpp>

int hexWidth = 10;
int mod = 7;

long factorial(int n) {
	long num = 1;
	for (int i = 0; i < n; ++i) {
		num *= i + 1;
	}
	return num;
}

long nChooseK(int n, int k) {
	return factorial(n) / (factorial(k) * factorial(n - k));
}

void drawHexagon(int x, int y) {
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 6; i++) {
		glVertex2f(x, y);
		glVertex2f((sin(i / 6.0 * 2 * M_PI) * hexWidth) + x, (cos(i / 6.0 * 2 * M_PI) * hexWidth) + y);
		glVertex2f((sin((i + 1) / 6.0 * 2 * M_PI) * hexWidth) + x, (cos((i + 1) / 6.0 * 2 * M_PI) * hexWidth) + y);
	}
	glEnd();
}

int main() {
	sf::Window window;
	window.create(sf::VideoMode(1280, 720, 32), "Pascal's Triangle");
	window.setFramerateLimit(20);

	glClearColor(0, 0, 0, 1);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, 1280, 720);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1280, 0, 720, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glm::vec3 colors[mod];
	srand(time(0));
	for(int i = 0; i < mod; ++i){
		colors[i] = glm::vec3(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
	}

	sf::Event event;
	bool running = true;
	float xOffset = 0, yOffset = 0;
	int lastMouseX = sf::Mouse::getPosition().x, lastMouseY = sf::Mouse::getPosition().y;
	float zoom = 1;
	int numRows = 294;

	bool needsUpdate = true;
	while (running) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				running = false;
			} else if (event.type == sf::Event::MouseMoved) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					xOffset += (event.mouseMove.x - lastMouseX);
					yOffset -= (event.mouseMove.y - lastMouseY);
					needsUpdate = true;
				}
				lastMouseX = event.mouseMove.x;
				lastMouseY = event.mouseMove.y;
			} else if (event.type == sf::Event::MouseWheelMoved) {
				zoom += event.mouseWheel.delta * 0.1f;
				zoom = (zoom <= 0 ? 0.2f : (zoom > 3 ? 3 : zoom));
				needsUpdate = true;
			} else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Up) {
					if (numRows < 350) {
						numRows++;
						needsUpdate = true;
					}
				} else if (event.key.code == sf::Keyboard::Down) {
					if (numRows > 1) {
						numRows--;
						needsUpdate = true;
					}
				} else if (event.key.code == sf::Keyboard::Right) {
					if (mod < 15) {
						mod++;
						needsUpdate = true;
					}
				} else if (event.key.code == sf::Keyboard::Left) {
					if (mod > 2) {
						mod--;
						needsUpdate = true;
					}
				}
			}
		}

		if (needsUpdate) {
			glClear(GL_COLOR_BUFFER_BIT);

			glLoadIdentity();
			glScalef(zoom, zoom, 0);
			glTranslatef(xOffset / zoom, yOffset / zoom, 0);

			int array[numRows + 2];
			array[0] = 0;
			array[1] = 1;
			array[2] = 0;
			for (int i = 0; i < numRows; ++i) {
				int rowNum = i + 1;
				if (i != 0) {
					for (int j = rowNum; j > 0; --j) {
						array[j] += array[j - 1];
						array[j] %= mod;
					}
					array[rowNum + 1] = 0;
				}
				for (int j = 0; j < rowNum; ++j) {
//					glColor3f(array[j + 1] / (float) (mod - 1), array[j + 1] / (float) (mod - 1), array[j + 1] / (float) (mod - 1));
					glColor3f(colors[array[j + 1]].r, colors[array[j + 1]].g, colors[array[j + 1]].b);
					float x = (1280 / 2) - (j - (rowNum / 2.f) + 0.5) * 2 * 0.866025405 * hexWidth;
					float y = 600 - (hexWidth * 1.5 * i);
					if ((x * zoom + xOffset + hexWidth < 1280) && (x * zoom + xOffset - hexWidth > 0) && (y * zoom + yOffset + hexWidth < 720) && (y * zoom + yOffset - hexWidth > 0)) {
						drawHexagon(x, y);
					}
				}
			}

			window.display();
			needsUpdate = false;
		}
	}
}

