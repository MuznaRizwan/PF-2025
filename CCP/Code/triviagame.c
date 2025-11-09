/**
 * TRIVIA.EXE - Console Trivia Game
 *
 * Features:
 * - 10 Questions across 4 categories
 * - Double or Nothing round at Q5
 * - Speed Duel round at Q8
 * - Settings (Sound, Mic, Language)
 * - Leaderboard
 * - Console glowing aesthetic
 *
 * Color Palette:
 * - Background: #0D0D0D (Soft Black)
 * - Primary: #00E0A1 (Aqua Mint)
 * - Secondary: #FFD369 (Soft Golden)
 * - Accent: #8B5CF6 (Muted Violet)
 * - Text: #EDEDED (Soft White)
 * - Muted: #AAAAAA (Gray)
 * - Error: #FF6B6B (Soft Red)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#define CLEAR "cls"
#else
#include <unistd.h>
#define CLEAR "clear"
#endif

#include <mmsystem.h>
#include "game.h"

// Global variables
Settings settings = {1, 0, "ENGLISH"};
LanguageStrings langStrings = {
	"START GAME",
	"LEADERBOARD",
	"SETTINGS",
	"EXIT",
	"SELECT OPTION:",
	"ℹ  10 QUESTIONS • 4 CATEGORIES • SPEED DUEL @ Q8",
	"> INITIALIZING KNOWLEDGE PROTOCOL...",
	0
};

void updateLanguage() {
	if (strcmp(settings.language, "URDU") == 0) {
		strcpy(langStrings.startGame, "کھیل شروع کریں");
		strcpy(langStrings.leaderboard, "لیڈر بورڈ");
		strcpy(langStrings.settings, "ترتیبات");
		strcpy(langStrings.exit, "باہر نکلیں");
		strcpy(langStrings.selectOption, "اختیار منتخب کریں:");
		strcpy(langStrings.questionsInfo, "10 سوالات • 4 زمرے • تیز مقابلہ @ س8");
		strcpy(langStrings.initializingProtocol, "> علم کا نظام شروع ہو رہا ہے...");
		langStrings.isRTL = 1;
	} else {
		strcpy(langStrings.startGame, "START GAME");
		strcpy(langStrings.leaderboard, "LEADERBOARD");
		strcpy(langStrings.settings, "SETTINGS");
		strcpy(langStrings.exit, "EXIT");
		strcpy(langStrings.selectOption, "SELECT OPTION:");
		strcpy(langStrings.questionsInfo, "ℹ  10 QUESTIONS • 4 CATEGORIES • SPEED DUEL @ Q8");
		strcpy(langStrings.initializingProtocol, "> INITIALIZING KNOWLEDGE PROTOCOL...");
		langStrings.isRTL = 0;
	}
}

void playSound(const char* soundFile) {
	if (!settings.sound) {
		return; // Don't play if sound is disabled
	}

#ifdef _WIN32
	PlaySound(TEXT(soundFile), NULL, SND_FILENAME | SND_ASYNC);
#endif
}

size_t utf8_strlen(const char *s) {
	size_t len = 0;
	while (*s) {
		if ((*s & 0xC0) != 0x80)  // Count only bytes that are not continuation bytes
			len++;
		s++;
	}
	return len;
}

// Main function
int main() {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif

#ifdef _WIN32
	// Enable ANSI colors on Windows
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
#endif

	srand(time(NULL));

	while(1) {
		showStartScreen();
	}

	return 0;
}

void clearScreen() {
	system(CLEAR);
}

void delay(int milliseconds) {
#ifdef _WIN32
	Sleep(milliseconds);
#else
	usleep(milliseconds * 1000);
#endif
}

// Function to load questions from file-file management
int loadQuestions(Question q[], const char *filename) {
	//pointer use
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		return 0;
	}

	int i = 0;
	while (i < MAX_QUESTIONS && !feof(fp)) {
		fgets(q[i].question, sizeof(q[i].question), fp);
		// if (strlen(q[i].question) < 2) break;

		fgets(q[i].options[0], sizeof(q[i].options[0]), fp);
		fgets(q[i].options[1], sizeof(q[i].options[1]), fp);
		fgets(q[i].options[2], sizeof(q[i].options[2]), fp);
		fgets(q[i].options[3], sizeof(q[i].options[3]), fp);

		fscanf(fp, " %c\n", &q[i].correctAnswer);
		fgets(q[i].hint, sizeof(q[i].hint), fp);

		char separator[10];
		fgets(separator, sizeof(separator), fp); // read the "---" line

		i++;
	}

	fclose(fp);
	return i;
}

int wrapText(const char *text, char lines[MAX_LINES][MAX_WIDTH + 1], int width) {
	int len = strlen(text);
	int start = 0;
	int count = 0;

	while (start < len && count < MAX_LINES) {
		int end = start + width;
		if (end > len) end = len;

		// find last space before wrap width
		if (end < len && text[end] != ' ') {
			int lastSpace = end;
			while (lastSpace > start && text[lastSpace] != ' ')
				lastSpace--;
			if (lastSpace > start)
				end = lastSpace;
		}

		// extract line
		int lineLen = end - start;
		strncpy(lines[count], text + start, lineLen);
		lines[count][lineLen] = '\0';

		// skip extra spaces
		while (text[end] == ' ') end++;

		start = end;
		count++;
	}

	return count; // total number of lines
}

void printBorder(const char* color, int width) {
	printf("%s", color);
	for(int i = 0; i < width; i++) {
		printf("═");
	}
	printf("%s\n", RESET);
}

void printCentered(const char* text, const char* color) {
	int l = utf8_strlen(text);
	int padding = (80 - l) / 2;
	for(int i = 0; i < padding; i++) printf(" ");
	printf("%s%s%s\n", color, text, RESET);
}

void printMenuOption(int number, char* text, const char* color) {
	if (langStrings.isRTL) {
		// Right-aligned for RTL languages
		int textLen = utf8_strlen(text);
		int padding = 75 - textLen - 5; // 5 for "[X] "
		printf("    ");
		for(int i = 0; i < padding; i++) printf(" ");
		
		reverse_utf8(text);
		printf("%s[%d] %s%s\n", color, number, text, RESET);
	} else {
		// Left-aligned for LTR languages
		printf("    %s%s[%d] %s%s\n", BOLD, color, number, text, RESET);
	}
}

void printGlow(const char* text, const char* color) {
	printf("%s%s%s%s", BOLD, color, text, RESET);
}

void animateText(const char* text, const char* color) {
	printf("%s", color);
	for(int i = 0; text[i] != '\0'; i++) {
		printf("%c", text[i]);
		fflush(stdout);
		delay(30);
	}
	printf("%s", RESET);
}

void printBox(const char* content, const char* color) {
	int len = utf8_strlen(content);
	printf("%s┌", color);
	for(int i = 0; i < len + 2; i++) printf("─");
	printf("┐\n");
	printf("│ %s │\n", content);
	printf("└");
	for(int i = 0; i < len + 2; i++) printf("─");
	printf("┘%s\n", RESET);
}

void showStartScreen() {
	clearScreen();

	printf("\n\n");
	printBorder(AQUA, 80);
	printf("\n");

	// Logo
	printCentered("███████████████████████████████", AQUA);
	printCentered("█                             █", AQUA);
	printCentered("█         T R I V I A         █", AQUA);
	printCentered("█                             █", AQUA);
	printCentered("███████████████████████████████", AQUA);

	printf("\n");
	printCentered("> INITIALIZING KNOWLEDGE PROTOCOL...", GRAY);
	printf("\n\n");

	printBorder(AQUA, 80);
	printf("\n");


	// Menu options
	printMenuOption(1, langStrings.startGame, AQUA);
	printMenuOption(2, langStrings.leaderboard, GOLD);
	printMenuOption(3, langStrings.settings, VIOLET);
	printMenuOption(4, langStrings.exit, RED);

	printf("\n");
	printBorder(AQUA, 80);
	printf("\n");

	// Info
	printf("%s    ℹ  10 QUESTIONS • 4 CATEGORIES • SPEED DUEL @ Q8\n", GRAY);
	printf("       DOUBLE OR NOTHING @ Q5 • LEADERBOARD OVERLAY%s\n\n", RESET);

	printf("%s    > SELECT OPTION: %s", WHITE, RESET);

	char choice;
	scanf(" %c", &choice);
	getchar(); // Clear newline
	//switch case
	switch(choice) {
		case '1':
			showSubjectSelection();
			break;
		case '2':
			showLeaderboard();
			break;
		case '3':
			showSettings();
			break;
		case '4':
			clearScreen();
			printf("\n%s    Thanks for playing TRIVIA!%s\n\n", GOLD, RESET);
			exit(0);
		default:
			printf("%s    Invalid option! Press Enter to continue...%s", RED, RESET);
			getchar();
	}
}

void showSettings() {
	clearScreen();

	printf("\n\n");
	printBorder(VIOLET, 80);
	printf("\n");

	printGlow("    > SETTINGS\n\n", VIOLET);
	printf("%s    CONFIGURE SYSTEM PARAMETERS%s\n\n", GRAY, RESET);

	printBorder(VIOLET, 80);
	printf("\n");

	// Sound setting
	printf("    %s┌─────────────────────────────────────────┐%s\n", AQUA, RESET);
	printf("    %s│%s  🔊 SOUND EFFECTS: %s%-20s%s│%s\n",
	       AQUA, WHITE,
	       settings.sound ? AQUA : RED,
	       settings.sound ? "ENABLED" : "DISABLED",
	       AQUA, RESET);
	printf("    %s└─────────────────────────────────────────┘%s\n\n", AQUA, RESET);

	// Language setting
	printf("    %s┌─────────────────────────────────────────┐%s\n", GOLD, RESET);
	printf("    %s│%s  🌐 LANGUAGE: %s%-27s%s│%s\n",
	       GOLD, WHITE, GOLD, settings.language, GOLD, RESET);
	printf("    %s└─────────────────────────────────────────┘%s\n\n", GOLD, RESET);

	printBorder(VIOLET, 80);
	printf("\n");

	printf("    %s[1] Toggle Sound   [2] Change Language   [3] Back%s\n\n", GRAY, RESET);
	if (langStrings.isRTL) {
		int textLen = utf8_strlen(langStrings.selectOption);
		int padding = 75 - textLen;
		printf("%s", WHITE);
		for(int i = 0; i < padding; i++) printf(" ");
		printf("%s < %s", langStrings.selectOption, RESET);
	} else {
		printf("%s    > %s %s", WHITE, langStrings.selectOption, RESET);
	}

	char choice;
	scanf(" %d", &choice);
	getchar();
	//switchcase2
	switch(choice) {
		case 1:
			settings.sound = !settings.sound;
			showSettings();
			break;
		case 2:
			printf("\n    %sLanguages: [1] ENGLISH  [2] URDU \n", GRAY, RESET);
			printf("    > Select: ");
			char lang;
			scanf(" %d", &lang);
			getchar();
			switch(lang) {
				case 1:
					strcpy(settings.language, "ENGLISH");
					break;
				case 2:
					strcpy(settings.language, "URDU");
					break;
			}
			updateLanguage();
			showSettings();
			break;
		case 3:
			showStartScreen();
			break;
		default:
			return;
	}
}

void showLeaderboard() {
	clearScreen();

	FILE *fp = fopen("leaderboard.dat", "rb");
	LeaderboardEntry entries[1000];
	int count = 0;
	char time_str[26];

	if (fp == NULL) {
		printf("No leaderboard data found!\n");
		return;
	}

	while (fread(&entries[count], sizeof(LeaderboardEntry), 1, fp) == 1) {
		count++;
	}
	fclose(fp);

	if (count == 0) {
		printf("Leaderboard is empty!\n");
		return;
	}

	qsort(entries, count, sizeof(LeaderboardEntry), compareScores);

	printf("\n\n");
	printBorder(GOLD, 80);
	printf("\n");

	printGlow("    > LEADERBOARD\n\n", GOLD);
	printf("%s    TOP PERFORMERS • ALL TIME%s\n\n", GRAY, RESET);

	printBorder(GOLD, 80);
	printf("\n");

	// Header
	printf("    %sRANK    PLAYER              SCORE           ACCURACY%s\n", GRAY, RESET);
	printf("    %s─────────────────────────────────────────────────────────%s\n\n", GRAY, RESET);

	// LeaderboardEntry leaderboard[5] = {
	// 	{ "Muzna", "English", 1100, 10, 0, 200, 100 },
	// 	{ "Areefa", "General Knowledge", 1000, 10, 0, 210, 100 },
	// 	{ "Zainab", "Maths", 900, 10, 0, 220, 90 },
	// 	{ "Aasiya", "Geography", 800, 10, 0, 260, 80 },
	// 	{ "Aatiqa", "English", 500, 10, 0, 500, 50 },
	// };
	// Entries-array use
	const char* medals[] = {"🏆", "🥈", "🥉", "  ", "  "};
	//for loop
	for(int i = 0; i < count; i++) {
		const char* color = (i == 0) ? GOLD : WHITE;
		printf("    %s%-2s %d    %-20s%s%-16d%s%d%%%s\n",
		       color,
		       medals[i],
		       i + 1,
		       entries[i].name,
		       AQUA,
		       entries[i].score,
		       VIOLET,
		       entries[i].accuracy,
		       RESET);
	}

	printf("\n");
	printBorder(GOLD, 80);
	printf("\n");

	printf("%s    Press Enter to return to menu...%s", GRAY, RESET);
	getchar();
}

#ifdef _WIN32
char getAnswerWithTimeout(int seconds, int *timedOut) {
	char answer = 0;
	*timedOut = 0;

	printf("\n%s    > YOUR ANSWER (1-4): %s", WHITE, RESET);
	fflush(stdout);

	time_t startTime = time(NULL);

	while (difftime(time(NULL), startTime) < seconds) {
		int remaining = seconds - (int)difftime(time(NULL), startTime);
		printf("\r%s    > YOUR ANSWER (1-4) [%d sec]: %s", WHITE, remaining, RESET);
		fflush(stdout);

		if (_kbhit()) {
			answer = _getch();
			printf("\r%s    > YOUR ANSWER (1-4): %c        %s\n", WHITE, answer, RESET);
			return answer;
		}

		delay(100);
	}

	*timedOut = 1;
	printf("\r%s    > TIME'S UP!                    %s\n", RED, RESET);
	return 0;
}
#else
char getAnswerWithTimeout(int seconds, int *timedOut) {
	char answer = 0;
	*timedOut = 0;

	struct termios oldSettings, newSettings;
	tcgetattr(STDIN_FILENO, &oldSettings);
	newSettings = oldSettings;
	newSettings.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);

	int oldFlags = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldFlags | O_NONBLOCK);

	printf("\n%s    > YOUR ANSWER (1-4): %s", WHITE, RESET);
	fflush(stdout);

	time_t startTime = time(NULL);

	while (difftime(time(NULL), startTime) < seconds) {
		int remaining = seconds - (int)difftime(time(NULL), startTime);
		printf("\r%s    > YOUR ANSWER (1-4) [%d sec]: %s", WHITE, remaining, RESET);
		fflush(stdout);

		char c;
		if (read(STDIN_FILENO, &c, 1) > 0) {
			if (c >= '1' && c <= '4') {
				answer = c;
				printf("\r%s    > YOUR ANSWER (1-4): %c        %s\n", WHITE, answer, RESET);
				break;
			}
		}

		usleep(100000);
	}

	if (answer == 0) {
		*timedOut = 1;
		printf("\r%s    > TIME'S UP!                    %s\n", RED, RESET);
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
	fcntl(STDIN_FILENO, F_SETFL, oldFlags);

	return answer;
}
#endif

void playGame(Question questions[MAX_QUESTIONS], int total, int* pScore, int* pAccuracy) {
	int score = 0;
	int currentQuestion = 0;
	int maxScore = SCORE_PER_QUESTION * total;
	int correct = 0;
	int hintsUsed = 0;
	int hintQuestion = -1;

	// Question questions[MAX_QUESTIONS];

	// char playerName[256];
	// char subjectName[256];
	// char filename[256];
	// int choice = -1;

	// printf("\nEnter your name: ");
	// //fgets(playerName, 255, stdin);
	// playerName[strcspn(playerName, "\n")] = 0; // remove newline

	// printf(YELLOW "Choose a Subject:\n" RESET);
	// printf("1. English\n2. Mathematics\n3. General Knowledge\n4. Geography\n");
	// printf("\nEnter your choice (1-4): ");
	// scanf("%d", &choice);
	// int c;
	// while ((c = getchar()) != '\n' && c != EOF);

	// switch (choice) {
	// 	case 1:
	// 		strcpy(subjectName, "English");
	// 		strcpy(filename, "english.txt");
	// 		break;
	// 	case 2:
	// 		strcpy(subjectName, "Mathematics");
	// 		strcpy(filename, "maths2.txt");
	// 		break;
	// 	case 3:
	// 		strcpy(subjectName, "General Knowledge");
	// 		strcpy(filename, "gk.txt");
	// 		break;
	// 	case 4:
	// 		strcpy(subjectName, "Geography");
	// 		strcpy(filename, "geography.txt");
	// 		break;
	// 	default:
	// 		printf(RED "Invalid choice! Try again.\n" RESET);
	// 		// continue;
	// }

	// int total = loadQuestions(questions, filename);

	//while loop
	while(currentQuestion < total) {
		clearScreen();

		Question q = questions[currentQuestion];
		int isDoubleEdge = (currentQuestion == DOUBLE_EDGE_QUESTION);
		int isSpeedDuel = (currentQuestion == SPEED_DUEL_QUESTION);

		printf("\n\n");
		printBorder(AQUA, 80);
		printf("\n");

		// Special round banner
		if(isDoubleEdge) {
			maxScore += SCORE_PER_QUESTION;
			printf("\n");
			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", GOLD);
			printCentered("⚠  DOUBLE EDGE ROUND!  ⚠", GOLD);
			printCentered("Wrong answer = -100 points!", GOLD);
			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", GOLD);
			printf("\n");
		} else if(isSpeedDuel) {
			printf("\n");
			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", RED);
			printCentered("⚡ SPEED DUEL ⚡", RED);
			printCentered("YOU HAVE 10 SECONDS TO ANSWER!", RED);
			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", RED);
			printf("\n");
		}

		// Header
		printf("    %s🏆 SCORE: %s%d/%d%s        ", WHITE, AQUA, score, maxScore, WHITE);
//        printf("%s[%s] %s%s\n\n", VIOLET, q.category, WHITE, RESET);

		// Progress
		int progress = ((currentQuestion + 1) * 100) / 10;
		printf("    %s> QUESTION %d OF 10                                    %s\n",
		       GRAY, currentQuestion + 1, RESET);
		printf("    %s[", AQUA);
		int barWidth = 50;
		int filled = (progress * barWidth) / 100;
		for(int i = 0; i < barWidth; i++) {
			//if-else statement
			if(i < filled) printf("█");
			else printf("░");
		}
		printf("] %d%%%s\n\n", progress, RESET);

		printBorder(AQUA, 80);
		printf("\n");

		// Question
		//printWrappedBox(q.question);

		char lines[MAX_LINES][MAX_WIDTH];
		int li=utf8_strlen(q.question);
		if (li>0 && q.question[li-1]=='\n') q.question[li-1]='\0';
		int lineCount = splitText(q.question, lines);
//		int total1 = wrapText(q.question, lines, 70);
		printf("    %s┌────────────────────────────────────────────────────────────────────────┐%s\n", AQUA, RESET);
		for(int i=0; i<lineCount; i++) {
			printf("    %s│%s %s %-69s%s│%s\n", AQUA, WHITE, i==0 ? ">": " ", lines[i], AQUA, RESET);
		}
		printf("    %s└────────────────────────────────────────────────────────────────────────┘%s\n\n", AQUA, RESET);

		// Options
		for(int i = 0; i < 4; i++) {
			char option = 'A' + i;
			li=utf8_strlen(q.options[i]);
			if (li>0 && (q.options[i][li-1]=='\n' || q.options[i][li-1]=='\r')) q.options[i][li-1]='\0';
			printf("    %s┌────────────────────────────────────────────────────────────────────────┐%s\n", GRAY, RESET);
			printf("    %s│%s [%d] %-67s%s│%s\n", GRAY, WHITE, i+1, q.options[i], GRAY, RESET);
			printf("    %s└────────────────────────────────────────────────────────────────────────┘%s\n", GRAY, RESET);
			if(i < 3) printf("\n");
		}

		printf("\n");
		printBorder(AQUA, 80);
		printf("\n");

		char answerChar;
		int timedOut = 0;

		if(isSpeedDuel) {
			answerChar = getAnswerWithTimeout(10, &timedOut);
		} else {
			printf("\n%s    > YOUR ANSWER (1-4): %s", WHITE, RESET);
			scanf(" %c", &answerChar);
			getchar();
		}
		//	fgets(answer,sizeof(answer),stdin);
//		getchar();

		//	answer = toupper(answer);
//		int answerIndex = q.correctAnswer-'1' ;
//
//		// Check answer
//		printf("\n");
//		if(answer[0] == q.correctAnswer) {
//			int points = isDoubleEdge ? 2 : 1;
//			score += points;
//
//			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━", AQUA);
//			char msg[100];
//			sprintf(msg, "✓ CORRECT! +%d POINTS", points * 100);
//			printCentered(msg, AQUA);
//			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━", AQUA);
//		} else {
//			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", RED);
//			printCentered("✗ INCORRECT!", RED);
//			char msg[200];
//			sprintf(msg, "CORRECT ANSWER: %s", q.options[answerIndex]);
//			printCentered(msg, RED);
//			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", RED);
//		}
//
//		printf("\n%s    Press Enter to continue...%s", GRAY, RESET);
//		getchar();
//		int c;
//		while ((c = getchar()) != '\n' && c != EOF);
//
//		currentQuestion++;
// answer = toupper(answer);
		int answerIndex = q.correctAnswer - '1';

		printf("\n");

		if(timedOut) {
			playSound("dong.wav"); // Play wrong sound
			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", RED);
			printCentered("⏰ TIME'S UP! NO POINTS!", RED);
			char msg[200];
			sprintf(msg, "CORRECT ANSWER: %s", q.options[answerIndex]);
			printCentered(msg, RED);
			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", RED);
		} else if (toupper(answerChar) == 'H') {
			if (hintsUsed < 3) {
				char msg[200];
				sprintf(msg, "Hint: %s", q.hint);
				printCentered(msg, BLUE);
				printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", RED);
				if (hintQuestion != currentQuestion) {
					hintQuestion = currentQuestion;
					hintsUsed++;
				}
			} else {
				char msg[200];
				printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", RED);
				printCentered("No hints left.", VIOLET);
				printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", RED);
			}
		} else if (answerChar == q.correctAnswer) {
			playSound("CORRECT ANSWER SOUND EFFECt.wav"); // Play correct sound
			int points = SCORE_PER_QUESTION * (isDoubleEdge ? 2 : 1);
			score += points;
			correct++;

			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", AQUA);
			char msg[100];
			sprintf(msg, "✓ CORRECT! +%d POINTS", points);
			printCentered(msg, AQUA);
			if(isSpeedDuel) {
				printCentered("⚡ LIGHTNING FAST! ⚡", AQUA);
			}
			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", AQUA);
		} else {
			playSound("mixkit-game-show-wrong-answer-buzz-950.wav"); // Play wrong sound
			int penalty = SCORE_PER_QUESTION * (isDoubleEdge ? -1 : 0);
			score += penalty;

			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", RED);
			printCentered("✗ INCORRECT!", RED);
			char msg[200];
			sprintf(msg, "CORRECT ANSWER: %s", q.options[answerIndex]);
			printCentered(msg, RED);
			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", RED);

			if (isDoubleEdge && penalty < 0) {
				char penaltyMsg[100];
				sprintf(penaltyMsg, "−%d POINTS", -penalty);
				printCentered(penaltyMsg, RED);
			}
		}

		printf("\n%s    Press Enter to continue...%s", GRAY, RESET);
		getchar();

		if (toupper(answerChar) != 'H') currentQuestion++;
	}

	showGameOver(correct, total, score, maxScore);

	*pScore = score;
	*pAccuracy = (correct * 100) / total;
}

//void isSpeedDuel(){
//    char answer[10];
//    fd_set inputSet;
//    struct timeval timeout;
//
//    // Watch stdin
//    FD_ZERO(&inputSet);
//    FD_SET(STDIN_FILENO, &inputSet);
//
//    int totalTime = 10;
//    int answered = 0;
//
//    for (int t = totalTime; t >= 0; t--) {
//        printf("\rTime left: %2d ", t);
//        fflush(stdout);
//
//        // Set timeout to 1 second
//        timeout.tv_sec = 1;
//        timeout.tv_usec = 0;
//
//        // Wait for input for 1 second
//        int ready = select(STDIN_FILENO + 1, &inputSet, NULL, NULL, &timeout);
//
//        if (ready > 0) {
//            fgets(answer, sizeof(answer), stdin);
//            answer[strcspn(answer, "\n")] = 0; // remove newline
//            answered = 1;
//            break;
//        }
//    }
//
//    printf("\n"); // move to next line after countdown
//
//    if (answered) {
//        if (answer[0] == 'B') { // example correct answer
//            printf("✓ CORRECT!\n");
//        } else {
//            printf("✗ INCORRECT!\n");
//        }
//    } else {
//        printf("⏰ TIME’S UP! You didn’t answer in time.\n");
//    }
//}

void showGameOver(int correct, int total, int score, int maxScore) {
	clearScreen();
	//mathematical calculations
	int percentage = (correct * 100) / total;
	int finalScore = score;
	int stars;
	const char* message;
	//if-else ladder
	if(percentage == 100) {
		stars = 5;
		message = "PERFECT EXECUTION";
	} else if(percentage >= 80) {
		stars = 4;
		message = "EXCELLENT PERFORMANCE";
	} else if(percentage >= 60) {
		stars = 3;
		message = "GOOD WORK";
	} else if(percentage >= 40) {
		stars = 2;
		message = "KEEP PRACTICING";
	} else {
		stars = 1;
		message = "TRY AGAIN";
	}
	//Escape sequence-newline character
	printf("\n\n\n");
	printBorder(GOLD, 80);
	printf("\n");

	// Trophy-pattern printing(back to basics)
	printCentered("        ___________        ", GOLD);
	printCentered("       '._==_==_=_.'       ", GOLD);
	printCentered("       .-\\:      /-.       ", GOLD);
	printCentered("      | (|:.     |) |      ", GOLD);
	printCentered("       '-|:.     |-'       ", GOLD);
	printCentered("         \\::.    /         ", GOLD);
	printCentered("          '::. .'          ", GOLD);
	printCentered("            ) (            ", GOLD);
	printCentered("          _.' '._          ", GOLD);

	printf("\n");
	// printGlow("                         GAME COMPLETE\n", GOLD);
	printCentered("GAME COMPLETE\n", GOLD);
	printf("\n");
	printCentered(message, GRAY);
	printf("\n");

	// Stars
	printf("                                   ");
	for(int i = 0; i < 5; i++) {
		if(i < stars) {
			printf("%s★%s ", GOLD, RESET);
		} else {
			printf("%s☆%s ", GRAY, RESET);
		}
	}
	printf("\n\n");

	printBorder(GOLD, 80);
	printf("\n");

	// Score display
	//string use
	printf("    %s┌──────────────────────────────────────────────────────────────────────┐%s\n", AQUA, RESET);
	printf("    %s│                                                                      │%s\n", AQUA, RESET);
	printf("    %s│%s                             FINAL SCORE                              %s│%s\n", AQUA, GRAY, AQUA, RESET);
	printf("    %s│%s                                 %s%-10d%s                           %s│%s\n",
	       AQUA, WHITE, AQUA, finalScore, WHITE, AQUA, RESET);
	printf("    %s│                                                                      │%s\n", AQUA, RESET);
	printf("    %s│%s               CORRECT: %s%d/%d%s            ACCURACY: %s%d%%%s                 %s│%s\n",
	       AQUA, WHITE, AQUA, correct, total, WHITE, VIOLET, percentage, WHITE, AQUA, RESET);
	printf("    %s│                                                                      │%s\n", AQUA, RESET);
	printf("    %s└──────────────────────────────────────────────────────────────────────┘%s\n", AQUA, RESET);

	printf("\n");
	printBorder(GOLD, 80);
	printf("\n");

	printf("    %s[R] PLAY AGAIN        [M] MAIN MENU%s\n\n", GRAY, RESET);
	printf("%s    > SELECT OPTION: %s", WHITE, RESET);

	char choice;
	scanf(" %c", &choice);
	//input statement variation
	getchar();

	choice = toupper(choice);

	if(choice == 'R') {
		showSubjectSelection();
	}
	// Otherwise return to main menu
}

int splitText(char text[256], char lines[MAX_LINES][MAX_WIDTH]) {
	int lineCount = 0;
	char *word = strtok(text, " "); // split by space
	char line[MAX_WIDTH + 1] = "";

	while (word != NULL) {
		// Check if adding the word exceeds MAX_LEN
		if (strlen(line) + utf8_strlen(word) + (utf8_strlen(line) > 0 ? 1 : 0) < MAX_WIDTH) {
			if (strlen(line) > 0) strcat(line, " ");
			strcat(line, word);
		} else {
			// Save current line and start a new one
			strcpy(lines[lineCount++], line);
			strcpy(line, word);
		}
		word = strtok(NULL, " ");
	}
	// Save last line if any
	if (strlen(line) > 0) {
		strcpy(lines[lineCount++], line);
	}
	return lineCount;
}

void showSubjectSelection() {
	clearScreen();

	char subjectName[50];
	char filename[100];
	int choice;
	time_t start_time, end_time;


	printf("\n\n");
	printBorder(AQUA, 80);
	printf("\n");

	printGlow("    > PLAYER SETUP\n\n", AQUA);
	printf("%s    ENTER YOUR DETAILS TO BEGIN%s\n\n", GRAY, RESET);

	printBorder(AQUA, 80);
	printf("\n");

	// Player name input
	printf("\n    %s> Enter your name: %s", WHITE, RESET);

	char playerName[256];
	fgets(playerName, 255, stdin);
	playerName[strcspn(playerName, "\n")] = 0; // remove newline

	if(strlen(playerName) == 0) {
		strcpy(playerName, "PLAYER");
	}

	clearScreen();

//	printf("\n");
//	printBorder(AQUA, 80);
	printf("\n");

	printf("    %s┌────────────────────────────────────────────────────────────────────────┐%s\n", AQUA, RESET);
	printf("    %s│%s  PLAYER NAME: %-50s                                                        %s│%s\n", AQUA, WHITE, playerName, AQUA, RESET);
	printf("    %s└────────────────────────────────────────────────────────────────────────┘%s\n", AQUA, RESET);

	// Subject selection
	printGlow("    > SELECT SUBJECT\n\n", GOLD);

	printf("    %s┌────────────────────────────────────────────────────────────────────────┐%s\n", AQUA, RESET);
	printf("    %s│%s  [1] 📚 ENGLISH                                                        %s│%s\n", AQUA, WHITE, AQUA, RESET);
	printf("    %s└────────────────────────────────────────────────────────────────────────┘%s\n\n", AQUA, RESET);

	printf("    %s┌────────────────────────────────────────────────────────────────────────┐%s\n", GOLD, RESET);
	printf("    %s│%s  [2] ➗ MATHEMATICS                                                    %s│%s\n", GOLD, WHITE, GOLD, RESET);
	printf("    %s└────────────────────────────────────────────────────────────────────────┘%s\n\n", GOLD, RESET);

	printf("    %s┌────────────────────────────────────────────────────────────────────────┐%s\n", AQUA, RESET);
	printf("    %s│%s  [3] 🌍 GENERAL KNOWLEDGE                                              %s│%s\n", AQUA, WHITE, AQUA, RESET);
	printf("    %s└────────────────────────────────────────────────────────────────────────┘%s\n\n", AQUA, RESET);

	printf("    %s┌────────────────────────────────────────────────────────────────────────┐%s\n", GOLD, RESET);
	printf("    %s│%s  [4] 🗺️  GEOGRAPHY                                                     %s│%s\n", GOLD, WHITE, GOLD, RESET);
	printf("    %s└────────────────────────────────────────────────────────────────────────┘%s\n", GOLD, RESET);

	printf("\n");
	printBorder(GOLD, 80);
	printf("\n");

	printf("%s    > Enter your choice (1-4): %s", WHITE, RESET);
	scanf("%d", &choice);
	int c;
	while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer

	int totalQuestions = 0;

	switch (choice) {
		case 1:
			strcpy(subjectName, "English");
			strcpy(filename, "english.txt");
			break;
		case 2:
			strcpy(subjectName, "Mathematics");
			strcpy(filename, "maths.txt");
			break;
		case 3:
			strcpy(subjectName, "General Knowledge");
			strcpy(filename, "gk.txt");
			break;
		case 4:
			strcpy(subjectName, "Geography");
			strcpy(filename, "geography.txt");
			break;
		default:
			printf("%s    Invalid choice! Returning to menu...%s\n", RED, RESET);
			delay(1500);
			return;
	}

	Question questions[10];
	Question defaultQuestions[10];

	// Try to load questions from file
	printf("\n%s    > Loading %s questions...%s\n", GRAY, subjectName, RESET);
	totalQuestions = loadQuestions(questions, filename);

	if(totalQuestions == 0) {
		printf("%s    ! File not found. Using default questions...%s\n", YELLOW, RESET);
		delay(1000);
		// Copy default questions
		for(int i = 0; i < 10; i++) {
			questions[i] = defaultQuestions[i];
		}
		totalQuestions = 10;
	}

	printf("%s    ✓ Loaded %d questions%s\n", AQUA, totalQuestions, RESET);
	printf("%s    > Press Enter to start...%s", GRAY, RESET);
	getchar();

	start_time = time(NULL);
	// Start the game
	int score = 0;
	int accuracy = 0;
	
	playGame(questions, totalQuestions, &score, &accuracy);
	
	end_time = time(NULL);
	LeaderboardEntry entry = {0};
	strcpy(entry.name, playerName);
	entry.score = score;
	entry.total_questions = totalQuestions;
	entry.test_date = end_time;
	entry.time_taken = (int)difftime(end_time, start_time);
	entry.accuracy = accuracy;
	saveScore(entry);
}

// Helper to check if a byte is a UTF-8 continuation byte (starts with 10xxxxxx)
int is_continuation(unsigned char byte) {
	return (byte & 0xC0) == 0x80;
}

// Reverses the string at the character (code point) level
void reverse_utf8(char *str) {
	int length = strlen(str);
	char **codepoints = NULL;
	int count = 0;
	char *p = str;

	// 1. Identify and store the start of each code point
	while (*p != '\0') {
		// Reallocate memory for the pointer array
		codepoints = realloc(codepoints, (count + 1) * sizeof(char *));
		codepoints[count++] = p;

		// Move to the next code point's start
		if ((*p & 0x80) == 0x00) { // ASCII (1 byte)
			p += 1;
		} else if ((*p & 0xE0) == 0xC0) { // 2 bytes
			p += 2;
		} else if ((*p & 0xF0) == 0xE0) { // 3 bytes (common for Urdu)
			p += 3;
		} else if ((*p & 0xF8) == 0xF0) { // 4 bytes
			p += 4;
		} else {
			// Handle invalid UTF-8 or other cases
			p += 1;
		}
	}

	// 2. Reverse the characters by copying them to a new buffer
	char *reversed_str = malloc(length + 1);
	char *q = reversed_str;
	for (int i = count - 1; i >= 0; i--) {
		char *cp_start = codepoints[i];
		char *next_cp_start = (i + 1 < count) ? codepoints[i+1] : str + length;
		int cp_len = next_cp_start - cp_start;

		memcpy(q, cp_start, cp_len);
		q += cp_len;
	}
	*q = '\0'; // Null-terminate the new string

	// 3. Copy the reversed string back to the original buffer (if modifying in-place)
	memcpy(str, reversed_str, length + 1);

	// Clean up
	free(codepoints);
	free(reversed_str);
}

// Save score to file
void saveScore(LeaderboardEntry entry) {
	FILE *fp = fopen("leaderboard.dat", "ab");

	if (fp == NULL) {
		printf("Error saving score!\n");
		return;
	}

	fwrite(&entry, sizeof(LeaderboardEntry), 1, fp);
	fclose(fp);

	//    printf("Score saved successfully!\n");
	printf(GREEN "\nYour score has been saved to scores.txt\n" RESET);
}

int compareScores(const void *a, const void *b) {
	LeaderboardEntry *entryA = (LeaderboardEntry *)a;
	LeaderboardEntry *entryB = (LeaderboardEntry *)b;

	float percentA = (float)entryA->score / entryA->total_questions;
	float percentB = (float)entryB->score / entryB->total_questions;

	if (percentB > percentA)
		return 1;
	if (percentB < percentA)
		return -1;

	// If scores are equal, faster time is better
	return entryA->time_taken - entryB->time_taken;
}
