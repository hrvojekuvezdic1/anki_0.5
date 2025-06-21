#pragma warning(disable : 4996)

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include <wx/wx.h>
#include "classes.h"
#include "functions.h"

#ifdef _WIN32
#include <stdio.h>
#endif

#define BAD_DURATION 1
#define OK_DURATION 10
#define GREAT_DURATION 30
#define EASY_DURATION 60

class MyApp : public wxApp {
private:
    
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title);
    std::vector<flashcard> flashcards;
    bool alreadyLoaded;
private:
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxTextCtrl* consoleCtrl;
    void onLoadFlashcards(wxCommandEvent& event);
    void onDisplayFlashcards(wxCommandEvent& event);
    void onAddFlashcard(wxCommandEvent& event);
    void onSaveFlashcards(wxCommandEvent& event);
    std::string saveLocalFlashcards();
    void onStudyFlashcards(wxCommandEvent& event);
    void updateLocalFlashcards(queue* queue);
    void onQuit(wxCommandEvent& event);
};

bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame("Anki Clone");
    frame->Show(true);
    frame->CenterOnScreen();
    return true;
}

wxIMPLEMENT_APP(MyApp);


MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(900, 700)) {
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    panel->SetBackgroundColour(wxColour(33, 22, 70));
    SetTransparent(240);

    consoleCtrl = new wxTextCtrl(panel, wxID_ANY, "",
        wxPoint(20, 430), wxSize(850, 200),
        wxTE_MULTILINE | wxTE_READONLY);
    consoleCtrl->SetBackgroundColour(wxColour(60, 60, 80));
    consoleCtrl->SetForegroundColour(wxColour(255, 255, 255));
    consoleCtrl->SetFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    static wxStreamToTextRedirector* redirector = nullptr;
    if (!redirector) {
        redirector = new wxStreamToTextRedirector(consoleCtrl);
    }
    
    CustomButton* btnLoadFlashcards = new CustomButton(panel, wxID_ANY, "Load flashcards", wxPoint(350, 25), wxSize(200, 50));
    btnLoadFlashcards->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    Bind(wxEVT_BUTTON, &MyFrame::onLoadFlashcards, this, btnLoadFlashcards->GetId());

    CustomButton* btnDisplayFlashcards = new CustomButton(panel, wxID_ANY, "Display all flashcards", wxPoint(350, 80), wxSize(200, 50));
    btnDisplayFlashcards->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    Bind(wxEVT_BUTTON, &MyFrame::onDisplayFlashcards, this, btnDisplayFlashcards->GetId());

    CustomButton* btnAddFlashcard = new CustomButton(panel, wxID_ANY, "Add new flashcard", wxPoint(350, 135), wxSize(200, 50));
    btnAddFlashcard->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    Bind(wxEVT_BUTTON, &MyFrame::onAddFlashcard, this, btnAddFlashcard->GetId());

    CustomButton* btnSaveFlashcards = new CustomButton(panel, wxID_ANY, "Save all flashcards", wxPoint(350, 190), wxSize(200, 50));
    btnSaveFlashcards->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    Bind(wxEVT_BUTTON, &MyFrame::onSaveFlashcards, this, btnSaveFlashcards->GetId());

    CustomButton* btnStudyFlashcards = new CustomButton(panel, wxID_ANY, "Study", wxPoint(350, 245), wxSize(200, 50));
    btnStudyFlashcards->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    Bind(wxEVT_BUTTON, &MyFrame::onStudyFlashcards, this, btnStudyFlashcards->GetId());

    CustomButton* btnQuit = new CustomButton(panel, wxID_ANY, "Quit", wxPoint(350, 300), wxSize(200, 50));
    btnQuit->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    Bind(wxEVT_BUTTON, &MyFrame::onQuit, this, btnQuit->GetId());
}

void MyFrame::onLoadFlashcards(wxCommandEvent& event) {
    if (alreadyLoaded && MyFrame::flashcards.size() != 0) {
        std::cout << "Already loaded!\n" << std::endl;
        return;
    }
    alreadyLoaded = true;
    std::string text = loadFlashcards(MyFrame::flashcards);
    if (MyFrame::flashcards.size() == 0) {
        std::cout << "Database empty!" << std::endl;
        return;
    }
    std::cout << text << std::endl;
}

void MyFrame::onDisplayFlashcards(wxCommandEvent& event) {
    std::vector<std::string> flashcardsQA = displayFlashcards(MyFrame::flashcards);
    if (flashcardsQA.size() == 0) {
        std::cout << "Empty deck!" << std::endl;
        return;
    }
    for (int i = 0; i < flashcardsQA.size(); i++) {
        DisplayFlashcard dlg(this, flashcardsQA[i]);
        int result = dlg.ShowModal();
        if (result == 1) continue;
        else if (result == 2) {
            flashcardsQA.erase(flashcardsQA.begin() + i);
            MyFrame::flashcards.erase(MyFrame::flashcards.begin() + i);
            i--;
            continue;
        }
        
    }
}

void MyFrame::onAddFlashcard(wxCommandEvent& event) {
    wxTextEntryDialog question(this, "     ", "Question");
    question.SetBackgroundColour(wxColour(30, 0, 60));
    
    if (question.ShowModal() == wxID_OK) {
        wxString flashcardQuestion = question.GetValue();
        wxTextEntryDialog answer(this, "     ", "Answer");
        answer.SetBackgroundColour(wxColour(30, 0, 60));
        answer.SetForegroundColour(wxColour(255, 255, 255));
        if (answer.ShowModal() == wxID_OK) {
            wxString flashcardAnswer = answer.GetValue();
            MyFrame::flashcards.push_back(flashcard(flashcardQuestion.ToStdString(), flashcardAnswer.ToStdString()));
            std::cout << "Flashcard added successfully!\n" << std::endl;
        }
    }
}

void MyFrame::onSaveFlashcards(wxCommandEvent& event) {
    std::cout << saveLocalFlashcards() << std::endl;
}

std::string MyFrame::saveLocalFlashcards() {
    if (MyFrame::flashcards.size() == 0) {
        return "Empty deck!";
    }
    return saveFlashcards(MyFrame::flashcards);
}

void MyFrame::onStudyFlashcards(wxCommandEvent& event) {
    if (flashcards.size() == 0) {
        std::cout << "Empty deck!" << std::endl;
        return;
    }
    std::vector<time_t> dateTimes;
    queue* _queue = nullptr;
    node* head = nullptr;
    for (int i = 0; i < flashcards.size(); i++) {
        dateTimes.push_back(flashcards[i].getDateTime());
    }
    bool isFirstSession = true;
    for (int i = 0; i < flashcards.size(); i++) {
        for (int j = 0; j < flashcards.size(); j++) {
            if (dateTimes[i] != dateTimes[j]) {
                break;
                isFirstSession = false;
            }
        }
    }
    if (isFirstSession) {
        head = new node(flashcards[0]);
        _queue = new queue(head);
        for (int i = 1; i < flashcards.size(); i++) {
            _queue->enqueue(new node(flashcards[i]));
        }
    }
    node* t;
    while ((t = _queue->dequeue()) != nullptr) {
        std::string input;
        t->setNext(nullptr);
        flashcard& _flashcard = t->getFlashcard();
        int result;
        QuestionDialog qdlg(this, _flashcard.getQuestion());
        qdlg.CenterOnScreen();
        result = qdlg.ShowModal();

        bool endSession = false;
        if (result == 1) {
            FiveButtonDialog dlg(this, _flashcard.getAnswer());
            dlg.CenterOnScreen();
            result = dlg.ShowModal();

            if (result == 1) {
                _flashcard.setDateTime(std::chrono::system_clock::now() + std::chrono::minutes(BAD_DURATION));
            }
            else if (result == 2) {
                _flashcard.setDateTime(std::chrono::system_clock::now() + std::chrono::minutes(OK_DURATION));
            }
            else if (result == 3) {
                _flashcard.setDateTime(std::chrono::system_clock::now() + std::chrono::minutes(GREAT_DURATION));
            }
            else if (result == 4) {
                _flashcard.setDateTime(std::chrono::system_clock::now() + std::chrono::minutes(EASY_DURATION));
            }
            else if (result == 5) {
                _queue->push(t);
                std::vector<flashcard> flashcardsToSave;
                while ((t = _queue->dequeue()) != nullptr) {
                    flashcardsToSave.push_back(t->getFlashcard());
                }
                MyFrame::flashcards = flashcardsToSave;
                saveFlashcards(flashcardsToSave);
                break;
            }
            _queue->enqueue(t);
            _queue->sortByDateTime();
            continue;
        }
        else if (result == 2) {
            _queue->push(t);
            std::vector<flashcard> flashcardsToSave;
            while ((t = _queue->dequeue()) != nullptr) {
                flashcardsToSave.push_back(t->getFlashcard());
            }
            MyFrame::flashcards = flashcardsToSave;
            saveFlashcards(flashcardsToSave);
            break;
        }
    }
}

void MyFrame::updateLocalFlashcards(queue* _queue) {
    flashcards.clear();
    node* t;
    while ((t = _queue->peek()) != nullptr) {
        flashcards.push_back(t->getFlashcard());
        t = t->getNext();
    }
}

void MyFrame::onQuit(wxCommandEvent& event) {
    std::cout << "Saving..." << std::endl;
    std::cout << saveLocalFlashcards() << std::endl;
    wxThread::Sleep(1000);

    QuitDialog qd(this);
    int result = qd.ShowModal();
    if (result == 1) {
        Close();
    }
    else if (result == 2) {
        return;
    }
    
}






