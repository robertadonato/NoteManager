#include "NoteManager.h"
#include <iostream>
#include <algorithm>
#include <memory>

NoteManager::NoteManager() {
    importantColl = std::make_shared<ImportantCollection>();
    importantCounter = std::make_shared<CollectionCounter>(importantColl);
    importantColl->addObserver(importantCounter.get());
}

void NoteManager::run() {
    int choice;
    
    while (true) {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 0) break;
        
        try {
            switch (choice) {
                case 1: handleAddNote(); break;
                case 2: handleRemoveNote(); break;
                case 3: handleModifyNote(); break;
                case 4: handleLockUnlock(); break;
                case 5: handleSetImportant(); break;
                case 6: handleDisplayAll(); break;
                case 7: handleDisplayCollection(); break;
                case 8: handleDisplayImportant(); break;
                default:
                    std::cout << "Scelta non valida\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Errore: " << e.what() << "\n";
        }
    }
}

void NoteManager::displayMenu() const {
    std::cout << "\n=== GESTORE NOTE ===\n";
    std::cout << "1. Aggiungi nota\n";
    std::cout << "2. Rimuovi nota\n";
    std::cout << "3. Modifica nota\n";
    std::cout << "4. Blocca/Sblocca nota\n";
    std::cout << "5. Segna come importante\n";
    std::cout << "6. Visualizza tutte le note\n";
    std::cout << "7. Visualizza collezione\n";
    std::cout << "8. Visualizza note importanti\n";
    std::cout << "0. Esci\n";
    std::cout << "Scelta: ";
}

void NoteManager::handleAddNote() {
    std::string title, text, collName;
    
    std::cout << "Titolo: ";
    std::getline(std::cin, title);
    std::cout << "Testo: ";
    std::getline(std::cin, text);
    std::cout << "Nome collezione: ";
    std::getline(std::cin, collName);
    
    auto note = std::make_shared<Note>(title, text);
    allNotes.push_back(note);
    
    auto coll = findOrCreateCollection(collName);
    coll->addNote(note);
    
    std::cout << "Nota aggiunta!\n";
}

void NoteManager::handleRemoveNote() {
    std::string title;
    std::cout << "Titolo nota da rimuovere: ";
    std::getline(std::cin, title);
    
    auto toRemove = findNoteByTitle(title);
    if (!toRemove) {
        std::cout << "Nota non trovata\n";
        return;
    }

    for (auto& c : collections) {
        try {
            c->removeNote(toRemove);
        } catch (...) {}
    }
    
    if (toRemove->isImportant()) {
        try {
            importantColl->removeNote(toRemove);
        } catch (...) {}
    }
    
    allNotes.erase(
        std::remove(allNotes.begin(), allNotes.end(), toRemove),
        allNotes.end()
    );
    
    std::cout << "Nota rimossa!\n";
}

void NoteManager::handleModifyNote() {
    std::string title, newText;
    
    std::cout << "Titolo nota: ";
    std::getline(std::cin, title);
    std::cout << "Nuovo testo: ";
    std::getline(std::cin, newText);
    
    auto note = findNoteByTitle(title);
    if (note) {
        note->setText(newText);
        std::cout << "Nota modificata!\n";
    } else {
        std::cout << "Nota non trovata\n";
    }
}

void NoteManager::handleLockUnlock() {
    std::string title;
    std::cout << "Titolo nota: ";
    std::getline(std::cin, title);
    
    auto note = findNoteByTitle(title);
    if (note) {
        if (note->isLocked()) {
            note->unlock();
            std::cout << "Nota sbloccata\n";
        } else {
            note->lock();
            std::cout << "Nota bloccata\n";
        }
    } else {
        std::cout << "Nota non trovata\n";
    }
}

void NoteManager::handleSetImportant() {
    std::string title;
    std::cout << "Titolo nota: ";
    std::getline(std::cin, title);
    
    auto note = findNoteByTitle(title);
    if (note) {
        note->setImportant(true);
        importantColl->addNote(note);
        std::cout << "Nota segnata come importante!\n";
    } else {
        std::cout << "Nota non trovata\n";
    }
}

void NoteManager::handleDisplayAll() const {
    std::cout << "\n=== TUTTE LE NOTE ===\n";
    for (const auto& n : allNotes) {
        std::cout << "\nTitolo: " << n->getTitle();
        std::cout << "\nTesto: " << n->getText();
        std::cout << "\nBloccata: " << (n->isLocked() ? "Si" : "No");
        std::cout << "\nImportante: " << (n->isImportant() ? "Si" : "No");
        std::cout << "\n---\n";
    }
}

void NoteManager::handleDisplayCollection() const {
    std::string collName;
    std::cout << "Nome collezione: ";
    std::getline(std::cin, collName);
    
    for (const auto& c : collections) {
        if (c->getName() == collName) {
            std::cout << "\n=== Collezione: " << collName << " ===\n";
            for (const auto& n : c->getNotes()) {
                std::cout << "- " << n->getTitle() << "\n";
            }
            std::cout << "Totale: " << c->size() << " note\n";
            return;
        }
    }
    std::cout << "Collezione non trovata\n";
}

void NoteManager::handleDisplayImportant() const {
    std::cout << "\n=== NOTE IMPORTANTI ===\n";
    for (const auto& n : importantColl->getNotes()) {
        std::cout << "- " << n->getTitle() << "\n";
    }
    std::cout << "Totale: " << importantColl->size() << " note\n";
}

std::shared_ptr<Collection> NoteManager::findOrCreateCollection(const std::string& name) {
    for (const auto& c : collections) {
        if (c->getName() == name) {
            return c;
        }
    }
    
    auto coll = std::make_shared<Collection>(name);
    collections.push_back(coll);
    
    auto counter = std::make_shared<CollectionCounter>(coll);
    coll->addObserver(counter.get());
    counters.push_back(counter);
    
    return coll;
}

std::shared_ptr<Note> NoteManager::findNoteByTitle(const std::string& title) {
    for (const auto& n : allNotes) {
        if (n->getTitle() == title) {
            return n;
        }
    }
    return nullptr;
}