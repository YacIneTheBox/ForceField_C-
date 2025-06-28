#include <iostream>
#include "raylib.h"
#include <cstdlib>

using namespace std;

Vector2 CalculFunctionLinear(Vector2 mouse, Vector2 star);

const int MAX_STARS = 400;
const float STREGTH_IMPACT = 1.5;
const float FRICTION = 0.95f;
const int FPS = 240;
const int FORCEFIELD_RADIUS = 70;
const int ATTRACTION_RADIUS = 250; 
const float BRILLANCE = 0.5f;
const int DEFAULT_BRILLANCE = 170;


                                     
struct Star {
	float x, y;
	int size;
	bool isMoving = false;
	bool isAttracted = false;
	Vector2 velocity = {0.0f,0.0f};
	Color color = WHITE;
};

int main() {
	const int screenWidth = 1600;
	const int screenHeight = 900;

	InitWindow(screenWidth, screenHeight, "ForceField");
	SetTargetFPS(FPS);
	srand(time(NULL));


	Color ForceFieldColor = { 204, 255, 204, 100 };
	Color AttractionColor = { 204, 255, 255 , 100 };
	Color col1 = { 255,255,255,DEFAULT_BRILLANCE };
	Color col2 = { 255, rand() % 255 + 102, rand() % 102 ,DEFAULT_BRILLANCE };


	Star stars[MAX_STARS];
	for (int i = 0; i < MAX_STARS; i++) {
		stars[i].x = rand() % screenWidth;
		stars[i].y = rand() % screenHeight;
		stars[i].size = rand() % 10 + 5;
		if (i % 2 == 0) { stars[i].color = col2; }
			
		
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

			if (CheckCollisionPointCircle(mouse, starLocation, ATTRACTION_RADIUS) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
			{ stars[i].isAttracted = true; }
			else { stars[i].isAttracted = false; }
		}
		
		
		////////////////////////////////////  eco
		BeginDrawing();
		ClearBackground(BLACK);

		DrawCircle(mouse.x, mouse.y, FORCEFIELD_RADIUS, ForceFieldColor);
		DrawCircle(mouse.x, mouse.y, ATTRACTION_RADIUS, AttractionColor);

		for (int i = 0; i < MAX_STARS; i++) {
			Vector2 starCoor = { stars[i].x,stars[i].y };
			if (stars[i].isMoving) {	// REPULSION
				Vector2 force = CalculFunctionLinear(mouse, starCoor);
				stars[i].velocity.x += force.x;
				stars[i].velocity.y += force.y;

				stars[i].color.a = 255;
			}
			if (stars[i].isAttracted) { // ATTRACTION
				Vector2 force = CalculFunctionLinear(mouse, starCoor);
				stars[i].velocity.x += -force.x;
				stars[i].velocity.y += -force.y;
			}

			// Toujours appliquer la vitesse et friction
			stars[i].x += stars[i].velocity.x;
			stars[i].y += stars[i].velocity.y;
			
			
			stars[i].velocity.x *= FRICTION;
			stars[i].velocity.y *= FRICTION;


			if (stars[i].color.a > DEFAULT_BRILLANCE) { stars[i].color.a -= BRILLANCE; }

			DrawCircle(stars[i].x, stars[i].y, stars[i].size, stars[i].color);

		}
		EndDrawing();
	}
	CloseWindow();


	return 0;
}

Vector2 CalculFunctionLinear(Vector2 mouse, Vector2 star) {
	Vector2 dir;

	dir.x = star.x - mouse.x;
	dir.y = star.y - mouse.y;

	float length = sqrt(dir.x * dir.x + dir.y * dir.y);
	if (length == 0) length = 1; // éviter division par zéro

	dir.x = (dir.x / length)* STREGTH_IMPACT;
	dir.y = (dir.y / length) * STREGTH_IMPACT;

	return dir;
}