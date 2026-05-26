#include "screens.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

int czarny;
int zielony;
int czerwony;
int niebieski;

// narysowanie napisu txt na powierzchni screen, zaczynaj�c od punktu (x, y)
// charset to bitmapa 128x128 zawieraj�ca znaki
// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawString(SDL_Surface* screen, int x, int y, const char* text,
	SDL_Surface* charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};

void DrawStringScaled(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset, float scale) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = (int)(8 * scale);
	d.h = (int)(8 * scale);
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitScaled(charset, &s, screen, &d);
		x += (int)(8 * scale);
		text++;
	};
};

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt �rodka obrazka sprite na ekranie
// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};


// rysowanie pojedynczego pixela
// draw a single pixel
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
};


// rysowanie linii o d�ugo�ci l w pionie (gdy dx = 0, dy = 1) 
// b�d� poziomie (gdy dx = 1, dy = 0)
// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};


// rysowanie prostok�ta o d�ugo�ci bok�w l i k
// draw a rectangle of size l by k
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};

void DrawBackground(SDL_Surface* screen, SDL_Surface* background, int cameraX, int cameraY) {
	//background photo part that we're cutting to fit onto the screen
	SDL_Rect srcRect = {(int)cameraX, (int)cameraY, (int)(SCREEN_WIDTH*1.5), (int)(SCREEN_HEIGHT*1.5)};
	//destination screen for the background photo
	SDL_Rect destRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	SDL_BlitScaled(background, &srcRect, screen, &destRect);
}

int CallMenu(SDL_Surface* screen, SDL_Surface* charset, SDL_Texture* scrtex, SDL_Renderer* renderer, char* playerNickname) {
	SDL_Event event;
	int quit = 0;
	char text[256];
	char text1[256];
	char text2[256];
	char text3[256];
	char text4[256];
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int inputState = 0; // 0 - main menu, 1- Nickname input

	char errorText[256];
	Uint32 messageEndTime = 0;

	if (playerNickname[0] == '\0') strcpy(playerNickname, "Hero");

	while(!quit){
		SDL_FillRect(screen, NULL, czarny);
		if (inputState == 0) {
			snprintf(text, sizeof(text), "Main menu");
			snprintf(text1, sizeof(text), "Press N to start a new game");
			snprintf(text2, sizeof(text), "Press ESC to quit");
			snprintf(text3, sizeof(text), "Press L to check Leaderboard");
			snprintf(text4, sizeof(text), "Press G to choose level");
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, screen->h / 2 - 8, text, charset);
			DrawString(screen, screen->w / 2 - strlen(text1) * 8 / 2, (screen->h / 2 + 16), text1, charset);
			DrawString(screen, screen->w / 2 - strlen(text2) * 8 / 2, screen->h / 2 + 32, text2, charset);
			DrawString(screen, screen->w / 2 - strlen(text3) * 8 / 2, screen->h / 2 + 48, text3, charset);
			DrawString(screen, screen->w / 2 - strlen(text4) * 8 / 2, screen->h / 2 + 64, text4, charset);

			if (SDL_GetTicks() < messageEndTime) {
				SDL_SetSurfaceColorMod(charset, 255, 0, 0);
				DrawString(screen, screen->w / 2 - strlen(errorText) * 8 / 2, screen->h / 2 + 88, errorText, charset);
				SDL_SetSurfaceColorMod(charset, 255, 255, 255);
			}
		}
		else if (inputState == 1) {
			snprintf(text, sizeof(text), "Enter your nickname:");
			snprintf(text1, sizeof(text), "%s", playerNickname);
			snprintf(text2, sizeof(text), "Press ENTER to continue, ESC to back");

			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, screen->h / 2 - 20, text, charset);

			// Draw Nickname
			SDL_SetSurfaceColorMod(charset, 0, 255, 0);
			DrawString(screen, screen->w / 2 - strlen(text1) * 8 / 2, screen->h / 2, text1, charset);
			SDL_SetSurfaceColorMod(charset, 255, 255, 255);

			DrawString(screen, screen->w / 2 - strlen(text2) * 8 / 2, screen->h / 2 + 20, text2, charset);
		}

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_TEXTINPUT:
				if (inputState == 1) {
					if (strlen(playerNickname) < 15) {
						strcat(playerNickname, event.text.text);
					}
				}
				break;
			case SDL_KEYDOWN:
				if (inputState == 0) {
					if (event.key.keysym.sym == SDLK_n) {
						inputState = 1;
						strcpy(playerNickname, ""); // Clear default on entry
						SDL_StartTextInput();
						SDL_Event dummy;
						while (SDL_PollEvent(&dummy));
					}
					else if (event.key.keysym.sym == SDLK_l || event.key.keysym.sym == SDLK_g) {
						snprintf(errorText, sizeof(errorText), "Opcja niedostepna!");
						messageEndTime = SDL_GetTicks() + 2000;
					}
					else if (event.key.keysym.sym == SDLK_ESCAPE) {
						SDL_StopTextInput();
						return 0;
					}
				}
				else if (inputState == 1) {
					if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
						if (strlen(playerNickname) > 0) {
							SDL_StopTextInput();
							return 1;
						}
					}
					else if (event.key.keysym.sym == SDLK_BACKSPACE) {
						int len = strlen(playerNickname);
						if (len > 0) playerNickname[len - 1] = '\0';
					}
					else if (event.key.keysym.sym == SDLK_ESCAPE) {
						SDL_StopTextInput();
						inputState = 0;
					}
				}
				break;
			case SDL_QUIT:
				SDL_StopTextInput();
				return 0;
			}
		}
		SDL_Delay(10);
	}
	SDL_StopTextInput();
	return 0;
}

void FreeAll(SDL_Surface* screen, SDL_Surface* charset, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer) {
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void DrawHealthBar(SDL_Surface* screen, int x, int y, int health, int maxHealth, int width, int height) {
	float healthPerc = (float)health / maxHealth;
	if (healthPerc < 0) healthPerc = 0;
	int currentBarWidth = (int)(width * healthPerc);

	DrawRectangle(screen, x, y, width, height, czerwony, czerwony);

	if (currentBarWidth > 0) {
		DrawRectangle(screen, x, y, currentBarWidth, height, zielony, zielony);
	}

	Uint32 color = SDL_MapRGB(screen->format, 255, 255, 255); // Biały
	DrawLine(screen, x, y, width, 1, 0, color);           // Góra
	DrawLine(screen, x, y + height, width, 1, 0, color);  // Dół
	DrawLine(screen, x, y, height, 0, 1, color);          // Lewo
	DrawLine(screen, x + width, y, height + 1, 0, 1, color); // Prawo

}

void DrawStatSection(SDL_Surface* screen, SDL_Surface* charset, double worldTime, double fps, const char* nickname, int playerHealth, int enemy1Health, int enemy2Health, int score) {
	char text[256];

	DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 60, czerwony, niebieski);

	snprintf(text, sizeof(text), "Elapsed time = %.1lf s  %.0lf frames / s", worldTime, fps);
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);

	snprintf(text, sizeof(text), "Esc - exit");
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);

	snprintf(text, sizeof(text), "Player: %s", nickname);
	DrawString(screen, 30, 36, text, charset);

	int barX = 30 + strlen(text) * 8 + 10;
	int barY = 36 - 1;
	DrawHealthBar(screen, barX, barY, playerHealth, 1200, 200, 10);

	char hpText[32];
	snprintf(hpText, sizeof(hpText), "%d / %d", playerHealth > 0 ? playerHealth : 0, 1200);
	DrawString(screen, barX, barY + 10 + 4, hpText, charset);

	snprintf(text, sizeof(text), "Points: %d", score);
	DrawString(screen, 300 + 150, 36, text, charset);

	snprintf(text, sizeof(text), "Enemy Health: %d", enemy1Health);
	int textWidth = strlen(text) * 8;
	DrawString(screen, SCREEN_WIDTH - textWidth - 30, 36, text, charset);

	snprintf(text, sizeof(text), "Enemy2 Health: %d", enemy2Health);
	DrawString(screen, SCREEN_WIDTH - textWidth - 30, 48, text, charset);

	snprintf(text, sizeof(text), "Zaimplementowane wymagania: 1, 2, 3, 4, A, B, C, D, E, G, (I)");
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 50, text, charset);
}