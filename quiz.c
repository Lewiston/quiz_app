#include "quiz.h"

// ===================================================
// Helper: Get current date (YYYY-MM-DD)
// ===================================================
void getCurrentDate(char* dateStr) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(dateStr, 20, "%Y-%m-%d", tm);
}

// ===================================================
// Display main menu
// ===================================================
void displayMainMenu() {
    printf("\n=== MAIN MENU ===\n");
    printf("1. Start Quiz\n");
    printf("2. Choose Quiz Mode\n");
    printf("3. View Past Scores\n");
    printf("4. Help\n");
    printf("5. Exit\n");
}

// ===================================================
// Choose quiz modes (timed, topic, difficulty, random)
// ===================================================
void chooseQuizMode() {
    printf("\n=== CHOOSE QUIZ MODE ===\n");
    printf("Current Settings:\n");
    printf("  Topic: %s\n", selectedTopic);
    printf("  Difficulty: %s\n", selectedDifficulty);
    printf("  Timed Mode: %s (%d sec)\n", isTimedMode ? "ON" : "OFF", timeLimitPerQuestion);
    printf("  Random Mode: %s\n\n", isRandomMode ? "ON" : "OFF");

    printf("1. Toggle Timed Mode\n");
    printf("2. Set Topic Mode\n");
    printf("3. Toggle Random Mode\n");
    printf("4. Set Difficulty Mode\n");
    printf("5. Reset Defaults\n");
    printf("6. Back to Menu\n");
    printf("Enter choice: ");

    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n'); // clear buffer

    switch (choice) {
        case 1:
            isTimedMode = !isTimedMode;
            if (isTimedMode) {
                printf("Enter time per question (sec): ");
                scanf("%d", &timeLimitPerQuestion);
                while (getchar() != '\n');
            } else {
                timeLimitPerQuestion = 0;
            }
            break;
        case 2:
            printf("Enter topic (or 'All'): ");
            fgets(selectedTopic, sizeof(selectedTopic), stdin);
            selectedTopic[strcspn(selectedTopic, "\n")] = 0;
            break;
        case 3:
            isRandomMode = !isRandomMode;
            break;
        case 4:
            printf("Enter difficulty (Easy/Medium/Hard or 'All'): ");
            fgets(selectedDifficulty, sizeof(selectedDifficulty), stdin);
            selectedDifficulty[strcspn(selectedDifficulty, "\n")] = 0;
            break;
        case 5:
            strcpy(selectedTopic, "All Topics");
            strcpy(selectedDifficulty, "All");
            isTimedMode = 0;
            isRandomMode = 0;
            timeLimitPerQuestion = 0;
            break;
        case 6:
            return;
        default:
            printf("Invalid choice.\n");
    }
}

// ===================================================
// View past quiz scores from results.txt
// ===================================================
void viewPastScores() {
    FILE *file = fopen("results.txt", "r");
    if (!file) {
        printf("No past scores found.\n");
        return;
    }

    printf("\n%-15s %-10s %-20s %-12s\n", "Username", "Score", "Mode", "Date");
    printf("------------------------------------------------------------\n");

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Result r;
        if (sscanf(line, "%49[^,],%d/%d,%99[^,],%19s",
                   r.username, &r.score, &r.total_questions, r.mode, r.date) == 5) {
            printf("%-15s %d/%d %-20s %-12s\n",
                   r.username, r.score, r.total_questions, r.mode, r.date);
        }
    }
    fclose(file);
}

// ===================================================
// Display help
// ===================================================
void displayHelp() {
    printf("\n=== HELP ===\n");
    printf("1. Answer questions by typing A, B, C, or D.\n");
    printf("2. Choose quiz modes (timed, topic, random, difficulty) in settings.\n");
    printf("3. Results are saved in results.txt with your name, score, and date.\n");
}

// ===================================================
// Exit the program
// ===================================================
void exitProgram() {
    printf("Goodbye!\n");
    exit(0);
}

// ===================================================
// Load questions from questions.txt
// ===================================================
void loadQuestions() {
    FILE *file = fopen("questions.txt", "r");
    if (!file) {
        perror("Error opening questions.txt");
        exit(1);
    }

    char line[256];
    totalQuestionsLoaded = 0;
    Question currentQ;

    while (fgets(line, sizeof(line), file) && totalQuestionsLoaded < MAX_QUESTIONS) {
        line[strcspn(line, "\n")] = 0; // trim newline

        if (strncmp(line, "Topic: ", 7) == 0) {
            strcpy(currentQ.topic, line + 7);
        } else if (strncmp(line, "Difficulty: ", 12) == 0) {
            strcpy(currentQ.difficulty, line + 12);
        } else if (strncmp(line, "A. ", 3) == 0) {
            strcpy(currentQ.options[0], line + 3);
        } else if (strncmp(line, "B. ", 3) == 0) {
            strcpy(currentQ.options[1], line + 3);
        } else if (strncmp(line, "C. ", 3) == 0) {
            strcpy(currentQ.options[2], line + 3);
        } else if (strncmp(line, "D. ", 3) == 0) {
            strcpy(currentQ.options[3], line + 3);
            // Next line is correct answer
            if (fgets(line, sizeof(line), file)) {
                line[strcspn(line, "\n")] = 0;
                if (strlen(line) == 1 && (line[0] >= 'A' && line[0] <= 'D')) {
                    currentQ.correct_option = line[0];
                    questions[totalQuestionsLoaded++] = currentQ;
                    fgets(line, sizeof(line), file); // consume blank line
                }
            }
        } else if (strlen(line) > 0) {
            strcpy(currentQ.question, line);
        }
    }
    fclose(file);
    printf("Loaded %d questions.\n", totalQuestionsLoaded);
}

// ===================================================
// Filter questions based on selected topic/difficulty
// ===================================================
void filterQuestions(Question filtered[], int* count) {
    *count = 0;
    for (int i = 0; i < totalQuestionsLoaded; i++) {
        int topicMatch = (strcmp(selectedTopic, "All Topics") == 0 ||
                          strcmp(selectedTopic, questions[i].topic) == 0);
        int diffMatch = (strcmp(selectedDifficulty, "All") == 0 ||
                         strcmp(selectedDifficulty, questions[i].difficulty) == 0);
        if (topicMatch && diffMatch) {
            filtered[(*count)++] = questions[i];
        }
    }
}

// ===================================================
// Shuffle questions (Random Mode)
// ===================================================
void shuffleQuestions(Question arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Question temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// ===================================================
// Ask one question and return 1 if correct
// ===================================================
int askEachQuestion(Question q) {
    printf("\nTopic: %s | Difficulty: %s\n", q.topic, q.difficulty);
    printf("Q: %s\n", q.question);
    printf("A. %s\nB. %s\nC. %s\nD. %s\n",
           q.options[0], q.options[1], q.options[2], q.options[3]);

    char ans;
    time_t start = time(NULL);

    while (1) {
        printf("Your answer: ");
        scanf(" %c", &ans);
        ans = toupper(ans);

        if (ans >= 'A' && ans <= 'D') break;

        if (isTimedMode && difftime(time(NULL), start) >= timeLimitPerQuestion) {
            printf("Time's up!\n");
            return 0;
        }
        printf("Invalid. Enter A, B, C, or D.\n");
    }

    if (ans == q.correct_option) {
        printf("Correct!\n");
        return 1;
    } else {
        printf("Incorrect. Correct answer: %c\n", q.correct_option);
        return 0;
    }
}

// ===================================================
// Save result to results.txt
// ===================================================
void calculateAndStoreScore(const char* username, int score, int totalQuestions, const char* modeDescription) {
    char dateStr[20];
    getCurrentDate(dateStr);

    FILE *file = fopen("results.txt", "a");
    if (!file) {
        perror("Error opening results.txt");
        return;
    }
    fprintf(file, "%s,%d/%d,%s,%s\n", username, score, totalQuestions, modeDescription, dateStr);
    fclose(file);

    printf("\nScore saved: %d/%d (%s)\n", score, totalQuestions, modeDescription);
}

// ===================================================
// Start a quiz session
// ===================================================
void startQuiz() {
    char username[50];
    printf("\nEnter your name: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;
    if (strlen(username) == 0) strcpy(username, "Guest");

    // Filter questions
    Question filtered[MAX_QUESTIONS];
    int count;
    filterQuestions(filtered, &count);

    if (count == 0) {
        printf("No questions match your settings.\n");
        return;
    }

    if (isRandomMode) shuffleQuestions(filtered, count);

    int score = 0;
    for (int i = 0; i < count; i++) {
        printf("\n--- Question %d/%d ---\n", i + 1, count);
        if (askEachQuestion(filtered[i])) score++;
    }

    // Save results
    char modeDesc[100] = "";
    if (isTimedMode) strcat(modeDesc, "Timed, ");
    if (isRandomMode) strcat(modeDesc, "Random, ");
    if (strcmp(selectedTopic, "All Topics") != 0) {
        strcat(modeDesc, "Topic: ");
        strcat(modeDesc, selectedTopic);
        strcat(modeDesc, ", ");
    }
    if (strcmp(selectedDifficulty, "All") != 0) {
        strcat(modeDesc, "Difficulty: ");
        strcat(modeDesc, selectedDifficulty);
        strcat(modeDesc, ", ");
    }
    if (strlen(modeDesc) == 0) strcpy(modeDesc, "Default Mode");

    calculateAndStoreScore(username, score, count, modeDesc);
}
