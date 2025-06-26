#include <iostream>
#include "raylib.h"
#include <cstdlib>

using namespace std;

void drawStar();
Vector2 CalculFunctionLinear(float mouseX, float mouseY, float starX, float starY);

const int MAX_STARS = 200;
const float STREGTH_IMPACT = 1.5;
const float FRICTION = 0.95f;
const int FPS = 240;
const int FORCEFIELD_RADIUS = 70;
const int ATTRACTION_RADIUS = 250;

Color ForceFieldColor = { 0, 255, 0, 100 };
Color AttractionColor = { 153, 102, 255 , 100 };


struct Star {
	float x, y;
	int size;
	bool isMoving = false;
	bool isAttracted = false;
	Vector2 velocity = {0.0f,0.0f};
};

int main() {
	const int screenWidth = 1600;
	const int screenHeight = 900;

	InitWindow(screenWidth, screenHeight, "ForceField");
	SetTargetFPS(FPS);
	srand(time(NULL));
	Star stars[MAX_STARS];
	for (int i = 0; i < MAX_STARS; i++) {
		stars[i].x = rand() % screenWidth;
		stars[i].y = rand() % screenHeight;
		stars[i].size = rand() % 10 + 5;
	}
	HideCursor();

	while (!WindowShouldClose()) {
		// update here 
		Vector2 mouse = GetMousePosition();
		int i= 0;
		for (i; i < MAX_STARS; i++) {
			Vector2 starLocation; starLocation.x = stars[i].x;starLocation.y = stars[i].y;

			if (CheckCollisionPointCircle(mouse, starLocation, FORCEFIELD_RADIUS) ) { stars[i].isMoving = true;
			}
			else {
				stars[i].isMoving = false;
			}

			if (CheckCollisionPointCircle(mouse, starLocation, ATTRACTION_RADIUS) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{ stars[i].isAttracted = true; }
			else { stars[i].isAttracted = false; }
		}
		
		
		////////////////////////////////////
		BeginDrawing();
		ClearBackground(BLACK);

		DrawCircle(mouse.x, mouse.y, FORCEFIELD_RADIUS, ForceFieldColor);
		DrawCircle(mouse.x, mouse.y, ATTRACTION_RADIUS, AttractionColor);

		for (int i = 0; i < MAX_STARS; i++) {
			if (stars[i].isMoving) {
				Vector2 force = CalculFunctionLinear(mouse.x, mouse.y, stars[i].x, stars[i].y);
				stars[i].velocity.x += force.x;
				stars[i].velocity.y += force.y;
			}
			if (stars[i].isAttracted) {
				Vector2 force = CalculFunctionLinear(mouse.x, mouse.y, stars[i].x, stars[i].y);
				stars[i].velocity.x += -force.x * 2;
				stars[i].velocity.y += -force.y * 2;
			}

			// Toujours appliquer la vitesse et friction
			stars[i].x += stars[i].velocity.x;
			stars[i].y += stars[i].velocity.y;
			
			stars[i].velocity.x *= FRICTION;
			stars[i].velocity.y *= FRICTION;

			DrawCircleGradient(stars[i].x, stars[i].y, stars[i].size, ORANGE, YELLOW);
		}



		
		EndDrawing();
	}
	CloseWindow();


	return 0;
}

Vector2 CalculFunctionLinear(float mouseX, float mouseY, float starX, float starY) {
	Vector2 dir;

	dir.x = starX - mouseX;
	dir.y = starY - mouseY;

	float length = sqrt(dir.x * dir.x + dir.y * dir.y);
	if (length == 0) length = 1; // éviter division par zéro

	dir.x = (dir.x / length)* STREGTH_IMPACT;
	dir.y = (dir.y / length) * STREGTH_IMPACT;

	return dir;
}