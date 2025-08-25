#ifndef QUIZ_H
#define QUIZ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_QUESTIONS 100
#define MAX_RESULTS 50

// ---------------------------
// Structure to hold one quiz question
// ---------------------------
typedef struct {
    char topic[50];           // Question category (History, Math, etc.)
    char difficulty[20];      // Easy, Medium, Hard
    char question[256];       // The actual question text
    char options[4][100];     // Four possible answers
    char correct_option;      // 'A', 'B', 'C', or 'D'
} Question;

// ---------------------------
// Structure to hold quiz results for logging
// ---------------------------
typedef struct {
    char username[50];
    int score;
    int total_questions;
    char mode[100];
    char date[20];
} Result;

// ---------------------------
// Global variables (declared in main.c)
// ---------------------------
extern Question questions[MAX_QUESTIONS];
extern int totalQuestionsLoaded;

extern char selectedTopic[50];
extern char selectedDifficulty[20];
extern int isTimedMode;
extern int isRandomMode;
extern int timeLimitPerQuestion;

// ---------------------------
// Function prototypes
// ---------------------------
void displayMainMenu();
void chooseQuizMode();
void viewPastScores();
void displayHelp();
void exitProgram();

void loadQuestions();
void filterQuestions(Question filtered[], int* count);
void shuffleQuestions(Question arr[], int n);
int askEachQuestion(Question q);
void calculateAndStoreScore(const char* username, int score, int totalQuestions, const char* modeDescription);
void startQuiz();

#endif
