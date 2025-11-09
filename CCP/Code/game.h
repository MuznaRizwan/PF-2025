#define MAX_QUESTIONS 10
#define MAX_HINTS 3
#define MAX_LINES   50
#define MAX_WIDTH   70

// ANSI Color Codes
#define RESET "\033[0m"
#define AQUA "\033[38;2;0;224;161m"      // #00E0A1
#define GOLD "\033[38;2;255;211;105m"    // #FFD369
#define VIOLET "\033[38;2;139;92;246m"   // #8B5CF6
#define WHITE "\033[38;2;237;237;237m"   // #EDEDED
#define GRAY "\033[38;2;170;170;170m"    // #AAAAAA
#define RED "\033[38;2;255;107;107m"     // #FF6B6B
#define BG_DARK "\033[48;2;13;13;13m"    // #0D0D0D

// Console color codes (for Windows/Linux)
//#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define CYAN "\x1b[36m"
//#define RESET "\x1b[0m"

// Bold and effects
#define BOLD "\033[1m"
#define DIM "\033[2m"

#define DOUBLE_EDGE_QUESTION 4
#define SPEED_DUEL_QUESTION 7

#define SCORE_PER_QUESTION 100

typedef struct {
    int sound;
    int mic;
    char language[10];
} Settings;

typedef struct {
    char question[256];
    char options[4][128];
    char correctAnswer;
    char hint[200];
} Question;

typedef struct
{
	char name[50];
	char subject[50];
	int score;
	int total_questions;
	time_t test_date;
	int time_taken; // in seconds
	int accuracy; // %age
} LeaderboardEntry;

typedef struct {
    char startGame[50];
    char leaderboard[50];
    char settings[50];
    char exit[50];
    char selectOption[50];
    char questionsInfo[100];
    char initializingProtocol[100];
    int isRTL; // Right-to-left flag
} LanguageStrings;

extern LanguageStrings langStrings;
void updateLanguage();
void printMenuOption(int number, char* text, const char* color);
// Function prototypes
void clearScreen();
void delay(int milliseconds);
void printBorder(const char* color, int width);
void printCentered(const char* text, const char* color);
void printGlow(const char* text, const char* color);
void showStartScreen();
void showSettings();
void showLeaderboard();
void playGame(Question questions[MAX_QUESTIONS], int total, int* score, int* accuracy);
void showGameOver(int correct, int total, int score, int maxScore);
void printBox(const char* content, const char* color);
void animateText(const char* text, const char* color);
// void printWrappedBox(const char *text);
int loadQuestions(Question questions[],const char* filename);
int splitText(char text[256], char lines[MAX_LINES][MAX_WIDTH]);
void showSubjectSelection();
char getAnswerWithTimeout(int seconds, int *timedOut);
void playSound(const char* soundFile);
int is_continuation(unsigned char byte);
void reverse_utf8(char *str);
void saveScore(LeaderboardEntry entry);
int compareScores(const void *a, const void *b);