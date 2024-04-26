#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <thread>
#include <sstream>

using namespace std;

struct Question {
    string question;
    vector<string> options;
    int correctOption;
};

class QuizGame {
private:
    vector<Question> questions;
    int score;
    int currentQuestionIndex;

public:
    QuizGame(const vector<Question>& questions) : questions(questions), score(0), currentQuestionIndex(0) {}

    void startGame() {
        score = 0;
        currentQuestionIndex = 0;
        cout << "Welcome to the Anime Quiz Game!" << endl;
        cout << "Answer the following questions:" << endl;
        random_shuffle(questions.begin(), questions.end());
        while (currentQuestionIndex < 10) {
            displayNextQuestion();
        }
        cout << "Quiz completed! Your final score is: " << score << "/10" << endl;
    }

    void displayNextQuestion() {
        if (currentQuestionIndex < 10) {
            const Question& q = questions[currentQuestionIndex];
            cout << "Question " << (currentQuestionIndex + 1) << ": ";
            cout << q.question << endl;
            for (int i = 0; i < q.options.size(); ++i) {
                cout << i + 1 << ". " << q.options[i] << endl;
            }
            cout << "Enter your choice (1-" << q.options.size() << "): ";
            int choice;
            cin >> choice;
            if (choice >= 1 && choice <= q.options.size()) {
                evaluateAnswer(choice - 1);
            } else {
                cout << "Invalid choice. Please enter a number between 1 and " << q.options.size() << "." << endl;
            }
            currentQuestionIndex++;
        }
    }

    void evaluateAnswer(int choice) {
        const Question& q = questions[currentQuestionIndex];
        if (q.correctOption == choice) {
            cout << "Correct!" << endl;
            score++;
        } else {
            cout << "Incorrect! The correct answer is: " << q.options[q.correctOption] << endl;
        }
    }
};

vector<Question> fetchQuestionsFromTextFile(const string& filename) {
    vector<Question> questions;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        Question q;
        q.question = line;
        
        getline(file, line); // Read options line
        stringstream ss(line);
        string option;
        while (getline(ss, option, ',')) {
            q.options.push_back(option);
        }

        getline(file, line); // Read correct option index
        q.correctOption = stoi(line);

        questions.push_back(q);
    }

    file.close();

    return questions;
}

int main() {
    vector<Question> questions = fetchQuestionsFromTextFile("questions.txt");

    // Shuffle the questions
    random_device rd;
    mt19937 g(rd());
    shuffle(questions.begin(), questions.end(), g);

    // Select first 10 questions
    if (questions.size() > 10) {
        questions.resize(10);
    }

    QuizGame game(questions);
    game.startGame();

    return 0;
}
    