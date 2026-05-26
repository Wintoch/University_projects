#pragma once
#include <stdio.h>

extern "C" {
	#include"./SDL2-2.0.10/include/SDL.h"
}

struct InputEntry{
	int key;
	Uint32 timestamp;
};

class Character {
public:
	double posX, posY;
	SDL_Surface* sprite;
	SDL_Surface* spriteRight;
	SDL_Surface* spriteLeft;
	int width, height;
	SDL_Rect hurtbox;
	SDL_Rect hitbox;
	SDL_Rect currentFrame;
	char basePath[128];
	int framesCount;
	int currentFrameIndex;
	double animationTimer;
	double frameDuration;
	char currentAction[128];
	int health;
	int strokeWeight;
	double jumpOffset;
	double jumpVelocity;
	double gravity;
	bool facingRight;
	bool hasHit;
	bool isDead;

	Character();
	~Character();

	void Init(const char* folderPath, double startX, double startY);

	virtual void LoadAction(const char* actionState);

	void Draw(SDL_Surface* screen, int cameraX, int cameraY);

	virtual void UpdateAnimation(double deltaTime, const char* actionState);

	void UpdatePhysics(double delta);

	void CheckCollision(Character& other);

	void Free();
};

class Player : public Character {
public:
	char nickname[64];

	InputEntry InputEntry[10];
	int latestInputIndex;
	Uint8 lastKeyState[512];
	bool isDashing;
	double dashTimer;
	int comboCount;
	double comboTimer;
	int score;

	void InitPlayer(double startX, double startY, const char* nickname, int initialHealth);
	void UpdatePosition(double deltaX, double deltaY);
	void ScanInputs(const Uint8* state, double delta, bool& isJumpKick, bool& isDiveKick, bool& isTripleJab);
	void ProcessMovement(const Uint8* state, double delta, double& cameraX, int backgroundWidth, int screenWidth, bool& isMoving, bool& isJumping);
	void HandleInput(const Uint8* state, double delta, double& cameraX, int backgroundWidth, int screenWidth, char* actionState);
	void UpdateAnimation(double deltaTime, const char* actionState) override;
	void LoadAction(const char* actionState) override;
	void UpdateCombo(double delta);
};

class Enemy : public Character {
public :
	int type; // 0 - chaser, 1 - charger
	double stunTimer;
	double aiTimer;
	int aiState; //0 - wait, 1 - charges
	double attackCooldown;
	
	void InitEnemy(double startX, double startY, int initialHealth, int type);
	void LoadAction(const char* actionState) override;
	void UpdateAnimation(double deltaTime, const char* actionState) override;
	void UpdateChaserAI(double dx, double dy, double speed, char* actionState);
	void UpdateChargerAI(double playerX, double dx, double dy, double speed, double deltaTime, int backgroundWidth, char* actionState);
	void UpdateAI(double deltaTime, double playerX, double playerY, int backgroundWidth, char* actionState);
};

SDL_Surface* FlipSurfaceHorizontally(SDL_Surface* surface);