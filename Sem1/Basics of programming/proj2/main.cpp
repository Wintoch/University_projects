#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

#include "screens.h"
#include "charaters.h"


// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv) {
	int t1, t2, quit, frames, rc;
	char actionStatePlayer[128], actionStateEnemy1[128], actionStateEnemy2[128];
	double delta, worldTime, fpsTimer, fps, distance, etiSpeed;
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Surface *forestBackground;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;

	// console window is not visible, to see the printf output
	// the option:
	// project -> szablon2 properties -> Linker -> System -> Subsystem
	// must be changed to "Console"
	printf("wyjscie printfa trafia do tego okienka\n");
	printf("printf output goes here\n");

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
		}

	//fullscreen mode
	//rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,&window, &renderer);
	if(rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
		};
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Szablon do zdania drugiego 2017");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
	                              0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
	                           SDL_TEXTUREACCESS_STREAMING,
	                           SCREEN_WIDTH, SCREEN_HEIGHT);


	// wy��czenie widoczno�ci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	// wczytanie obrazka cs8x8.bmp
	charset = SDL_LoadBMP("./assets/cs8x8.bmp");
	if(charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
		};

	forestBackground = SDL_LoadBMP("./assets/backgrounds/las.bmp");
	if(forestBackground == NULL) {
        printf("SDL_LoadBMP(las.bmp) error: %s\n", SDL_GetError());
        SDL_FreeSurface(charset);
        SDL_FreeSurface(screen);
        SDL_DestroyTexture(scrtex);
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
	};
	SDL_SetColorKey(charset, true, 0x000000);

	char text[128];
	czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);

	//initlaization of characters
	Player player;
	Enemy enemy1;
	Enemy enemy2;

	char nicknameBuffer[64];
	memset(nicknameBuffer, 0, sizeof(nicknameBuffer));

	int programRunning = 1;
	while(programRunning) {
	
		if(CallMenu(screen, charset, scrtex, renderer, nicknameBuffer) == 0) {
			programRunning = 0;
			break;
		}

		player.InitPlayer(200.0, 700.0, nicknameBuffer, 1200);
		enemy1.InitEnemy(900.0, 700.0, 200, 0);
		enemy2.InitEnemy(1200.0, 650.0, 200, 1);
		
		t1 = SDL_GetTicks();

		frames = 0;
		fpsTimer = 0;
		fps = 0;
		worldTime = 0;

		//actionStates for player and enemy
		strcpy(actionStatePlayer, "idle");
		strcpy(actionStateEnemy1, "idle");
		strcpy(actionStateEnemy2, "idle");

		double cameraX = 0.0;
		double cameraY = 0.0;

		int levelState = 0; //0 - running, 1 - player dead, 2 - all enemies dead

		quit = 0;
		while(!quit) {
			t2 = SDL_GetTicks();

			// w tym momencie t2-t1 to czas w milisekundach,
			// jaki uplyna� od ostatniego narysowania ekranu
			// delta to ten sam czas w sekundach
			// here t2-t1 is the time in milliseconds since
			// the last screen was drawn
			// delta is the same time in seconds
			delta = (t2 - t1) * 0.001;
			t1 = t2;

			worldTime += delta;

			// --- GAME OVER / END CONDITIONS ---
			if (levelState == 0) {
				if (player.health <= 0) {
					levelState = 1; // Game Over
				}
				else if (enemy1.isDead && enemy2.isDead) {
					levelState = 2; // Level Complete
				}
			}
			// ----------------------------------
			DrawBackground(screen, forestBackground, (int)cameraX, (int)cameraY);

			fpsTimer += delta;
			if(fpsTimer > 0.5) {
				fps = frames * 2;
				frames = 0;
				fpsTimer -= 0.5;
				};

			DrawStatSection(screen, charset, worldTime, fps, player.nickname, player.health, enemy1.health, enemy2.health, player.score);

			if (levelState == 0) {
				// NORMAL GAMEPLAY LOOP
				if (player.comboCount > 1) {
					char comboText[32];
					snprintf(comboText, sizeof(comboText), "%dx COMBO!", player.comboCount);

					float scale = 4.0f + (player.comboCount * 0.5f);
					if (scale > 8.0f) scale = 8.0f;

					int textWidth = strlen(comboText) * 8 * scale;
					int drawX = (SCREEN_WIDTH - textWidth) / 2;
					int drawY = 150;

					SDL_SetSurfaceColorMod(charset, 255, 10, 144);
					DrawStringScaled(screen, drawX, drawY, comboText, charset, scale);
					SDL_SetSurfaceColorMod(charset, 255, 255, 255);
				}

				const Uint8* state = SDL_GetKeyboardState(NULL);
				player.HandleInput(state, delta, cameraX, forestBackground->w, SCREEN_WIDTH, actionStatePlayer);

				enemy1.UpdateAI(delta, player.posX, player.posY, forestBackground->w, actionStateEnemy1);
				enemy2.UpdateAI(delta, player.posX, player.posY, forestBackground->w, actionStateEnemy2);

				player.UpdateAnimation(delta, actionStatePlayer);
				enemy1.UpdateAnimation(delta, actionStateEnemy1);
				enemy2.UpdateAnimation(delta, actionStateEnemy2);

				player.CheckCollision(enemy1);
				player.CheckCollision(enemy2);
				enemy1.CheckCollision(player);
				enemy2.CheckCollision(player);
			}
			else {
				// END GAME SCREENS (Game Over or Level Complete)
				char endText[128];
				char scoreText[128];
				char optionText[128];

				// Keep drawing entities frozen in place
				player.Draw(screen, (int)cameraX, (int)cameraY);
				enemy1.Draw(screen, (int)cameraX, (int)cameraY);
				enemy2.Draw(screen, (int)cameraX, (int)cameraY);

				// Darken screen slightly via distinct rectangles if needed, 
				// or just draw text on top

				if (levelState == 1) {
					SDL_SetSurfaceColorMod(charset, 255, 0, 0);
					snprintf(endText, sizeof(endText), "GAME OVER");
					snprintf(optionText, sizeof(optionText), "Press ESC/M for Menu");
				}
				else {
					SDL_SetSurfaceColorMod(charset, 0, 255, 0);
					snprintf(endText, sizeof(endText), "LEVEL COMPLETED");
					snprintf(optionText, sizeof(optionText), "Press N for Next Level, ESC/M for Menu");
				}

				snprintf(scoreText, sizeof(scoreText), "Final Score: %d", player.score);

				DrawStringScaled(screen, (SCREEN_WIDTH - strlen(endText) * 8 * 4) / 2, 300, endText, charset, 4.0f);
				SDL_SetSurfaceColorMod(charset, 0, 0, 0);

				DrawStringScaled(screen, (SCREEN_WIDTH - strlen(scoreText) * 8 * 2) / 2, 400, scoreText, charset, 2.0f);
				DrawString(screen, (SCREEN_WIDTH - strlen(optionText) * 8) / 2, 450, optionText, charset);

				SDL_SetSurfaceColorMod(charset, 255, 255, 255);
			}

			// Always Draw entities (if game over, they stay in last position)
			if (levelState == 0) {
				player.Draw(screen, (int)cameraX, (int)cameraY);
				enemy1.Draw(screen, (int)cameraX, (int)cameraY);
				enemy2.Draw(screen, (int)cameraX, (int)cameraY);
			}

			SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
			SDL_RenderCopy(renderer, scrtex, NULL, NULL);
			SDL_RenderPresent(renderer);

			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						// In game: Exit to menu? Or just quit app?
						// Logic G.d: "return to menu".
						quit = 1; // Breaks inner loop, returns to outer menu loop
					}
					// If game ended, handle M and N
					if (levelState != 0) {
						if (event.key.keysym.sym == SDLK_m) {
							quit = 1; // Back to menu
						}
						else if (event.key.keysym.sym == SDLK_n && levelState == 2) {
							// Next Level (Demonstration: Reset everything)
							player.InitPlayer(200.0, 700.0, nicknameBuffer, 1200);
							enemy1.InitEnemy(900.0, 700.0, 200, 0); // Maybe harder?
							enemy2.InitEnemy(1200.0, 650.0, 200, 1);
							cameraX = 0;
							worldTime = 0;
							actionStatePlayer[0] = '\0'; // reset actions
							levelState = 0;
						}
					}
					break;
				case SDL_QUIT:
					quit = 1;
					programRunning = 0; // Full quit
					break;
				};
			};
			frames++;
		};
	}

	// zwolnienie powierzchni / freeing all surfaces
	player.Free();
	enemy1.Free();
	enemy2.Free();
	SDL_FreeSurface(forestBackground);
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
	};
