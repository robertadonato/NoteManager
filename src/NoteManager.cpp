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
                case 9: handleSearchByText(); break;
                case 10: handleFilterByLocked(); break;
                case 11: handleFilterByCollection(); break;
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
    std::cout << "9. Cerca note per testo\n";
    std::cout << "10. Filtra note bloccate/sbloccate\n";
    std::cout << "11. Filtra per collezione\n";
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

void NoteManager::handleSearchByText() const {
    std::string query;
    std::cout << "Testo da cercare: ";
    std::getline(std::cin, query);
    
    auto results = searchByText(query);
    displayNotes(results, "Risultati ricerca per: '" + query + "'");
}

void NoteManager::handleFilterByLocked() const {
    int choice;
    std::cout << "Filtra per:\n";
    std::cout << "1. Note bloccate\n";
    std::cout << "2. Note sbloccate\n";
    std::cout << "Scelta: ";
    std::cin >> choice;
    std::cin.ignore();
    
    if (choice == 1) {
        auto lockedNotes = filterByLocked(true);
        displayNotes(lockedNotes, "Note bloccate");
    } else if (choice == 2) {
        auto unlockedNotes = filterByLocked(false);
        displayNotes(unlockedNotes, "Note sbloccate");
    } else {
        std::cout << "Scelta non valida\n";
    }
}

void NoteManager::handleFilterByCollection() const {
    std::string collName;
    std::cout << "Nome collezione: ";
    std::getline(std::cin, collName);
    
    auto filteredNotes = filterByCollection(collName);
    displayNotes(filteredNotes, "Note nella collezione: " + collName);
}

void NoteManager::displayNotes(const std::vector<std::shared_ptr<Note>>& notes, const std::string& title) const {
    if (notes.empty()) {
        std::cout << "\n=== " << title << " ===\n";
        std::cout << "Nessuna nota trovata.\n";
        return;
    }
    
    std::cout << "\n=== " << title << " ===\n";
    std::cout << "Trovate " << notes.size() << " note:\n";
    
    for (const auto& n : notes) {
        std::cout << "\nTitolo: " << n->getTitle();
        std::cout << "\nTesto: " << n->getText();
        std::cout << "\nBloccata: " << (n->isLocked() ? "Si" : "No");
        std::cout << "\nImportante: " << (n->isImportant() ? "Si" : "No");
        std::cout << "\n---\n";
    }
}

std::vector<std::shared_ptr<Note>> NoteManager::searchByText(const std::string& query) const {
    std::vector<std::shared_ptr<Note>> results;
    
    if (query.empty()) {
        return results;
    }
    
    // Converti la query in lowercase per ricerca case-insensitive
    std::string queryLower = query;
    std::transform(queryLower.begin(), queryLower.end(), queryLower.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    
    for (const auto& note : allNotes) {
        std::string title = note->getTitle();
        std::string text = note->getText();
        
        // Converti titolo e testo in lowercase
        std::string titleLower = title;
        std::string textLower = text;
        std::transform(titleLower.begin(), titleLower.end(), titleLower.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        std::transform(textLower.begin(), textLower.end(), textLower.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        
        // Cerca la query nel titolo o nel testo
        if (titleLower.find(queryLower) != std::string::npos || 
            textLower.find(queryLower) != std::string::npos) {
            results.push_back(note);
        }
    }
    
    return results;
}

std::vector<std::shared_ptr<Note>> NoteManager::filterByLocked(bool locked) const {
    std::vector<std::shared_ptr<Note>> results;
    
    for (const auto& note : allNotes) {
        if (note->isLocked() == locked) {
            results.push_back(note);
        }
    }
    
    return results;
}

std::vector<std::shared_ptr<Note>> NoteManager::filterByCollection(const std::string& collName) const {
    std::vector<std::shared_ptr<Note>> results;
    
    // Trova la collezione
    std::shared_ptr<Collection> targetColl = nullptr;
    for (const auto& coll : collections) {
        if (coll->getName() == collName) {
            targetColl = coll;
            break;
        }
    }
    
    if (!targetColl) {
        std::cout << "Collezione '" << collName << "' non trovata.\n";
        return results;
    }
    
    // Ottieni tutte le note dalla collezione
    const auto& notesInCollection = targetColl->getNotes();
    
    // Aggiungi le note ai risultati
    for (const auto& note : notesInCollection) {
        results.push_back(note);
    }
    
    return results;
}