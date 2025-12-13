#include "Note.h"
#include <stdexcept>

Note::Note(const std::string& title, const std::string& text)
    : title(title), text(text), locked(false), important(false) {
    if (title.empty()) {
        throw std::invalid_argument("Il titolo non può essere vuoto");
    }
}

const std::string& Note::getTitle() const {
    return title;
}

const std::string& Note::getText() const {
    return text;
}

void Note::setTitle(const std::string& newTitle) {
    if (locked) {
        throw std::runtime_error("Nota bloccata: impossibile modificare");
    }
    if (newTitle.empty()) {
        throw std::invalid_argument("Il titolo non può essere vuoto");
    }
    title = newTitle;
}

void Note::setText(const std::string& newText) {
    if (locked) {
        throw std::runtime_error("Nota bloccata: impossibile modificare");
    }
    text = newText;
}

void Note::lock() {
    locked = true;
}

void Note::unlock() {
    locked = false;
}

bool Note::isLocked() const {
    return locked;
}

void Note::setImportant(bool important) {
    this->important = important;
}

bool Note::isImportant() const {
    return important;
}