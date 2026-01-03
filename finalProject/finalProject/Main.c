#define MINIAUDIO_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>
#include <string.h>
#include <conio.h>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 6031)
#pragma warning(disable : 4996)
//hehehehe
#include <SDL3/SDL.h>
#include "miniaudio.h"
#include "card.h"

const char* SYMBOLS[] = { "CHERRY", "BELL", "7", "STAR", "LEMON", "BAR", "EMPTY", "BOMB" };
const int SYMBOL_COUNT = sizeof(SYMBOLS) / sizeof(SYMBOLS[0]);

int rand_index(int max);
void spin_Reels(int reel[3]);
int calculate_payout(const int reel[], int bet);
void print_reels(const int reels[]);
void flush_input();
void show_help(int* _temp);

ma_engine global_engine;
int is_engine_init = 0;
ma_sound currentSound;
int is_sound_init = 0;

enum game { finish, win, lose };
enum game gamestate;
enum music { shuffle, casino, dealing_card, roulette1, slot_machine, none, win1, lose1 };
enum music scenemusic;

void music(void);
void music_stop(void);
void picture(void);
void total(int totall);
void endprent(int moneyy, int again);
int roulate(int num[38]);

int main(int argc, char* argv[]) {

#ifdef _WIN32
	system("chcp 65001 > nul");
#endif

	srand((unsigned int)time(NULL));
	gamestate = finish;
	card deck[52];
	card player[10];
	card dealer[10];
	int num, a = 0, item = 1;
	int playercard = 0, dealercard = 0;
	int playerScore = 0, dealerScore = 0;
	int money = 0, total_money = 0, again = 1;
	int roulette[38] = { 0,1,13,36,24,3,15,34,22,5,17,32,20,7,11,30,26,9,28,0,2,14,35,23,4,16,33,21,6,18,31,19,8,12,29,25,10,27 };
	const char* face[] = { "Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };
	const char* suit[] = { "Hearts", "Diamonds", "Clubs", "Spades" };

	SDL_Init(SDL_INIT_VIDEO);
	if (ma_engine_init(NULL, &global_engine) == MA_SUCCESS)
		is_engine_init = 1;

	fillDeck(deck, face, suit);
	shuffle_p(deck);
	scenemusic = casino;
	music();

	printf("how much money do you have? ");
	scanf(" %d", &total_money);
	printf("would you like to participate? Slot machine(2) blackjack(1) roulette(0) ");
	scanf(" %d", &item);

	if (item == 1) {
		music_stop();
		scenemusic = shuffle;
		music();
	}
bb:
	while (total_money >= 0 && again == 1 && item == 0)
	{
		printf("\n---roulette start ---");
		do {
			printf("\nhow much do you want to bet? ");
			scanf(" %d", &money);
			if (money <= 0 || money > total_money)
				printf("you dont have that munch money\n");
		} while (money <= 0 || money > total_money);

		int asw = 0, single;
		printf("even(0) or odd(1) or single bet(2)");
		scanf(" %d", &asw);

		if (asw == 2) {
			printf("which number do you want to bet on?(1-36) ");
			scanf(" %d", &single);
		}
		num = roulate(roulette);
		printf("\nthe number is %d\n", roulette[num]);

		if (asw == 2) {
			if (roulette[num] == single) {
				music_stop();
				printf("you win 35 times :) you win %d\n", money * 35);
				total_money += money * 35;
				total(total_money);
				scenemusic = win1; 
				music();
				Sleep(2000); 
				music_stop();
				gamestate = win; 
				picture();
			}
			else {
				music_stop();
				printf("you lose %d\n", money);
				total_money -= money;
				total(total_money);
				scenemusic = lose1; 
				music();
				Sleep(2000); 
				music_stop();
				gamestate = lose; 
				picture();
			}
		}
		else {
			music_stop();
			if ((roulette[num] % 2 == 0 && roulette[num] != 0 && asw == 0) || (roulette[num] % 2 == 1 && roulette[num] != 0 && asw == 1)) {
				printf("\nWin! you win %d\n", money);
				total_money += money;
				total(total_money);
				scenemusic = win1; 
				music();
				Sleep(2000); 
				music_stop();
				gamestate = win; 
				picture();
			}
			else {
				printf("\nyou lose %d\n", money);
				total_money -= money;
				total(total_money);
				scenemusic = lose1; 
				music();
				Sleep(2000); 
				music_stop();
				gamestate = lose; 
				picture();
			}
		}
		printf("\nDo you want to play again? yes(1) no(0) ");
		scanf(" %d", &again);
		scenemusic = casino; 
		music();
		if (total_money == 0) 
			break;
	}

	//black jack/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	while (total_money >= 0 && again == 1 && item == 1)
	{
		fillDeck(deck, face, suit);
		shuffle_p(deck);
		dealerScore = 0; playerScore = 0; dealercard = 0; playercard = 0;
		player[playercard++] = deck[a++];
		dealer[dealercard++] = deck[a++];
		player[playercard++] = deck[a++];
		dealer[dealercard++] = deck[a++];

		printf("\n---black jack start ---");
		do {
			printf("\nhow much do you want to bet? ");
			scanf(" %d", &money);
			if (money <= 0 || money > total_money)
				printf("you dont have that munch money\n");
		} while (money <= 0 || money > total_money);
		music_stop();
		scenemusic = dealing_card; 
		music();
		Sleep(2000);
		printf("dealer: [?], [%s of %s]\n", dealer[1].face, dealer[1].suit);
		while (1) {
			int choice = 0;
			Sleep(2000);
			printHand(player, playercard, "player");
			playerScore = calculate(player, playercard);

			if (playerScore > 21) {
				printf("player>21 dealer win,and you lose %d\n", money);
				total_money -= money;
				total(total_money);
				gamestate = lose;
				music_stop(); scenemusic = lose1; 
				music();
				Sleep(2000); 
				music_stop();
				picture();
				goto aa;
			}
			music_stop();
			printf("need(1) or stop(0) ");
			scanf(" %d", &choice);

			if (choice == 1) {
				player[playercard++] = deck[a++];
				scenemusic = dealing_card; 
				music();
			}
			else break;
		}

		printf("\n--- dealer round ---\n");
		printHand(dealer, dealercard, "dealer");
		Sleep(1000);
		while (calculate(dealer, dealercard) < 17) {
			dealer[dealercard++] = deck[a++];
			Sleep(1000);
			printHand(dealer, dealercard, "dealer");
		}

		playerScore = calculate(player, playercard);
		dealerScore = calculate(dealer, dealercard);
		printf("\nplayer:%d \ndealer: %d \n", playerScore, dealerScore);
		Sleep(2000);
		if (dealerScore > 21 || playerScore > dealerScore) {
			printf("player win :) you win %d\n", money);
			total_money += money;
			total(total_money);
			gamestate = win;
			music_stop(); 
			scenemusic = win1; 
			music();
			Sleep(2000); 
			music_stop();
			picture();
		}
		else if (playerScore < dealerScore) {
			printf("dealer win :( you lose %d\n", money);
			total_money -= money;
			total(total_money);
			gamestate = lose;
			music_stop(); 
			scenemusic = lose1; 
			music();
			Sleep(2000); 
			music_stop();
			picture();
		}
		else {
			music_stop(); 
			printf("Draw\n");
		}
	aa:
		printf("\nDo you want to play again? yes(1) no(0) ");
		scanf(" %d", &again);
		if (total_money == 0) 
			break;
		scenemusic = shuffle;
		music();
	}

	//Slot machine/////
	int _tmp = 1;
	while (total_money >= 0 && again == 1 && item == 2)
	{
		int reels[3];
		printf("\n\n=== Terminal Slot Machine ===\n");
		show_help(&_tmp);
		do {
			printf("\nhow much do you want to bet? ");
			scanf(" %d", &money);
			if (money <= 0 || money > total_money) {
				flush_input();
				printf("you dont have that munch money\n");
			}
		} while (money <= 0 || money > total_money);

		total_money -= money;
		spin_Reels(reels);
		printf("Enter any button to start the slot machine");
		getch();
		printf("\n\nSpinning...\n");
		music_stop(); 
		scenemusic = slot_machine; 
		music();
		Sleep(4000); 
		music_stop();
		print_reels(reels);
		int payout = calculate_payout(reels, money);
		total_money += payout;
		if (payout > 0) {
			printf("Congratulations! You won: %d\n", payout);
			scenemusic = win1; 
			music();
			Sleep(2000); 
			music_stop();
			gamestate = win; 
			picture();
		}
		else {
			printf("No win. You lost: %d\n", money);
			scenemusic = lose1; 
			music();
			Sleep(2000); 
			music_stop();
			gamestate = lose; 
			picture();
		}
		total(total_money);
		printf("\nDo you want to play again? yes(1) no(0) ");
		scanf(" %d", &again);
		scenemusic = casino; 
		music();
		if (total_money == 0) 
			break;
	}

	if (total_money != 0 && again == 0) {
		printf("you still have %d to play\n", total_money);
		printf("do you want play another item? Slot machine(2) black jack(1) roulette(0)");
		scanf(" %d", &item);
		if (item >= 0 && item <= 2) {
			again = 1;
			goto bb;
		}
	}
	endprent(total_money, again);
	music_stop();
	if (is_engine_init) ma_engine_uninit(&global_engine);
	SDL_Quit();
	system("pause");
	return 0;
}

void total(int totall) { printf("total money: %d\n", totall); }
void endprent(int money, int again) {
	if (money == 0 && again == 1) printf("you have no money to play\n");
	else printf("You have %d . welcome to come again next time!\n", money);
}
int roulate(int roulette[38]) {
	music_stop(); scenemusic = roulette1; music();
	int number = rand() % 38 + 114;
	for (int i = 0; i <= number; i++) {
		printf(" %d", roulette[i % 38]);
		if (i < 20) Sleep(20);
		else if (i < 40) Sleep(30);
		else if (i < 60) Sleep(40);
		else if (i < 80) Sleep(50);
		else if (i < 100) Sleep(70);
		else if (i - number < -10) Sleep(80);
		else if (i - number < -5) Sleep(150);
		else Sleep(200);
	}
	return number % 38;
}

void picture(void) {
#define WIN_W 800
#define WIN_H 600
	SDL_Window* window = SDL_CreateWindow("Result", WIN_W, WIN_H, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
	SDL_Surface* surface = (gamestate == win) ? SDL_LoadBMP("picture_happy.bmp") : SDL_LoadBMP("picture_sad.bmp");
	if (!surface) {
		SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window); return;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_DestroySurface(surface);
	float imgW, imgH;
	SDL_GetTextureSize(texture, &imgW, &imgH);
	SDL_FRect destRect = { (WIN_W - 400.0f) / 2.0f, (WIN_H - (400.0f * (imgH / imgW))) / 2.0f, 400.0f, 400.0f * (imgH / imgW) };
	Uint64 startTime = SDL_GetTicks();
	int running = 1;
	SDL_Event event;
	while (running && (SDL_GetTicks() - startTime < 2000)) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_KEY_DOWN) running = 0;
		}
		SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
		SDL_RenderClear(renderer);
		SDL_RenderTexture(renderer, texture, NULL, &destRect);
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	gamestate = finish;
}

void music(void) {
	if (!is_engine_init) return;
	if (is_sound_init) {
		ma_sound_stop(&currentSound); ma_sound_uninit(&currentSound); is_sound_init = 0;
	}
	if (scenemusic == none) return;
	const char* filePath = NULL;
	if (scenemusic == casino) 
		filePath = "casino.mp3";
	else if (scenemusic == shuffle) 
		filePath = "shuffle.mp3";
	else if (scenemusic == dealing_card) 
		filePath = "dealing_card.mp3";
	else if (scenemusic == roulette1) 
		filePath = "roulette1.mp3";
	else if (scenemusic == slot_machine) 
		filePath = "slot_machine.mp3";
	else if (scenemusic == win1) 
		filePath = "money.mp3";
	else if (scenemusic == lose1) 
		filePath = "sad.mp3";
	if (filePath && ma_sound_init_from_file(&global_engine, filePath, 0, NULL, NULL, &currentSound) == MA_SUCCESS) {
		if (scenemusic == casino) ma_sound_set_looping(&currentSound, MA_TRUE);
		ma_sound_start(&currentSound); is_sound_init = 1;
	}
}

void music_stop(void) { scenemusic = none; music(); }
int rand_index(int max) { 
	return rand() % max; 
}
void spin_Reels(int reel[3]) { 
	for (int i = 0; i < 3; i++) reel[i] = rand() % SYMBOL_COUNT; 
}

int calculate_payout(const int reel[], int bet) {
	int p1 = reel[0], p2 = reel[1], p3 = reel[2], payout = 0, emp = 0;
	for (int i = 0; i < 3; i++) 
		if (strcmp(SYMBOLS[reel[i]], "BOMB") == 0) 
			return 0;
	for (int i = 0; i < 3; i++) 
		if (strcmp(SYMBOLS[reel[i]], "EMPTY") == 0) 
			emp = 1;
	if ((p1 == p2 && p2 == p3) && !emp) 
		payout = (strcmp(SYMBOLS[p1], "7") == 0) 
		? bet * 10 : bet * 5;
	else if ((p1 == p2 || p2 == p3 || p3 == p1) && !emp) 
		payout = bet * 3;
	for (int i = 0; i < 3; i++) 
		if (strcmp(SYMBOLS[reel[i]], "STAR") == 0) 
			payout += bet;
	return payout;
}

void print_reels(const int reels[]) {
	printf("\n[ %s | %s | %s ]\n", SYMBOLS[reels[0]], SYMBOLS[reels[1]], SYMBOLS[reels[2]]);
}

void flush_input() { int ch; while ((ch = getchar()) != '\n' && ch != EOF); }

void show_help(int* _temp) {
	if (*_temp) {
		printf("\nRules: 3 same=*5 (7=*10) | 2 same=*3 | STAR=bonus | BOMB=Loss | EMPTY=Block\n");
		*_temp = 0;
	}
}