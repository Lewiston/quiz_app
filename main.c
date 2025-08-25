#include "quiz.h"

// ---------------------------
// Global variables
// ---------------------------
Question questions[MAX_QUESTIONS];
int totalQuestionsLoaded = 0;

char selectedTopic[50] = "All Topics";
char selectedDifficulty[20] = "All";
int isTimedMode = 0;
int isRandomMode = 0;
int timeLimitPerQuestion = 0;

// ---------------------------
// Program Entry Point
// ---------------------------
int main() {
    srand(time(NULL)); // Seed random generator for shuffling

    loadQuestions(); // Load questions from file

    int choice;
    while (1) {
        displayMainMenu();
        printf("Enter choice: ");

        // Input validation
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // clear buffer
            continue;
        }
        while (getchar() != '\n'); // consume leftover newline

        // Handle menu choice
        switch (choice) {
            case 1: startQuiz(); break;
            case 2: chooseQuizMode(); break;
            case 3: viewPastScores(); break;
            case 4: displayHelp(); break;
            case 5: exitProgram(); break;
            default: printf("Invalid option. Try again.\n"); break;
        }
    }
    return 0;
}
