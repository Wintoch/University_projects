#include "charaters.h"
#include "screens.h"
#include <string.h>
#include <math.h>

Character::Character() {
	posX = 0.0;
	posY = 0.0;
	sprite = NULL;
	spriteLeft = NULL;
	spriteRight = NULL;
	width = 0;
	height = 0;
	currentFrame = { 0, 0, 0, 0 };
	hitbox = { 0, 0, 0, 0 };
	basePath[0] = '\0';
	framesCount = 1;
	currentFrameIndex = 0;
	animationTimer = 0.0;
	frameDuration = 0.2;
	currentAction[0] = '\0';
	strokeWeight = 0;
	jumpOffset = 0.0;
	jumpVelocity = 0.0;
	gravity = 980.0;
	facingRight = true;
	hasHit = false;
	isDead = false;
}

Character::~Character() {
	Free();
}

void Character::Init(const char* folderPath, double startX, double startY) {
	posX = startX;
	posY = startY;

	strncpy(basePath, folderPath, 127);
	basePath[127] = '\0';

	LoadAction("idle");
}

SDL_Surface* FlipSurfaceHorizontally(SDL_Surface* src) {
	if(!src) return NULL;
	SDL_Surface* surface = SDL_ConvertSurface(src, src->format, 0);
	if(!surface) return NULL;

	int width = surface->w;
	int height = surface->h;
	int bpp = surface->format->BytesPerPixel;

	SDL_LockSurface(surface);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width / 2; x++) {
			Uint8* leftPixel = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
			Uint8* rightPixel = (Uint8*)surface->pixels + y * surface->pitch + (width - 1 - x) * bpp;

			for (int i = 0; i < bpp; i++) {
				int temp = leftPixel[i];
				leftPixel[i] = rightPixel[i];
				rightPixel[i] = temp;
			}
		}
	}
	SDL_UnlockSurface(surface);
	return surface;
}

void Character::LoadAction(const char* actionState) {
	Free();

	hasHit = false;

	//remember current action
	strcpy(currentAction, actionState);
	currentAction[127] = '\0';

	char fullPath[256];
	//copy base path
	strcpy(fullPath, basePath);
	//add do base path
	strcat(fullPath, actionState);

	//add extension
	strcat(fullPath, ".bmp");

	//load file
	spriteRight = SDL_LoadBMP(fullPath);

	if (spriteRight == NULL) {
		printf("SDL_LoadBMP(%s) error: %s\n", fullPath, SDL_GetError());
		return;
	}

	spriteLeft = FlipSurfaceHorizontally(spriteRight);

	//set magenta as transparent and set transparency
	Uint32 colorkey = SDL_MapRGB(spriteRight->format, 255, 0, 255);
	SDL_SetColorKey(spriteRight, SDL_TRUE, colorkey);
	if(spriteLeft){
		SDL_SetColorKey(spriteLeft, SDL_TRUE, colorkey);
	}

	if (facingRight) {
		sprite = spriteRight;
	}
	else {
		sprite = spriteLeft;
	}

	framesCount = 1;
	frameDuration = 0.2;
	currentFrameIndex = 0;
	animationTimer = 0.0;

	width = sprite->w / framesCount;
	height = sprite->h;
	currentFrame = { 0, 0, width, height };
	hurtbox = { 0, 0, 0, 0 };
}

void Player::LoadAction(const char* actionState) {
	Character::LoadAction(actionState);

	framesCount = 1;
	frameDuration = 0.2;
	if (strstr(actionState, "idle")) framesCount = 4;
	else if (strstr(actionState, "hurt")) {
		framesCount = 2;
		frameDuration = 0.2;
	}
	else if (strstr(actionState, "walk")) {
		framesCount = 10;
		frameDuration = 0.05;
	}
	else if (strstr(actionState, "jab")) {
		framesCount = 3;
		frameDuration = 0.05;
	}
	else if (strstr(actionState, "jump_kick")) {
		framesCount = 3;
		frameDuration = 0.09;
	}
	else if (strstr(actionState, "dive_kick")) {
		framesCount = 5;
		frameDuration = 0.12;
	}
	else if (strstr(actionState, "kick")) {
		framesCount = 5;
		frameDuration = 0.09;
	}
	else if (strstr(actionState, "punch")) {
		framesCount = 3;
		frameDuration = 0.07;
	}
	else if (strstr(actionState, "jump")) {
		framesCount = 4;
		frameDuration = 0.1;
	}

	if (sprite != NULL) {
		width = sprite->w / framesCount;
		height = sprite->h;
		currentFrame = { 0, 0, width, height };

		hurtbox = { width * 7 / 16, height * 3 / 8, width * 2 / 16, height * 5 / 8 };
	}

}

void Enemy::LoadAction(const char* actionState) {
	if(isDead && strstr(actionState, "hurt") == NULL) {
		return;
	}

	Character::LoadAction(actionState);

	SDL_Surface* temp = spriteRight;
	spriteRight = spriteLeft;
	spriteLeft = temp;

	if (facingRight) sprite = spriteRight;
	else sprite = spriteLeft;

	framesCount = 1;
	frameDuration = 0.2;

	if (strstr(actionState, "hurt")) {
		framesCount = 4;
		frameDuration = 0.15;
	}
	else if (strstr(actionState, "walk")) {
		framesCount = 4;
		frameDuration = 0.1;
	}
	else if (strstr(actionState, "idle")) {
		framesCount = 4;
	}
	else if (strstr(actionState, "punch")) {
		framesCount = 3;
		frameDuration = 0.15;
	}

	if (sprite != NULL) {
		width = sprite->w / framesCount;
		height = sprite->h;
		currentFrame = { 0, 0, width, height };
		hurtbox = { width * 7 / 16, height * 3 / 8, width * 2 / 16, height * 5 / 8 };
	}
}

void Character::UpdateAnimation(double deltaTime, const char* actionState) {
	bool isHurt = (strstr(currentAction, "hurt") != NULL);
	bool animationFinished = (currentFrameIndex >= framesCount - 1);

	if (isHurt && !animationFinished) {

	}
	else if (strcmp(currentAction, actionState) != 0) {
		LoadAction(actionState);
	}
	
	if (facingRight) {
		sprite = spriteRight;
	}
	else {
		sprite = spriteLeft;
	}

	animationTimer += deltaTime;
	if (animationTimer >= frameDuration) {
		animationTimer -= frameDuration;
		currentFrameIndex++;
		if (currentFrameIndex >= framesCount) {

			if (strstr(currentAction, "jab") || strstr(currentAction, "punch") || strstr(currentAction, "kick")) {
				hasHit = false;
			};
			

			currentFrameIndex = 0;
		}
	}

	if (facingRight) currentFrame.x = currentFrameIndex * width;
	else currentFrame.x = (framesCount - 1 - currentFrameIndex) * width;

	hitbox = { 0, 0, 0, 0 };

	UpdatePhysics(deltaTime);
}

void Enemy::UpdateAnimation(double deltaTime, const char* actionState) {
	if (isDead) {
		if (strstr(currentAction, "hurt") == NULL) {
			LoadAction("hurt");
		}

		//only till last frame
		if (currentFrameIndex < framesCount - 1) {
			animationTimer += deltaTime;
			if (animationTimer >= frameDuration) {
				animationTimer -= frameDuration;
				currentFrameIndex++;
			}
		}
		else {
			currentFrameIndex = framesCount - 1;
		}

		if (facingRight) {
			sprite = spriteRight;
			currentFrame.x = (framesCount - 1 - currentFrameIndex) * width;
		}
		else {
			sprite = spriteLeft;
			currentFrame.x = currentFrameIndex * width;
		}

		hitbox = { 0, 0, 0, 0 };

		UpdatePhysics(deltaTime);
	} else {
		Character::UpdateAnimation(deltaTime, actionState);
		if (type == 1 && aiState == 1) {
			// CHARGER hitbox is hutrbox during charge
			hitbox = hurtbox;
			strokeWeight = 4;
		}
		else if (strstr(currentAction, "punch")) {
			if (currentFrameIndex == 1) {
				if (facingRight) {
					hitbox = { width * 6 / 10, height * 11 / 24, width / 4, height / 15 };
				}
				else {
					hitbox = { width * 3 / 16, height * 11 / 24, width / 4, height / 15 };
				}
			}
		}
	}
}

//Hitbox adjustments for player attacks
void Player::UpdateAnimation(double deltaTime, const char* actionState) {
	UpdateCombo(deltaTime);

	Character::UpdateAnimation(deltaTime, actionState);

	if (strstr(currentAction, "jab") || strstr(currentAction, "punch")) {
		if (currentFrameIndex == 1) {
			if (facingRight) {
				hitbox = { width * 6 / 10, height * 11 / 24, width / 4, height / 15 };
			}
			else {
				hitbox = { width * 3 / 16, height * 11 / 24, width / 4, height / 15 };
			}
		}
	} else if (strstr(currentAction, "jump_kick")) {
		if (currentFrameIndex == 1) {
			if (facingRight) {
				hitbox = { width * 11 / 20, height * 2 / 8, width / 6, height * 3 / 10 };
			}
			else {
				hitbox = { width * 6 / 20, height * 2 / 8, width / 6, height * 3 / 10 };
			}
		}
	} else if (strstr(currentAction, "dive_kick")) {
		if (currentFrameIndex == 3) {
			if (facingRight) {
				hitbox = { width * 3 / 5, height * 17 / 24, width * 3 / 10, height / 12 };
			}
			else
			{
				hitbox = { width * 1 / 10, height * 17 / 24, width * 3 / 10, height / 12 };
			}
		}
	} else if (strstr(currentAction, "kick")) {
		if (currentFrameIndex == 1) {
			if (facingRight) {
				hitbox = { width * 5 / 10, height * 13 / 24, width / 3, height / 12 };
			}
			else {
				hitbox = { width * 2 / 16, height * 13 / 24, width / 3, height / 12 };
			}
		}
	}
}

void Player::UpdatePosition(double deltaX, double deltaY) {
	posX += deltaX;
	posY += deltaY;
	if (jumpOffset > 0.0) {
		jumpOffset -= deltaY * 2.0;
		if (jumpOffset < 0.0) jumpOffset = 0.0;
	}
}

void Character::Draw(SDL_Surface* screen, int cameraX, int cameraY) {
	if (sprite != NULL) {
		SDL_Rect dest;

		float scale = 3.0f;

		dest.w = (int)(width * scale);
		dest.h = (int)(height * scale);
		dest.x = (int)(posX - cameraX - dest.w / 2);
		dest.y = (int)(posY - cameraY + jumpOffset - dest.h / 2);
		SDL_BlitScaled(sprite, &currentFrame, screen, &dest);

		//uncommnet to draw hurtbox and hitbox for debugging
		//// --- DRAWING HURTBOX FOR DEBUGGING ---
		////Position and size of hurtbox after scaling
		//int hbX = dest.x + (int)(hurtbox.x * scale);
		//int hbY = dest.y + (int)(hurtbox.y * scale);
		//int hbW = (int)(hurtbox.w * scale);
		//int hbH = (int)(hurtbox.h * scale);

		//Uint32 debugColor = SDL_MapRGB(screen->format, 255, 0, 0);

		//DrawRectangle(screen, hbX, hbY, hbW, hbH, debugColor, debugColor);

		//// --- DRAWING HITBOX FOR DEBBUGING ---
		//// Only if active hitbox
		//if (hitbox.w > 0 && hitbox.h > 0) {
		//	int hitX = dest.x + (int)(hitbox.x * scale);
		//	int hitY = dest.y + (int)(hitbox.y * scale);
		//	int hitW = (int)(hitbox.w * scale);
		//	int hitH = (int)(hitbox.h * scale);
		//	Uint32 hitColor = SDL_MapRGB(screen->format, 0, 255, 0); // Zielony
		//	DrawRectangle(screen, hitX, hitY, hitW, hitH, hitColor, hitColor);
		//}
		//// -----------------------------------------


	}
}

void Character::Free() {
    if (spriteRight != NULL) {
        SDL_FreeSurface(spriteRight);
        spriteRight = NULL;
    }
    if (spriteLeft != NULL) {
        SDL_FreeSurface(spriteLeft);
        spriteLeft = NULL;
    }
    sprite = NULL;
}

void Character::UpdatePhysics(double delta){
	if (jumpOffset < 0.0 || jumpVelocity != 0.0) {
		jumpOffset += jumpVelocity * delta;
		jumpVelocity += gravity * delta;
		if (jumpOffset > 0.0) {
			jumpOffset = 0.0;
			jumpVelocity = 0.0;
		}
	}
}

void Player::InitPlayer(double startX, double startY, const char* nick, int initialHealth) {
	Init("./assets/characters/Brawler_Girl/", startX, startY);

	if (nick != NULL) {
		strncpy(nickname, nick, 63);
		nickname[63] = '\0';
	}
	else {
		strcpy(nickname, "Hero");
	}

	health = initialHealth;

	memset(lastKeyState, 0, sizeof(lastKeyState));
	latestInputIndex = 0;
	isDashing = false;
	dashTimer = 0.0;
	memset(InputEntry, 0, sizeof(InputEntry));

	comboCount = 0;
	comboTimer = 0.0;
	score = 0;
}
void Player::ScanInputs(const Uint8* state, double delta, bool& isJumpKick, bool& isDiveKick, bool& isTriplePunch) {
	Uint32 currentTime = SDL_GetTicks();

	static bool showDebugBuffer = false;

	int monitoredKeys[] = {
		SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D,
		SDL_SCANCODE_J, SDL_SCANCODE_K, SDL_SCANCODE_P, SDL_SCANCODE_SPACE
	};

	// Toggle developer mode (F1)
	if (state[SDL_SCANCODE_B] && !lastKeyState[SDL_SCANCODE_B]) {
		showDebugBuffer = !showDebugBuffer;
		printf("Developer Mode: %s\n", showDebugBuffer ? "ON" : "OFF");

		if (showDebugBuffer) {
			printf("Buffer: ");
			for (int i = 0; i < 10; i++) {
				int idx = (latestInputIndex - i + 10) % 10;
				if (InputEntry[idx].key != 0) {
					printf("[%s] ", SDL_GetScancodeName((SDL_Scancode)InputEntry[idx].key));
				}
			}
			printf("| Action: %s\n", currentAction);
		}
	}

	if (isDashing) {
		dashTimer -= delta;
		if (dashTimer <= 0.0) isDashing = false;
	}

	if (!state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D]) isDashing = false;
	for (int key : monitoredKeys) {
		if (state[key] && !lastKeyState[key]) {
			latestInputIndex = (latestInputIndex + 1) % 10;
			InputEntry[latestInputIndex].key = key;
			InputEntry[latestInputIndex].timestamp = currentTime;

			int index2 = (latestInputIndex - 1 + 10) % 10;
			int index3 = (latestInputIndex - 2 + 10) % 10;

			// Dash detection
			if (key == SDL_SCANCODE_A || key == SDL_SCANCODE_D) {
				bool sameKeys = (InputEntry[index2].key == key);
				bool fastEnough = (currentTime - InputEntry[index2].timestamp) <= 300;
				if (sameKeys && fastEnough) {
					isDashing = true;
					dashTimer = 0.20;
				}
			}
			//// Jump Kick detection
			//if (key == SDL_SCANCODE_J) {
			//	//bool diveKick = (InputEntry[index2].key == SDL_SCANCODE_SPACE);
			//	bool JSPACE = (InputEntry[index2].key == SDL_SCANCODE_SPACE);
			//	bool fastEnoughKick = (currentTime - InputEntry[index2].timestamp) <= 300;
			//	if (JSPACE && fastEnoughKick) {
			//		isJumpKick = true;
			//		jumpVelocity = -50.0;
			//	}
			//}
			//Punch detection
			if (key == SDL_SCANCODE_P) {
				bool prevP = (InputEntry[index2].key == SDL_SCANCODE_P);
				bool prevPrevP = (InputEntry[index3].key == SDL_SCANCODE_P);
				bool fastEnoughCombo = (currentTime - InputEntry[index3].timestamp) <= 700;

				if (prevP && prevPrevP && fastEnoughCombo) {
					isTriplePunch = true;
				}
			}
			// Dive or Jump Kick detection
			if (key == SDL_SCANCODE_K) {
				bool pressedJ = (InputEntry[index2].key == SDL_SCANCODE_J);
				bool fastEnough = (currentTime - InputEntry[index2].timestamp) <= 300;

				if (pressedJ && fastEnough) {
					isJumpKick = true;
					jumpVelocity = -50.0;
				}
				else if (jumpOffset < -50.0) {
					isDiveKick = true;
				}
			}
			if (showDebugBuffer) {
				const char* actionToDisplay = currentAction;

				// Nadpisz nazw� akcji, je�li wykryli�my now� komend� w tej klatce
				if (isJumpKick) actionToDisplay = "jump_kick";
				else if (isDiveKick) actionToDisplay = "dive_kick";
				else if (isTriplePunch) actionToDisplay = "punch"; // Triple punch to wci�� punch, ale mo�na zmieni� nazw� je�li trzeba
				else {
					if (key == SDL_SCANCODE_J) actionToDisplay = "jab";
					else if (key == SDL_SCANCODE_K) actionToDisplay = "kick";
					else if (key == SDL_SCANCODE_P) actionToDisplay = "punch";
					else if (key == SDL_SCANCODE_SPACE) actionToDisplay = "jump";
					else if (key == SDL_SCANCODE_W || key == SDL_SCANCODE_S ||
						key == SDL_SCANCODE_A || key == SDL_SCANCODE_D) {
						actionToDisplay = "walk";
					}
				}

				printf("Buffer: ");
				for (int i = 0; i < 10; i++) {
					int idx = (latestInputIndex - i + 10) % 10;
					if (InputEntry[idx].key != 0) {
						printf("[%s] ", SDL_GetScancodeName((SDL_Scancode)InputEntry[idx].key));
					}
				}
				printf("| Action: %s\n", actionToDisplay);
			}
		}
	}
	memcpy(lastKeyState, state, sizeof(lastKeyState));
}

void Player::ProcessMovement(const Uint8* state, double delta, double& cameraX, int backgroundWidth, int screenWidth, bool& isMoving, bool& isJumping) {
	double baseSpeed = 300.0 * delta;
	double moveSpeed = isDashing ? baseSpeed * 5.0 : baseSpeed;

	if (state[SDL_SCANCODE_D] && posX < (backgroundWidth - screenWidth * 0.5 - width / 2)) {
		UpdatePosition(moveSpeed, 0.0);
		isMoving = true;
		if (posX - cameraX > screenWidth - 200) cameraX += moveSpeed;
		facingRight = true;
	}
	if (state[SDL_SCANCODE_A] && posX > (width / 2)) {
		UpdatePosition(-moveSpeed, 0.0);
		isMoving = true;
		if (posX - cameraX < 200) cameraX -= moveSpeed;
		facingRight = false;
	}
	if (state[SDL_SCANCODE_W] && posY > 620.0) {
		UpdatePosition(0.0, -moveSpeed);
		isMoving = true;
	}
	if (state[SDL_SCANCODE_S] && posY < 710.0) {
		UpdatePosition(0.0, moveSpeed);
		isMoving = true;
	}
	if (state[SDL_SCANCODE_SPACE] && jumpOffset == 0.0) {
		isJumping = true;
		jumpVelocity = -600.0;
	}

	// Camera boundaries
	if (cameraX < 0) cameraX = 0;
	if (cameraX > (backgroundWidth - screenWidth * 1.5)) cameraX = (backgroundWidth - screenWidth * 1.5);

}

void Player::HandleInput(const Uint8* state, double delta, double& cameraX, int backgroundWidth, int screenWidth, char* actionState) {
	bool isJumpKick = false;
	bool isDiveKick = false;
	bool isTriplePunch = false;
	//bool currentlyJumpKicking = (strstr(currentAction, "jump_kick") != NULL);

	ScanInputs(state, delta, isJumpKick, isDiveKick, isTriplePunch);

	bool isActionLocked = (strstr(currentAction, "jab") || strstr(currentAction, "kick") || strstr(currentAction, "punch") || strstr(currentAction, "jump_kick"));

	if (isActionLocked && isJumpKick && strstr(currentAction, "jab")) {
		isActionLocked = false;
	}

	if (isActionLocked) {
		bool animationOngoing = (currentFrameIndex < framesCount - 1);

		if (currentFrameIndex < framesCount - 1 || (currentFrameIndex == framesCount - 1 && animationTimer < frameDuration - 0.02)) {
			strcpy(actionState, currentAction);
			return;
		}
	}

	bool isJumping = false;
	bool isMoving = false;

	ProcessMovement(state, delta, cameraX, backgroundWidth, screenWidth, isMoving, isJumping);

	if (isJumpKick) {
		strcpy(actionState, "jump_kick");
		strokeWeight = 4;
	} else if (isDiveKick) {
		strcpy(actionState, "dive_kick");
		strokeWeight = 5;
	} else if (jumpOffset < 0.0 && !isJumpKick) {
		strcpy(actionState, "jump");
	} else if (isTriplePunch) {
		strcpy(actionState, "punch");
		strokeWeight = 2;
	} else if (state[SDL_SCANCODE_J]) {
		strcpy(actionState, "jab");
		strokeWeight = 1;
	} else if (state[SDL_SCANCODE_K]) {
		strcpy(actionState, "kick");
		strokeWeight = 3;
	} else if (isJumping) {
		strcpy(actionState, "jump");
	} else if (isMoving) {
		strcpy(actionState, "walk");
	} else {
		strcpy(actionState, "idle");
	}
}

void Player::UpdateCombo(double delta) {
	if (comboCount > 0) {
		comboTimer -= delta;
		if (comboTimer <= 0.0) {
			comboCount = 0;
			comboTimer = 0.0;
		}
	}
}

void Character::CheckCollision(Character& other) {
	if (this->hitbox.w <= 0 || this->hitbox.h <= 0) return;
	if (this->hasHit) return;
	if (other.isDead) return;

	//depth tolerance for 2.5D effect
	double depthTolerance = 20.0;
	if (fabs(this->posY - other.posY) > depthTolerance) {
		return;
	}

	float scale = 3.0f;
	SDL_Rect myRect;
	myRect.x = (int)(this->posX - (this->width * scale) / 2 + this->hitbox.x * scale);
	myRect.y = (int)(this->posY + this->jumpOffset - (this->height * scale) / 2 + this->hitbox.y * scale);
	myRect.w = (int)(this->hitbox.w * scale);
	myRect.h = (int)(this->hitbox.h * scale);

	SDL_Rect otherRect;
	otherRect.x = (int)(other.posX - (other.width * scale) / 2 + other.hurtbox.x * scale);
	otherRect.y = (int)(other.posY + other.jumpOffset - (other.height * scale) / 2 + other.hurtbox.y * scale);
	otherRect.w = (int)(other.hurtbox.w * scale);
	otherRect.h = (int)(other.hurtbox.h * scale);

	if (SDL_HasIntersection(&myRect, &otherRect)) {
		this->hasHit = true;

		Player* attackerPlayer = dynamic_cast<Player*>(this);
		Player* victimPlayer = dynamic_cast<Player*>(&other);
		Enemy* victimEnemy = dynamic_cast<Enemy*>(&other);

		if (attackerPlayer != nullptr) {
			attackerPlayer->comboCount++;
			attackerPlayer->comboTimer = 0.3;
			int multiplier = attackerPlayer->comboCount;

			int basePoints = 100;
			attackerPlayer->score += basePoints * multiplier;
		}
		else if (victimPlayer != nullptr) {
			int penalty = 200;
			if (victimPlayer->score >= penalty) victimPlayer->score -= penalty;
			else victimPlayer->score = 0;
		}

		other.health -= 10 * strokeWeight;

		if (other.health <= 0) {
			other.health = 0;
			other.isDead = true;
			other.LoadAction("hurt");
			if (victimEnemy != nullptr) victimEnemy->stunTimer = 0.0;
		}
		else {
			if (victimEnemy != nullptr) {
				victimEnemy->stunTimer = 0.4;
				victimEnemy->LoadAction("hurt");
			}
			else if (victimPlayer != nullptr) {
				victimPlayer->LoadAction("hurt");
				victimPlayer->comboCount = 0;
			}
		}

	}
}

void Enemy::InitEnemy(double startX, double startY, int initialHealth, int enemyType) {
	Init("./assets/characters/Enemy_Punk/", startX, startY);
	health = initialHealth;
	type = enemyType;
	stunTimer = 0.0;
	aiTimer = 0.0;
	aiState = 0;
	isDead = false;
}

void Enemy::UpdateChaserAI(double dx, double dy, double speed, char* actionState) {
	bool inRangeX = (fabs(dx) <= 70.0);
	bool inRangeY = (fabs(dy) <= 20.0);

	if (!inRangeX) {
		posX += (dx > 0) ? speed : -speed;
		strcpy(actionState, "walk");
		facingRight = (dx > 0);
	}

	if (fabs(dy) > 10.0) {
		posY += (dy > 0) ? speed : -speed;
		strcpy(actionState, "walk");
	}

	if (inRangeX && inRangeY && attackCooldown <= 0.0) {
		strcpy(actionState, "punch");
		attackCooldown = 1.5;
		facingRight = (dx > 0);
		strokeWeight = 2;
	}
}

void Enemy::UpdateChargerAI(double playerX, double dx, double dy, double speed, double deltaTime, int backgroundWidth, char* actionState) {
	double leftBound = width * 0.5;
	float scale = 3.0f;
	double rightBoundMargin = (width * scale) * 2.5;
	double rightBound = backgroundWidth - rightBoundMargin;

	if (aiState == 0) { // State: Aligning / Waiting
		facingRight = (playerX > posX);
		double targetX = playerX + (facingRight ? -250.0 : 250.0);
		double distToTargetX = targetX - posX;

		bool alignedY = fabs(dy) < 20.0;
		bool alignedX = fabs(distToTargetX) < 20.0;

		// Logic "cant backup so i attack"
		if (distToTargetX < 0 && posX <= leftBound + 10.0) alignedX = true;
		else if (distToTargetX > 0 && posX >= rightBound - 10.0) alignedX = true;

		if (!alignedX || !alignedY) {
			if (!alignedX) posX += (distToTargetX > 0 ? speed : -speed);
			if (!alignedY) posY += (dy > 0 ? speed : -speed);
			strcpy(actionState, "walk");
			aiTimer = 0.0;
		}
		else {
			strcpy(actionState, "idle");
			aiTimer += deltaTime;
			if (aiTimer > 1.0) {
				aiState = 1;
				aiTimer = 0.5;
			}
		}
	}
	else if (aiState == 1) { // State: Charging
		double chargeSpeed = 600.0 * deltaTime;
		posX += (facingRight ? chargeSpeed : -chargeSpeed);
		strcpy(actionState, "walk");

		aiTimer -= deltaTime;
		if (aiTimer <= 0.0) {
			aiState = 0;
			aiTimer = 0.0;
		}
	}
};

void Enemy::UpdateAI(double deltaTime, double playerX, double playerY, int backgroundWidth, char* actionState) {
	if (health <= 0 && !isDead) {
		isDead = true;
		LoadAction("hurt");
	}
	if (isDead) {
		strcpy(actionState, "hurt");
		return;
	}
	if (stunTimer > 0.0) {
		stunTimer -= deltaTime;
		strcpy(actionState, "hurt");
		if (stunTimer <= 0.0) strcpy(actionState, "idle");
		return;
	}

	bool isAttacking = (strstr(currentAction, "punch") != NULL);
	if (isAttacking && currentFrameIndex < framesCount - 1) {
		strcpy(actionState, currentAction);
		return;
	}

	if (attackCooldown > 0.0) attackCooldown -= deltaTime;

	double speed = 150.0 * deltaTime;
	double dx = playerX - posX;
	double dy = playerY - posY;

	strcpy(actionState, "idle");

	if (type == 0) {
		UpdateChaserAI(dx, dy, speed, actionState);
	}
	else {
		UpdateChargerAI(playerX, dx, dy, speed, deltaTime, backgroundWidth, actionState);
	}

	double leftLimit = width * 0.5;
	if (posX < leftLimit) posX = leftLimit;
	float scale = 3.0f;
	double rightLimit = (width * scale) * 2.5;
	if (posX > backgroundWidth - rightLimit) posX = backgroundWidth - rightLimit;

}