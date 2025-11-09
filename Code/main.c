#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include "game.h"

#define MAX_QUESTIONS 10
#define MAX_HINTS 3

// Console color codes (for Windows/Linux)
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"

// Function declarations
int loadQuestions(Question q[], const char *filename);
int playQuiz(Question q[], int total);
void saveScore(LeaderboardEntry entry);
void clearScreen();
void line();
void takeTest();
void displayMenu();
void viewLeaderboard();
int compareScores(const void *a, const void *b);

int main1() {

#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif
	int choice = 0;

	while (1) {
		displayMenu();
		printf("Enter your choice: ");
		scanf("%d", &choice);
		getchar(); // consume newline

		switch (choice) {
			case 1:
				takeTest();
				break;
			case 2:
				viewLeaderboard();
				break;
			case 3:
				printf("Thank you for using MCQ Program!\n");
				exit(0);
			default:
				printf("Invalid choice! Please try again.\n");
		}
	}
}

void takeTest() {
	int choice;
	char playAgain;
	Question questions[MAX_QUESTIONS];
	time_t start_time, end_time;
	LeaderboardEntry entry;
	const char *filename;

	do {
		clearScreen();
		printf(CYAN "\n====================================================\n" RESET);
		printf(GREEN "           WELCOME TO MULTI-SUBJECT QUIZ GAME        \n" RESET);
		printf(CYAN "====================================================\n\n" RESET);

		printf("\nEnter your name: ");
		fgets(entry.name, 50, stdin);
		entry.name[strcspn(entry.name, "\n")] = 0; // remove newline

		printf(YELLOW "Choose a Subject:\n" RESET);
		printf("1. English\n2. Mathematics\n3. General Knowledge\n4. Geography\n");
		printf("\nEnter your choice (1-4): ");
		scanf("%d", &choice);

		switch (choice) {
			case 1:
				strcpy(entry.subject, "English");
				filename = "english.txt";
				break;
			case 2:
				strcpy(entry.subject, "Mathematics");
				filename = "maths.txt";
				break;
			case 3:
				strcpy(entry.subject, "General Knowledge");
				filename = "gk.txt";
				break;
			case 4:
				strcpy(entry.subject, "Geography");
				filename = "geography.txt";
				break;
			default:
				printf(RED "Invalid choice! Try again.\n" RESET);
				continue;
		}

		int total = loadQuestions(questions, filename);
		if (total == 0) {
			printf(RED "\nError: No questions found or file missing!\n" RESET);
			continue;
		}


		start_time = time(NULL);
		int score = playQuiz(questions, total);
		end_time = time(NULL);
		entry.score = score;
		entry.total_questions = MAX_QUESTIONS>total ? total : MAX_QUESTIONS;
		entry.test_date = end_time;
		entry.time_taken = (int)difftime(end_time, start_time);
		saveScore(entry);

		printf("\nDo you want to play again? (y/n): ");
		scanf(" %c", &playAgain);

	} while (playAgain == 'y' || playAgain == 'Y');

	printf(GREEN "\nThank you for playing! Goodbye!\n" RESET);
}

void displayMenu() {
	clearScreen();
	printf("\n========================================\n");
	printf("     MCQ QUIZ PROGRAM\n");
	printf("========================================\n");
	printf("1. Take Test\n");
	printf("2. View Leaderboard\n");
	printf("3. Exit\n");
	printf("========================================\n");
}

// Function to load questions from file
int loadQuestions(Question q[], const char *filename) {
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

// Play quiz function
int playQuiz(Question q[], int total) {
	clearScreen();
	int i, score = 0, hintsUsed = 0;
	char answer, useHint;

	for (i = 0; i < total; i++) {
		clearScreen();
		line();
		printf(BLUE "Question %d of %d\n" RESET, i + 1, total);
		line();

		printf("%s", q[i].question);
		printf("%s%s%s%s\n", q[i].options[0], q[i].options[1], q[i].options[2], q[i].options[3]);

		// if (hintsUsed < MAX_HINTS) {
		// 	printf(YELLOW "Would you like a hint? (y/n): " RESET);
		// 	scanf(" %c", &useHint);
		// 	if (useHint == 'y' || useHint == 'Y') {
		// 		printf(CYAN "%s\n" RESET, q[i].hint);
		// 		hintsUsed++;
		// 	}
		// }

		printf("\nEnter your answer (A/B/C/D): ");
		scanf(" %c", &answer);

		if (answer == q[i].correctAnswer || answer == q[i].correctAnswer + 32) {
			printf(GREEN "\n? Correct!\n" RESET);
			score += 10;
		} else {
			printf(RED "\n? Wrong! Correct answer was %c.\n" RESET, q[i].correctAnswer);
		}

		if (i < total - 1) {
			printf("\nPress Enter for next question...");
			getchar();
			getchar();
		}
	}

	clearScreen();
	printf(CYAN "\n============================================\n" RESET);
	printf(GREEN "              QUIZ COMPLETED!\n" RESET);
	printf(CYAN "============================================\n" RESET);
	printf(YELLOW "Your Total Score: %d / %d\n" RESET, score, total * 10);
	printf("Hints Used: %d / %d\n", hintsUsed, MAX_HINTS);

	return score;
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

// Utility functions
void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void line() {
	printf(CYAN "----------------------------------------------------\n" RESET);
}

void viewLeaderboard() {
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

	printf("\n=========================================================================================\n");
	printf("          LEADERBOARD\n");
	printf("=========================================================================================\n");
	printf("%-4s %-20s %-20s %-10s %-10s %-20s\n",
	       "Rank", "Name", "Subject", "Score", "Time(s)", "Date");
	printf("=========================================================================================\n");

	for (int i = 0; i < count; i++) {
		strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S",
		         localtime(&entries[i].test_date));
		char score_str[20];
		sprintf(score_str, "%d/%d", entries[i].score, entries[i].total_questions);
		printf("%-4d %-20s %-20s %-10s %-10d %-20s\n",
		       i + 1,
		       entries[i].name,
		       entries[i].subject,
		       score_str,
		       entries[i].time_taken,
		       time_str);
	}
	printf("=========================================================================================\n");
	getche();
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
