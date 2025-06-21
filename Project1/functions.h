#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>
#include "classes.h"

flashcard generateFlashcard(std::string card);
std::string loadFlashcards(std::vector<flashcard>& flashcards, std::string);
void addFlashcard(std::vector<flashcard>&flashcards);
std::string saveFlashcards(std::vector<flashcard>& flashcards, std::string);
std::vector<std::string> displayFlashcards(std::vector<flashcard>& flashcards);

#endif