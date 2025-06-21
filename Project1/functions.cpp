#include "functions.h"
#include <fstream>
#include <string>
#include <iostream>

flashcard generateFlashcard(std::string card) {
    std::string question = "";
    std::string answer = "";
    bool isQuestion = true;
    for (int i = 0; i < card.length(); i++) {
        char c = card.at(i);
        if (c == '$') {
            isQuestion = false;
            continue;
        }
        if (isQuestion) {
            question.push_back(c);
        }
        else {
            answer.push_back(c);
        }
    }
    flashcard flashcard(question, answer);
    return flashcard;
}

std::string loadFlashcards(std::vector<flashcard>& flashcards) {
    std::ifstream file_in;
    file_in.open("C:\\Users\\Hrvoje\\Documents\\Anki\\db\\database.txt", std::ifstream::in);
    int i = 0;
    std::string line;
    std::string text;
    while (getline(file_in, line)) {
        flashcards.push_back(generateFlashcard(line));
        text.append("loaded: QUESTION - " + flashcards[i].getQuestion() + " ANSWER - " + flashcards[i].getAnswer() + " from the database.\n");
        i++;
    }
    return text;
}

void addFlashcard(std::vector<flashcard>& flashcards) {
    std::cout << "Format: q,a\n";
    std::string _flashcard;
    std::cin >> _flashcard;
    flashcards.push_back(generateFlashcard(_flashcard));
    std::cout << "added: QUESTION - " + flashcards.back().getQuestion() + " ANSWER - " + flashcards.back().getAnswer() + " to temporary storage.\n";
}

std::string saveFlashcards(std::vector<flashcard>& flashcards) {
    std::ofstream file_out;
    file_out.open("C:\\Users\\Hrvoje\\Documents\\Anki\\Anki\\Anki\\x64\\Debug\\database.txt", std::ofstream::out);
    std::string text;
    for (int i = 0; i < flashcards.size(); i++) {
        file_out << flashcards[i].getQuestion() + "$" + flashcards[i].getAnswer() + "\n";
        text.append("saved: QUESTION - " + flashcards[i].getQuestion() + " ANSWER - " + flashcards[i].getAnswer() + " to the database.\n");
    }
    return text;
}

std::vector<std::string> displayFlashcards(std::vector<flashcard>& flashcards) {
    std::vector<std::string> flashcardQA;
    for (int i = 0; i < flashcards.size(); i++) {
        std::time_t dt = flashcards[i].getDateTime();
        flashcardQA.push_back("Q - " + flashcards[i].getQuestion()
            + "\n" + "A - " + flashcards[i].getAnswer()
            + "\n\n");
    }
    return flashcardQA;
}