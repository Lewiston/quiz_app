# Interactive Quiz System (C Project)

This is a terminal-based quiz application written in C.  
It allows you to play quizzes with multiple modes (timed, random, topic-based, difficulty-based).  
Your results are saved for later review.

## 📂 Project Files

quiz-project/
├── main.c # Entry point of the program
├── quiz.c # Core quiz logic
├── quiz.h # Structures, globals, and function prototypes
├── questions.txt # Stores all quiz questions
├── results.txt # Stores quiz results (auto-generated)
└── README.txt # Instructions (this file)

## ⚙️ Compilation

1. Open a terminal/command prompt.
2. Navigate to the folder containing the project files.
3. Run the following command:

`bash`
gcc main.c quiz.c -o quiz_app

./quiz_app # Linux / Mac
quiz_app.exe # Windows
