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
#include "game.h"

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#define CLEAR "cls"
#else
#include <unistd.h>
#define CLEAR "clear"
#endif
// Global variables
Settings settings = {1, 0, "ENGLISH"};

// Question questions[] =
// 	{
// 		"Mount Everest is located in which mountain range?",
// 		{
// 			"Alps",
// 			"Andes",
// 			"Himalayas",
// 			"Rockies"
// 		},
// 		'1',
// 		"It’s the world’s most earthquake-prone region, shaped like a giant horseshoe."
// 	};

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

//void printWrappedBox(const char *text) {
//    // ┌─── top border ───┐
//    printf("    %s┌", AQUA);
//    for (int i = 0; i < WRAP_WIDTH + 2; i++) printf("─");
//    printf("┐%s\n", RESET);
//
//    int len = utf8_strlen(text);
//    int start = 0;
//
//    while (start < len) {
//        int end = len - start; // start + WRAP_WIDTH;
////        if (end > len) end = len;
//		if (end > WRAP_WIDTH) end = WRAP_WIDTH;
//
//        // wrap cleanly at spaces
//        if ((len - start) > WRAP_WIDTH && end < len && text[start+end] != ' ') {
//            int lastSpace = start+end;
//            while (lastSpace > start && text[lastSpace] != ' ')
//                lastSpace--;
//            if (lastSpace > start)
//                end = lastSpace;
//        }
//
//        // extract one line
//        char line[WRAP_WIDTH + 1];
//        int lineLen = end; // - start;
//        strncpy(line, text + start, lineLen);
//        line[lineLen] = '\0';
//
//        // skip any spaces at start of next line
//        while (text[end] == ' ') end++;
//
//        // print with consistent padding
//        printf("    %s│ %s%-*s%s │%s\n", AQUA, WHITE, WRAP_WIDTH, line, AQUA, RESET);
//		if(start+end>=len){
//			break;
//		}
//        start = end;
//    }

// └─── bottom border ───┘
//    printf("    %s└", AQUA);
//  for (int i = 0; i < WRAP_WIDTH + 2; i++) printf("─");
//printf("┘%s\n\n", RESET);
//}
//void printWrappedBox(const char *text) {
//    printf("    %s┌────────────────────────────────────────────────────────────────────────┐%s\n", AQUA, RESET);
//
//    int len = utf8_strlen(text);
//    int start = 0;
//
//    while (start < len) {
//        int end = start + WRAP_WIDTH;
//        if (end > len) end = len;
//
//        // Find last space before width for clean wrapping
//        if (end < len && text[end] != ' ') {
//            int lastSpace = end;
//            while (lastSpace > start && text[lastSpace] != ' ')
//                lastSpace--;
//            if (lastSpace > start)
//                end = lastSpace;
//        }
//
//        // Copy substring
//        char line[WRAP_WIDTH + 1];
//        int lineLen = end - start;
//        strncpy(line, text + start, lineLen);
//        line[lineLen] = '\0';
//
//        // Skip spaces before next word
//        while (text[end] == ' ') end++;
//
//        // Print line with padding
//        printf("    %s│%s %-70s%s│%s\n", AQUA, WHITE, line, AQUA, RESET);
//        start = end;
//    }
//
//    printf("    %s└────────────────────────────────────────────────────────────────────────┘%s\n\n", AQUA, RESET);
//}

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
	printf("    %s%s[1] START GAME%s\n", BOLD, AQUA, RESET);
	printf("    %s[2] LEADERBOARD%s\n", GOLD, RESET);
	printf("    %s[3] SETTINGS%s\n", VIOLET, RESET);
	printf("    %s[4] EXIT%s\n", RED, RESET);

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
			printf("\n%s    Thanks for playing TRIVIA.EXE!%s\n\n", GOLD, RESET);
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

	// Mic setting
	printf("    %s┌─────────────────────────────────────────┐%s\n", AQUA, RESET);
	printf("    %s│%s  🎤 MICROPHONE INPUT: %s%-17s%s│%s\n",
	       AQUA, WHITE,
	       settings.mic ? AQUA : RED,
	       settings.mic ? "ENABLED" : "DISABLED",
	       AQUA, RESET);
	printf("    %s└─────────────────────────────────────────┘%s\n\n", AQUA, RESET);

	// Language setting
	printf("    %s┌─────────────────────────────────────────┐%s\n", GOLD, RESET);
	printf("    %s│%s  🌐 LANGUAGE: %s%-27s%s│%s\n",
	       GOLD, WHITE, GOLD, settings.language, GOLD, RESET);
	printf("    %s└─────────────────────────────────────────┘%s\n\n", GOLD, RESET);

	printBorder(VIOLET, 80);
	printf("\n");

	printf("    %s[1] Toggle Sound    [2] Toggle Mic    [3] Change Language    [ESC] Back%s\n\n", GRAY, RESET);
	printf("%s    > SELECT OPTION: %s", WHITE, RESET);

	char choice;
	scanf(" %c", &choice);
	getchar();
	//switchcase2
	switch(choice) {
		case '1':
			settings.sound = !settings.sound;
			showSettings();
			break;
		case '2':
			settings.mic = !settings.mic;
			showSettings();
			break;
		case '3':
			printf("\n    %sLanguages: [1] ENGLISH  [2] URDU \n", GRAY, RESET);
			printf("    > Select: ");
			char lang;
			scanf(" %c", &lang);
			getchar();
			switch(lang) {
				case '1':
					strcpy(settings.language, "ENGLISH");
					break;
				case '2':
					strcpy(settings.language, "URDU");
					break;
			}
			showSettings();
			break;
		case 27:
			showStartScreen();
			break;
		default:
			return;
	}
}

void showLeaderboard() {
	clearScreen();

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

	// Entries-array use
	const char* medals[] = {"🏆", "🥈", "🥉", "  ", "  "};
	//for loop
	for(int i = 0; i < 5; i++) {
		const char* color = (i == 0) ? GOLD : WHITE;
//        printf("    %s%-2s %d    %-20s%s%-16d%s%-10d%%%s\n",
//               color,
//               medals[i],
//               i + 1,
//               leaderboard[i].name,
//               AQUA,
//               leaderboard[i].score,
//               VIOLET,
//               leaderboard[i].accuracy,
//               RESET);
	}

	printf("\n");
	printBorder(GOLD, 80);
	printf("\n");

	printf("%s    Press Enter to return to menu...%s", GRAY, RESET);
	getchar();
}

void playGame(Question questions[MAX_QUESTIONS], int total) {
	int score = 0;
	int currentQuestion = 0;

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
		int isDoubleOrNothing = (currentQuestion == 4);
		int isSpeedDuel = (currentQuestion == 7);

		printf("\n\n");
		printBorder(AQUA, 80);
		printf("\n");

		// Special round banner
		if(isDoubleOrNothing) {
			printf("\n");
			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", GOLD);
			printCentered("⚠  DOUBLE OR NOTHING ROUND!  ⚠", GOLD);
			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", GOLD);
			printf("\n");
		} else if(isSpeedDuel) {
			printf("\n");
			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", RED);
			printCentered("⚡ SPEED DUEL ⚡ 10 SECONDS!", RED);
			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", RED);
			printf("\n");
		}

		// Header
		printf("    %s🏆 SCORE: %s%d%s/10        ", WHITE, AQUA, score, WHITE);
//        printf("%s[%s] %s%s\n\n", VIOLET, q.category, WHITE, RESET);

		// Progress
		int progress = ((currentQuestion + 1) * 100) / 10;
		printf("    %s> QUESTION %d OF 10                                    %d%%%s\n",
		       GRAY, currentQuestion + 1, progress, RESET);
		printf("    %s[", AQUA);
		int barWidth = 50;
		int filled = (progress * barWidth) / 100;
		for(int i = 0; i < barWidth; i++) {
			//if-else statement
			if(i < filled) printf("█");
			else printf("░");
		}
		printf("]%s\n\n", RESET);

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
			printf("    %s│%s [%c] %-67s%s│%s\n", GRAY, WHITE, option, q.options[i], GRAY, RESET);
			printf("    %s└────────────────────────────────────────────────────────────────────────┘%s\n", GRAY, RESET);
			if(i < 3) printf("\n");
		}

		printf("\n");
		printBorder(AQUA, 80);
		printf("\n");

		printf("%s    > YOUR ANSWER (A/B/C/D): %s", WHITE, RESET);

		char answer[3];
		scanf(" %c", &answer[0]);
		//	fgets(answer,sizeof(answer),stdin);
//		getchar();

		//	answer = toupper(answer);
		int answerIndex = q.correctAnswer-'1' ;

		// Check answer
		printf("\n");
		if(answer[0] == q.correctAnswer) {
			int points = isDoubleOrNothing ? 2 : 1;
			score += points;

			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━", AQUA);
			char msg[100];
			sprintf(msg, "✓ CORRECT! +%d POINTS", points * 100);
			printCentered(msg, AQUA);
			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━", AQUA);
		} else {
			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", RED);
			printCentered("✗ INCORRECT!", RED);
			char msg[200];
			sprintf(msg, "CORRECT ANSWER: %s", q.options[answerIndex]);
			printCentered(msg, RED);
			printCentered("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", RED);
		}

		printf("\n%s    Press Enter to continue...%s", GRAY, RESET);
		getchar();
		int c;
		while ((c = getchar()) != '\n' && c != EOF);

		currentQuestion++;
	}

	showGameOver(score, 10);
}

void showGameOver(int score, int total) {
	clearScreen();
	//mathematical calculations
	int percentage = (score * 100) / total;
	int finalScore = score * 100;
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
	printGlow("                         GAME COMPLETE\n", GOLD);
	printf("\n");
	printCentered(message, GRAY);
	printf("\n");

	// Stars
	printf("                              ");
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
	printf("    %s│%s                          FINAL SCORE                               %s│%s\n", AQUA, GRAY, AQUA, RESET);
	printf("    %s│%s                          %s%-10d%s                             %s│%s\n",
	       AQUA, WHITE, AQUA, finalScore, WHITE, AQUA, RESET);
	printf("    %s│                                                                      │%s\n", AQUA, RESET);
	printf("    %s│%s        CORRECT: %s%d/%d%s            ACCURACY: %s%d%%%s                   %s│%s\n",
	       AQUA, WHITE, AQUA, score, total, WHITE, VIOLET, percentage, WHITE, AQUA, RESET);
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

	printf("\n\n");
	printBorder(AQUA, 80);
	printf("\n");

	printGlow("    > PLAYER SETUP\n\n", AQUA);
	printf("%s    ENTER YOUR DETAILS TO BEGIN%s\n\n", GRAY, RESET);

	printBorder(AQUA, 80);
	printf("\n");

	// Player name input
	printf("    %s┌────────────────────────────────────────────────────────────────────────┐%s\n", AQUA, RESET);
	printf("    %s│%s  PLAYER NAME:                                                         %s│%s\n", AQUA, WHITE, AQUA, RESET);
	printf("    %s└────────────────────────────────────────────────────────────────────────┘%s\n", AQUA, RESET);
	printf("\n    %s> Enter your name: %s", WHITE, RESET);

	char playerName[256];
	fgets(playerName, 255, stdin);
	playerName[strcspn(playerName, "\n")] = 0; // remove newline

	if(strlen(playerName) == 0) {
		strcpy(playerName, "PLAYER");
	}

	printf("\n");
	printBorder(AQUA, 80);
	printf("\n");

	// Subject selection
	printGlow("    > SELECT SUBJECT\n\n", GOLD);

	printf("    %s┌────────────────────────────────────────────────────────────────────────┐%s\n", AQUA, RESET);
	printf("    %s│%s  [1] 📚 ENGLISH                                                       %s│%s\n", AQUA, WHITE, AQUA, RESET);
	printf("    %s└────────────────────────────────────────────────────────────────────────┘%s\n\n", AQUA, RESET);

	printf("    %s┌────────────────────────────────────────────────────────────────────────┐%s\n", GOLD, RESET);
	printf("    %s│%s  [2] ➗ MATHEMATICS                                                   %s│%s\n", GOLD, WHITE, GOLD, RESET);
	printf("    %s└────────────────────────────────────────────────────────────────────────┘%s\n\n", GOLD, RESET);

	printf("    %s┌────────────────────────────────────────────────────────────────────────┐%s\n", AQUA, RESET);
	printf("    %s│%s  [3] 🌍 GENERAL KNOWLEDGE                                             %s│%s\n", AQUA, WHITE, AQUA, RESET);
	printf("    %s└────────────────────────────────────────────────────────────────────────┘%s\n\n", AQUA, RESET);

	printf("    %s┌────────────────────────────────────────────────────────────────────────┐%s\n", GOLD, RESET);
	printf("    %s│%s  [4] 🗺️  GEOGRAPHY                                                    %s│%s\n", GOLD, WHITE, GOLD, RESET);
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
			strcpy(filename, "maths2.txt");
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

	// Start the game
	playGame(questions, totalQuestions);
}