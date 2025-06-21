#pragma warning(disable : 4996)

#ifndef CLASSES_H
#define CLASSES_H

#include <string>
#include <chrono>

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include <wx/wx.h>

#ifdef _WIN32
#include <stdio.h>
#endif

class flashcard {
private:
	std::string question;
	std::string answer;
	std::chrono::system_clock::time_point now;
public:
	flashcard(std::string question, std::string answer);
	std::string getQuestion();
	std::string getAnswer();
	std::time_t getDateTime();
	void setDateTime(std::chrono::system_clock::time_point);
};

class node {
private:
	flashcard _flashcard;
	node* next;
public:
	node(flashcard);
	node();
	node* getNext();
	void setNext(node*);
	flashcard& getFlashcard();
	void setFlashcard(const flashcard& fc);
};

class queue {
private:
	node* first;
	node* last;
public:
	queue(node* head);
	void enqueue(node* newNode);
	node* dequeue();
	void sortByDateTime();
	void push(node* newHead);
	node* peek();
};

class CustomButton : public wxControl {
public:
    CustomButton(wxWindow* parent, wxWindowID id, const wxString& label,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize)
        : wxControl(parent, id, pos, size, wxBORDER_NONE), label(label), hover(false)
    {
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        Bind(wxEVT_PAINT, &CustomButton::OnPaint, this);
        Bind(wxEVT_ENTER_WINDOW, &CustomButton::OnMouseEnter, this);
        Bind(wxEVT_LEAVE_WINDOW, &CustomButton::OnMouseLeave, this);
        Bind(wxEVT_LEFT_DOWN, &CustomButton::OnMouseClick, this);
    }

private:
    wxString label;
    bool hover;

    void OnPaint(wxPaintEvent&) {
        wxPaintDC dc(this);
        wxColour bg = hover ? wxColour(30, 100, 220) : wxColour(50, 0, 200);
        dc.SetBrush(wxBrush(bg));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.DrawRectangle(GetClientRect());
        dc.SetTextForeground(*wxWHITE);
        dc.SetFont(GetFont());
        wxSize textSize = dc.GetTextExtent(label);
        wxSize size = GetClientSize();
        dc.DrawText(label, (size.x - textSize.x) / 2, (size.y - textSize.y) / 2);
    }

    void OnMouseEnter(wxMouseEvent&) {
        hover = true;
        Refresh();
    }
    void OnMouseLeave(wxMouseEvent&) {
        hover = false;
        Refresh();
    }
    void OnMouseClick(wxMouseEvent& event) {
        wxCommandEvent evt(wxEVT_BUTTON, GetId());
        evt.SetEventObject(this);
        GetParent()->GetEventHandler()->ProcessEvent(evt);
    }
};

class QuitDialog : public wxDialog {
public:
    QuitDialog(wxWindow* parent)
        : wxDialog(parent, wxID_ANY, "Exiting", wxDefaultPosition, wxSize(600, 600)) {
        SetBackgroundColour(wxColour(30, 0, 60));

        wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

        wxStaticText* questionLabel = new wxStaticText(this, wxID_ANY, "Are you sure you want to exit?");
        questionLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        questionLabel->SetForegroundColour(wxColour(255, 255, 255));
        vbox->Add(questionLabel, 0, wxALL | wxALIGN_CENTER, 15);

        wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
        CustomButton* btnYes = new CustomButton(this, wxID_ANY, "Yes", wxDefaultPosition, wxSize(200, 50));
        btnYes->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        CustomButton* btnCancel = new CustomButton(this, wxID_ANY, "Cancel", wxDefaultPosition, wxSize(200, 50));
        btnCancel->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        hbox->Add(btnYes, 1, wxALL, 5);
        hbox->Add(btnCancel, 1, wxALL, 5);
        vbox->Add(hbox, 0, wxALIGN_CENTER);

        SetSizerAndFit(vbox);
        CentreOnParent();

        Bind(wxEVT_BUTTON, [=](wxCommandEvent& evt) {
            if (evt.GetEventObject() == btnYes)
                EndModal(1);
            else if (evt.GetEventObject() == btnCancel)
                EndModal(2);
            });
    }
};

class DisplayFlashcard : public wxDialog {
public:
    DisplayFlashcard(wxWindow* parent, const wxString& question)
        : wxDialog(parent, wxID_ANY, "Flashcard", wxDefaultPosition, wxSize(600, 600)) {
        SetBackgroundColour(wxColour(30, 0, 60));

        wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

        wxStaticText* questionLabel = new wxStaticText(this, wxID_ANY, question);
        questionLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        questionLabel->SetForegroundColour(wxColour(255, 255, 255));
        vbox->Add(questionLabel, 0, wxALL | wxALIGN_CENTER, 15);

        wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
        CustomButton* btnNext = new CustomButton(this, wxID_ANY, "Next", wxDefaultPosition, wxSize(200, 50));
        btnNext->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        CustomButton* btnRemove = new CustomButton(this, wxID_ANY, "Remove", wxDefaultPosition, wxSize(200, 50));
        btnRemove->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        hbox->Add(btnNext, 1, wxALL, 5);
        hbox->Add(btnRemove, 1, wxALL, 5);
        vbox->Add(hbox, 0, wxALIGN_CENTER);

        SetSizerAndFit(vbox);
        CentreOnParent();

        Bind(wxEVT_BUTTON, [=](wxCommandEvent& evt) {
            if (evt.GetEventObject() == btnNext)
                EndModal(1);
            else if (evt.GetEventObject() == btnRemove)
                EndModal(2);
            });
    }
};

class QuestionDialog : public wxDialog {
public:
    QuestionDialog(wxWindow* parent, const wxString& question)
        : wxDialog(parent, wxID_ANY, "Question", wxPoint(450, 350), wxSize(600, 400)) {
        SetBackgroundColour(wxColour(30, 0, 60));
        SetForegroundColour(wxColour(255, 255, 255));
        SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
        vbox->Add(new wxStaticText(this, wxID_ANY, question), 0, wxALL | wxALIGN_CENTER, 15);
        wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
        CustomButton* btnShowAnswer = new CustomButton(this, wxID_ANY, "Show answer", wxDefaultPosition, wxSize(100, 40));
        btnShowAnswer->SetFont(wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        CustomButton* btnEndSession = new CustomButton(this, wxID_ANY, "End session", wxDefaultPosition, wxSize(100, 40));
        btnEndSession->SetFont(wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        hbox->Add(btnShowAnswer, 1, wxALL, 5);
        hbox->Add(btnEndSession, 1, wxALL, 5);

        vbox->Add(hbox, 0, wxALIGN_CENTER);

        SetSizerAndFit(vbox);

        Bind(wxEVT_BUTTON, [=](wxCommandEvent& evt) {
            if (evt.GetEventObject() == btnShowAnswer)
                EndModal(1);
            else if (evt.GetEventObject() == btnEndSession)
                EndModal(2);
            });
    }
};

class FiveButtonDialog : public wxDialog {
public:
    FiveButtonDialog(wxWindow* parent, const wxString& question)
        : wxDialog(parent, wxID_ANY, "How well do you know the flashcard?", wxDefaultPosition, wxSize(600, 400)) {
        SetBackgroundColour(wxColour(30, 0, 60));
        SetForegroundColour(wxColour(255, 255, 255));
        SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
        vbox->Add(new wxStaticText(this, wxID_ANY, question), 0, wxALL | wxALIGN_CENTER, 15);
        wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

        CustomButton* btnBad = new CustomButton(this, wxID_ANY, "Bad", wxDefaultPosition, wxSize(100, 40));
        btnBad->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        CustomButton* btnOK = new CustomButton(this, wxID_ANY, "OK", wxDefaultPosition, wxSize(100, 40));
        btnOK->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        CustomButton* btnGreat = new CustomButton(this, wxID_ANY, "Great", wxDefaultPosition, wxSize(100, 40));
        btnGreat->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        CustomButton* btnEasy = new CustomButton(this, wxID_ANY, "Easy", wxDefaultPosition, wxSize(100, 40));
        btnEasy->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        CustomButton* btnEndSession = new CustomButton(this, wxID_ANY, "End session", wxDefaultPosition, wxSize(100, 40));
        btnEndSession->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        hbox->Add(btnBad, 1, wxALL, 5);
        hbox->Add(btnOK, 1, wxALL, 5);
        hbox->Add(btnGreat, 1, wxALL, 5);
        hbox->Add(btnEasy, 1, wxALL, 5);
        hbox->Add(btnEndSession, 1, wxALL, 5);

        vbox->Add(hbox, 0, wxALIGN_CENTER);

        SetSizerAndFit(vbox);

        Bind(wxEVT_BUTTON, [=](wxCommandEvent& evt) {
            if (evt.GetEventObject() == btnBad)
                EndModal(1);
            else if (evt.GetEventObject() == btnOK)
                EndModal(2);
            else if (evt.GetEventObject() == btnGreat)
                EndModal(3);
            else if (evt.GetEventObject() == btnEasy)
                EndModal(4);
            else if (evt.GetEventObject() == btnEndSession)
                EndModal(5);
            });
    }
};

#endif