#include "Collection.h"
#include <algorithm>
#include <stdexcept>

Collection::Collection(const std::string& name) : name(name) {
    if (name.empty()) {
        throw std::invalid_argument("Il nome della collezione non può essere vuoto");
    }
}

Collection::~Collection() {}

const std::string& Collection::getName() const {
    return name;
}

void Collection::addNote(Note* n) {
    if (n == nullptr) {
        throw std::invalid_argument("Impossibile aggiungere una nota nulla");
    }

    auto it = std::find(notes.begin(), notes.end(), n);
    if (it != notes.end()) {
        throw std::runtime_error("La nota è già presente nella collezione");
    }
    
    notes.push_back(n);
    notifyObservers();
}

void Collection::removeNote(Note* n) {
    if (n == nullptr) {
        throw std::invalid_argument("Impossibile rimuovere una nota nulla");
    }
    
    if (n->isLocked()) {
        throw std::runtime_error("Impossibile rimuovere una nota bloccata");
    }
    
    auto it = std::find(notes.begin(), notes.end(), n);
    if (it == notes.end()) {
        throw std::runtime_error("Nota non trovata nella collezione");
    }
    
    notes.erase(it);
    notifyObservers();
}

const std::vector<Note*>& Collection::getNotes() const {
    return notes;
}

int Collection::size() const {
    return notes.size();
}

void Collection::addObserver(Observer* o) {
    if (o != nullptr) {
        observers.push_back(o);
    }
}

void Collection::removeObserver(Observer* o) {
    observers.erase(
        std::remove(observers.begin(), observers.end(), o),
        observers.end()
    );
}

void Collection::notifyObservers() const {
    for (Observer* obs : observers) {
        obs->update();
    }
}