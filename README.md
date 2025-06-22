![image](https://github.com/user-attachments/assets/ebe90bf9-d996-46aa-96ce-60c1fe03d0ff)

# Anki 0.5 - Flashcard Application for Studying

## Table of Contents

- [Characteristics](#characteristics)
- [Techonologies](#technologies)
- [Structure](#structure)
- [Usage](#usage)
- [Dependencies](#dependencies)
- [Note](#note)

## Characteristics
- creating flashcards, saving them to database and loading them from the database (text file)
- displaying all flashcards with ability to remove them from the collection
- adding flashcards to collection
- studying the flashcards using an algorithm based on spaced repetition
- simple graphical user interface with buttons
- built-in logger for useful information

## Technologies
- C++
- [wxWidgets 3.3.0.](https://wxwidgets.org/downloads/) - for GUI
- Windows (Win64)
 
## Structure
- main.cpp
- classes.h and classes.cpp - contains classes:
  - flashcard,
  - node,
  - queue,
  - classes of different types of buttons (CustomButton, QuitDialog, DisplayFlashcard, QuestionDialog, FiveButtonDialog)
- functions.h and functions.cpp - contains functions:
  - generateFlashcard - generates flashcard from string (parser),
  - loadFlashcards - load flashcards from database (text file),
  - addFlashcard - add flashcard to collection (deck),
  - saveFlashcards - save all flashcards from a collection to database (text file),
  - displayFlashcards - used when "Display all flashcards" button is pressed
- Visual Studio project and solution

## Usage
- open solution and define FILEPATH constant which defines path to database (text file)
- compile and run:
  1. Database is empty on first run so Load flashcards will not do anything
  2. Add flashcards via Add flashcard button
  3. Display flashcards (you can also remove flashcards from this event)
  4. Save flashcards for later use (flashcards will also be saved automatically when "Save and Quit" button is pressed)
  5. Study - on each answer shown you are asked to choose how well you knew the flashcard which determines order in which the cards will reappear
  6. Save and Quit - saves and asks you if you are sure you want to exit
 
## Dependencies
- [wxWidgets 3.3.0.](https://wxwidgets.org/downloads/)
- Visual Studio 2019 or newer
- Tested on Windows

## Note
- project heavily inspired by [Anki](https://apps.ankiweb.net/)
- "Anki is a flashcard program that helps you spend more time on challenging material, and less on what you already know."
