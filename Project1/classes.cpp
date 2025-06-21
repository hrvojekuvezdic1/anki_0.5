#include "classes.h"

flashcard::flashcard(std::string question, std::string answer) {
    this->question = question;
    this->answer = answer;
    now = std::chrono::system_clock::now();
}
std::string flashcard::getQuestion() { return flashcard::question; }
std::string flashcard::getAnswer() { return flashcard::answer; }
std::time_t flashcard::getDateTime() { return std::chrono::system_clock::to_time_t(now); }
void flashcard::setDateTime(std::chrono::system_clock::time_point newDateTime) { now = newDateTime; }

node::node() : _flashcard("", ""), next(nullptr) {}
node::node(flashcard _flashcard) : _flashcard(_flashcard), next(nullptr) {}
node* node::getNext() { return next; }
void node::setNext(node* node) { next = node; }
flashcard& node::getFlashcard() { return _flashcard; }
void node::setFlashcard(const flashcard& fc) { _flashcard = fc; }

queue::queue(node* head) : first(head), last(head) {}
void queue::enqueue(node* newNode) {
    last->setNext(newNode);
    last = newNode;
}
node* queue::dequeue() {
    if (first == nullptr) return nullptr;
    node* first = this->first;
    this->first = this->first->getNext();
    return first;
}
void queue::sortByDateTime() {
    if (!first || !first->getNext()) return;

    for (node* i = first; i != nullptr; i = i->getNext()) {
        node* minNode = i;
        for (node* j = i->getNext(); j != nullptr; j = j->getNext()) {
            if (j->getFlashcard().getDateTime() < minNode->getFlashcard().getDateTime()) {
                minNode = j;
            }
        }

        if (minNode != i) {
            flashcard temp = i->getFlashcard();
            i->setFlashcard(minNode->getFlashcard());
            minNode->setFlashcard(temp);
        }
    }
}
void queue::push(node* newHead) {
    node* previousFirst = first;
    first = newHead;
    first->setNext(previousFirst);
}
node* queue::peek() {
    return first;
}
