#define MINIAUDIO_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <ctype.h>
#include<windows.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 6031)
#pragma warning(disable : 4996)
#include <SDL3/SDL.h>
#include "miniaudio.h"
#include <string.h>
#include <conio.h>

const char* SYMBOLS[] = { "CHERRY", "BELL", "7", "STAR", "LEMON", "BAR", "EMPTY", "BOMB"};
const int SYMBOL_COUNT = sizeof(SYMBOLS) / sizeof(SYMBOLS[0]);

int rand_index(int max);
void spin_Reels(int reel[3]);
int calculate_payout(const int reel[], int bet);
void print_reels(const int reels[]);
void flush_input();
void show_help(int* _temp);

typedef struct _card {
	const char* face;
	const char* suit;
	int value;
} card;
ma_engine global_engine;
int is_engine_init = 0;
enum game { finish, win, lose };
enum game gamestate;
void music(void);
void music_stop(void);
enum music { shuffle, casino, dealing_card, roulette1, slot_machine, none, win1, lose1 }; //洗牌 賭場 發牌 輪盤
enum music scenemusic;
void fillDeck(card* wDeck, const char* wFace[], const char* wSuit[]);
void picture(void);
void shuffle_p(card* wDeck);
int calculate(card hand[], int cardCount);
void printHand(card hand[], int cardCount, const char* name);
void total(int totall);
void endprent(int moneyy, int again);
int roulate(int num[38]);
ma_sound currentSound;
int is_sound_init = 0;

int main(int argc, char* argv[]) {

#ifdef _WIN32
	system("chcp 65001 > nul");
#endif

	srand(time(NULL));
	gamestate = finish;
	card deck[52];
	card player[10];
	card dealer[10];
	int num;
	int a = 0;
	int item = 1;
	int playercard = 0;
	int dealercard = 0;
	int playerScore = 0;
	int dealerScore = 0;
	int money = 0;
	int total_money = 0;
	int again = 1;
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
		do
		{
			printf("\nhow much do you want to bet? ");
			scanf(" %d", &money);
			if (money <= 0 || money > total_money)
				printf("you dont have that munch money\n");
		} while (money <= 0 || money > total_money);

		int asw = 0;
		int single;

		printf("even(0) or odd(1) or single bet(2)");
		scanf(" %d", &asw);

		if (asw == 2)
		{
			printf("which number do you want to bet on?(1-36) ");
			scanf(" %d", &single);
		}
		num = roulate(roulette);
		printf("\nthe number is %d\n", roulette[num]);

		if (asw == 2)
		{
			if (roulette[num] == single)
			{
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
			else
			{
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
		else
		{
			music_stop();
			if (roulette[num] % 2 == 0 && roulette[num] != 0 && asw == 0)
			{
				printf("\neven number,you win 2 times :) you win %d\n", money);
				total_money += money;
				total(total_money);
				scenemusic = win1;
				music();
				Sleep(2000);
				music_stop();
				gamestate = win;
				picture();

			}
			else if (roulette[num] % 2 == 1 && roulette[num] != 0 && asw == 1)
			{
				printf("\nodd number,you win 2 times :) you win %d\n", money);
				total_money += money;
				total(total_money);
				scenemusic = win1;
				music();
				Sleep(2000);
				music_stop();
				gamestate = win;
				picture();

			}
			else
			{
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
		dealerScore = 0;
		playerScore = 0;
		dealercard = 0;
		playercard = 0;
		player[playercard++] = deck[a++];
		dealer[dealercard++] = deck[a++];
		player[playercard++] = deck[a++];
		dealer[dealercard++] = deck[a++];


		printf("\n---black jack start ---");

		do
		{
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
				music_stop();
				scenemusic = lose1;
				music();
				Sleep(2000);
				music_stop();
				picture();
				goto aa;
			}
			music_stop();

			printf("need(1) or stop(0) ");
			scanf(" %d", &choice);

			if (tolower(choice) == 1) {
				player[playercard++] = deck[a++];
				scenemusic = dealing_card;
				music();
			}
			else {
				break;
			}
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
		if (dealerScore > 21) {
			printf("dealer>21,and player win:) you win %d\n", money);
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
		else if (playerScore > dealerScore) {
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
	}
//Slot machine/////
int _tmp = 1;
	while (total_money >= 0 && again == 1 && item == 2)
	{
		int reels[3];
#ifdef _WIN32
		system("chcp 65001 > nul");
#endif

		srand((unsigned)time(NULL));
		printf("\n\n=== Terminal Slot Machine ===\n");
		show_help(&_tmp);
		do
		{
			printf("\nhow much do you want to bet? ");
			scanf(" %d", &money);
			if (money <= 0 || money > total_money)
			{
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
		if (payout > 0)
		{
			printf("Congratulations! You won: %d\n", payout);
			scenemusic = win1;
			music();
			Sleep(2000);
			music_stop();
			gamestate = win;
			picture();
		}
		else
		{
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

	if (total_money != 0 && again == 0)
	{
		printf("you still have %d to play\n", total_money);
		printf("do you want play another item? Slot machine(2) black jack(1) roulette(0)");
		scanf(" %d", &item);
		if (item == 0 || item == 1 ||item==2)
		{
			again = 1;
			goto bb;
		}
	}
	endprent(total_money, again);
	music_stop();
	if (is_engine_init)
		ma_engine_uninit(&global_engine);
	SDL_Quit();
	system("pause");
	return 0;
}




void fillDeck(card* wDeck, const char* wFace[], const char* wSuit[]) {
	for (int i = 0; i < 52; i++) {
		wDeck[i].face = wFace[i % 13];
		wDeck[i].suit = wSuit[i / 13];
		int val = (i % 13) + 1;
		if (val > 10) val = 10;
		if (val == 1) val = 11;
		wDeck[i].value = val;
	}
}

void shuffle_p(card* wDeck) {
	for (int i = 0; i < 52; i++) {
		int j = rand() % 52;
		card temp = wDeck[i];
		wDeck[i] = wDeck[j];
		wDeck[j] = temp;
	}
}
int calculate(card hand[], int cardCount) {
	int total = 0;
	int aceCount = 0;

	for (int i = 0; i < cardCount; i++) {
		total += hand[i].value;
		if (hand[i].value == 11) aceCount++;
	}
	while (total > 21 && aceCount > 0) {
		total -= 10;
		aceCount--;
	}
	return total;
}


void printHand(card hand[], int Count, const char* name) {
	printf("%s : ", name);
	for (int i = 0; i < Count; i++) {
		printf("[%s of %s] ", hand[i].face, hand[i].suit);
	}
	if (name == "dealer")
		printf("(%d)", calculate(hand, Count));
	printf("\n");
}
void endprent(int money, int again)
{
	if (money == 0 && again == 1)
		printf("you have no money to play\n");
	else
		printf("You have %d . welcome to come again next time!\n", money);
}
void total(int totall)
{

	printf("total money: %d\n", totall);
}
int roulate(int roulette[38])
{
	music_stop();
	scenemusic = roulette1;
	music();
	int number = rand() % 38 + 114;
	for (int i = 0; i <= number; i++)
	{
		printf(" %d", roulette[i % 38]);
		if (i < 20)
			Sleep(20);
		else if (i >= 20 && i < 40)
			Sleep(30);
		else if (i >= 40 && i < 60)
			Sleep(40);
		else if (i >= 60 && i < 80)
			Sleep(50);
		else if (i >= 80 && i < 100)
			Sleep(70);
		else if (i >= 100 && i - number < -10)
			Sleep(80);
		else if (i - number >= -10 && i - number < -5)
			Sleep(150);
		else
			Sleep(200);

	}
	return number % 38;
}

void picture(void) {
#define WIN_W 800
#define WIN_H 600
	SDL_Window* window = SDL_CreateWindow(
		"Result",
		WIN_W,
		WIN_H,
		0
	);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

	SDL_Surface* surface = NULL;
	if (gamestate == win)
		surface = SDL_LoadBMP("picture_happy.bmp");
	else
		surface = SDL_LoadBMP("picture_sad.bmp");

	if (!surface) {
		SDL_Log("圖片讀取失敗: %s", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		return;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_DestroySurface(surface);

	float imgW, imgH;
	SDL_GetTextureSize(texture, &imgW, &imgH);

	SDL_FRect destRect;
	destRect.w = 400.0f;
	destRect.h = destRect.w * (imgH / imgW);
	destRect.x = (WIN_W - destRect.w) / 2.0f;
	destRect.y = (WIN_H - destRect.h) / 2.0f;

	Uint64 startTime = SDL_GetTicks();
	int running = 1;
	SDL_Event event;
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	while (running) {
		Uint64 elapsed = SDL_GetTicks() - startTime;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT)
				running = 0;
			if (event.type == SDL_EVENT_KEY_DOWN &&
				(event.key.key == SDLK_RETURN ||
					event.key.key == SDLK_ESCAPE)) {
				running = 0;
			}
		}

		if (elapsed >= 2000)
			running = 0;

		SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
		SDL_RenderClear(renderer);

		SDL_RenderTexture(renderer, texture, NULL, &destRect);

		Uint8 fadeAlpha = 0;
		if (elapsed > 1500) {
			float fadeRatio = (float)(elapsed - 1500) / 500.0f;
			if (fadeRatio > 1.0f) fadeRatio = 1.0f;
			fadeAlpha = (Uint8)(fadeRatio * 255);
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, fadeAlpha);
		SDL_FRect fadeRect = { 0, 0, WIN_W, WIN_H };
		SDL_RenderFillRect(renderer, &fadeRect);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	gamestate = finish;
}

/*void picture(void) {		圖片不會自動關掉版本
	#define WIN_W 800
	#define WIN_H 600


	SDL_Window* window = SDL_CreateWindow("SDL3 Image Center Example", WIN_W, WIN_H, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
	SDL_Surface* surface = NULL;
	if (gamestate == win)
		surface = SDL_LoadBMP("picture_happy.bmp");
	else if (gamestate == lose)
		surface = SDL_LoadBMP("picture_sad.bmp");
	if (!surface) {
		SDL_Log("找不到圖片! 請確認 picture.bmp 在專案資料夾內: %s", SDL_GetError());
		return 1;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_DestroySurface(surface);

	float imgW, imgH;
	SDL_GetTextureSize(texture, &imgW, &imgH);

	SDL_FRect destRect;
	destRect.w = 400.0f;
	destRect.h = destRect.w * (imgH / imgW);

	destRect.x = (WIN_W - destRect.w) / 2.0f;
	destRect.y = (WIN_H - destRect.h) / 2.0f;

	int running = 1;
	SDL_Event event;
	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) running = 0;
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
}*/

void music(void) {
	if (!is_engine_init) return;

	if (is_sound_init) {
		ma_sound_stop(&currentSound);
		ma_sound_uninit(&currentSound);
		is_sound_init = 0;
	}

	if (scenemusic == none) return;

	const char* filePath = NULL;
	if (scenemusic == casino) filePath = "casino.mp3";
	else if (scenemusic == shuffle) filePath = "shuffle.mp3";
	else if (scenemusic == dealing_card) filePath = "dealing_card.mp3";
	else if (scenemusic == roulette1) filePath = "roulette1.mp3";
	else if (scenemusic == slot_machine) filePath = "slot_machine.mp3";
	else if (scenemusic == win1) filePath = "money.mp3";
	else if (scenemusic == lose1) filePath = "sad.mp3";

	if (filePath != NULL) {
		if (ma_sound_init_from_file(&global_engine, filePath, 0, NULL, NULL, &currentSound) == MA_SUCCESS) {
			if (scenemusic == casino) ma_sound_set_looping(&currentSound, MA_TRUE);

			ma_sound_start(&currentSound);
			is_sound_init = 1;
		}
	}
}

void music_stop(void) {
	scenemusic = none;
	music();
}

int rand_index(int max)
{
	return rand() % max;
}

void spin_Reels(int reel[3])
{
	int i;
	for (i = 0; i < 3; i++)
	{
		reel[i] = rand() % SYMBOL_COUNT;
	}
}

int calculate_payout(const int reel[], int bet)
{
	int pattern1 = reel[0], pattern2 = reel[1], pattern3 = reel[2];
	int payout = 0;
	int triple = ((pattern1 == pattern2) && (pattern2 == pattern3));
	int pair = ((pattern1 == pattern2) || (pattern2 == pattern3) || (pattern3 == pattern1));
	int i, emp = 0;

	for (i = 0; i < 3; i++)
	{
		if (strcmp(SYMBOLS[reel[i]], "BOMB") == 0)
		{
			printf("Boom! You hit a bomb!\n");
			return payout;
		}
	}
	for (i = 0; i < 3; i++)
	{
		if (strcmp(SYMBOLS[reel[i]], "EMPTY") == 0)
		{
			emp = 1;
		}
	}

	if (triple && (emp == 0))
	{
		if (strcmp(SYMBOLS[pattern1], "7") == 0)
		{
			payout = bet * 10;
		}
		else
		{
			payout = bet * 5;
		}
	}
	else if (
		pair && (
			((strcmp(SYMBOLS[pattern1], SYMBOLS[pattern2]) == 0) && (strcmp(SYMBOLS[pattern1], "EMPTY") != 0)) ||
			((strcmp(SYMBOLS[pattern3], SYMBOLS[pattern2]) == 0) && (strcmp(SYMBOLS[pattern3], "EMPTY") != 0)) ||
			((strcmp(SYMBOLS[pattern1], SYMBOLS[pattern3]) == 0) && (strcmp(SYMBOLS[pattern1], "EMPTY") != 0))
		)
	)
	{
		payout = bet * 3;
	}
	if (strcmp(SYMBOLS[pattern1], "STAR") == 0 || strcmp(SYMBOLS[pattern2], "STAR") == 0 || strcmp(SYMBOLS[pattern3], "STAR") == 0)
	{
		payout += bet;
	}
	return payout;
}

//void delay_print(const char* symbol)
//{
//	printf("%s", symbol);
//#ifdef _WIN32
//	Sleep(500); // 延遲 500 毫秒
//#else
//	usleep(500000); // 延遲 0.5 秒
//#endif
//}

void print_reels(const int reels[])
{
#ifdef _WIN32
	printf("[ %s | %s | %s ]\n", SYMBOLS[reels[0]], SYMBOLS[reels[1]], SYMBOLS[reels[2]]);
#else
	printf("┌───────────────┐\n");
	printf("│  %s  |  %s  |  %s  │\n", SYMBOLS[reels[0]], SYMBOLS[reels[1]], SYMBOLS[reels[2]]);
	printf("└───────────────┘\n");
#endif
}

void flush_input()
{
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF) {}
}

void show_help(int *_temp)
{
	if (*_temp==1)
	{
		printf("\nGame Rules:\n");
		printf(" - Three of the same symbol: *5 (if '7' then *10)\n");
		printf(" - Two of the same symbol:   *3\n");
		printf(" - Any 'STAR' symbol: bonus + bet\n");
		printf(" - Any 'BOMB' symbol: no bet!\n");
		printf(" - Any 'EMPTY' symbol: Unable to pair occupied positions\n\n");
		*_temp = 0;
	}
}
