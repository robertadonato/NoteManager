#include "Note.h"
#include "Collection.h"
#include "ImportantCollection.h"
#include "CollectionCounter.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

void displayMenu() {
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

int main() {
    std::vector<Note*> allNotes;
    std::vector<Collection*> collections;
    ImportantCollection importantColl;
    
    CollectionCounter importantCounter(&importantColl);
    importantColl.addObserver(&importantCounter);
    
    std::vector<CollectionCounter*> counters;
    
    int choice;
    std::string input;
    
    while (true) {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore();
        
        try {
            if (choice == 0) {
                break;
            }
            
            switch (choice) {
                case 1: {
                    std::string title, text, collName;
                    std::cout << "Titolo: ";
                    std::getline(std::cin, title);
                    std::cout << "Testo: ";
                    std::getline(std::cin, text);
                    std::cout << "Nome collezione: ";
                    std::getline(std::cin, collName);
                    
                    Note* note = new Note(title, text);
                    allNotes.push_back(note);
                    
                    Collection* coll = nullptr;
                    for (Collection* c : collections) {
                        if (c->getName() == collName) {
                            coll = c;
                            break;
                        }
                    }
                    
                    if (coll == nullptr) {
                        coll = new Collection(collName);
                        collections.push_back(coll);
                        
                        CollectionCounter* counter = new CollectionCounter(coll);
                        coll->addObserver(counter);
                        counters.push_back(counter);
                    }
                    
                    coll->addNote(note);
                    std::cout << "Nota aggiunta!\n";
                    break;
                }
                
                case 2: {
                    std::string title;
                    std::cout << "Titolo nota da rimuovere: ";
                    std::getline(std::cin, title);
                    
                    Note* toRemove = nullptr;
                    for (Note* n : allNotes) {
                        if (n->getTitle() == title) {
                            toRemove = n;
                            break;
                        }
                    }
                    
                    if (toRemove == nullptr) {
                        std::cout << "Nota non trovata\n";
                        break;
                    }
                    
                    for (Collection* c : collections) {
                        try {
                            c->removeNote(toRemove);
                        } catch (...) {}
                    }
                    
                    if (toRemove->isImportant()) {
                        try {
                            importantColl.removeNote(toRemove);
                        } catch (...) {}
                    }
                    
                    allNotes.erase(
                        std::remove(allNotes.begin(), allNotes.end(), toRemove),
                        allNotes.end()
                    );
                    delete toRemove;
                    
                    std::cout << "Nota rimossa!\n";
                    break;
                }
                
                case 3: {
                    std::string title, newText;
                    std::cout << "Titolo nota: ";
                    std::getline(std::cin, title);
                    std::cout << "Nuovo testo: ";
                    std::getline(std::cin, newText);
                    
                    for (Note* n : allNotes) {
                        if (n->getTitle() == title) {
                            n->setText(newText);
                            std::cout << "Nota modificata!\n";
                            break;
                        }
                    }
                    break;
                }
                
                case 4: {
                    std::string title;
                    std::cout << "Titolo nota: ";
                    std::getline(std::cin, title);
                    
                    for (Note* n : allNotes) {
                        if (n->getTitle() == title) {
                            if (n->isLocked()) {
                                n->unlock();
                                std::cout << "Nota sbloccata\n";
                            } else {
                                n->lock();
                                std::cout << "Nota bloccata\n";
                            }
                            break;
                        }
                    }
                    break;
                }
                
                case 5: {
                    std::string title;
                    std::cout << "Titolo nota: ";
                    std::getline(std::cin, title);
                    
                    for (Note* n : allNotes) {
                        if (n->getTitle() == title) {
                            n->setImportant(true);
                            importantColl.addNote(n);
                            std::cout << "Nota segnata come importante!\n";
                            break;
                        }
                    }
                    break;
                }
                
                case 6: {
                    std::cout << "\n=== TUTTE LE NOTE ===\n";
                    for (const Note* n : allNotes) {
                        std::cout << "\nTitolo: " << n->getTitle();
                        std::cout << "\nTesto: " << n->getText();
                        std::cout << "\nBloccata: " << (n->isLocked() ? "Si" : "No");
                        std::cout << "\nImportante: " << (n->isImportant() ? "Si" : "No");
                        std::cout << "\n---\n";
                    }
                    break;
                }
                
                case 7: {
                    std::string collName;
                    std::cout << "Nome collezione: ";
                    std::getline(std::cin, collName);
                    
                    for (const Collection* c : collections) {
                        if (c->getName() == collName) {
                            std::cout << "\n=== Collezione: " << collName << " ===\n";
                            for (const Note* n : c->getNotes()) {
                                std::cout << "- " << n->getTitle() << "\n";
                            }
                            std::cout << "Totale: " << c->size() << " note\n";
                            break;
                        }
                    }
                    break;
                }
                
                case 8: {
                    std::cout << "\n=== NOTE IMPORTANTI ===\n";
                    for (const Note* n : importantColl.getNotes()) {
                        std::cout << "- " << n->getTitle() << "\n";
                    }
                    std::cout << "Totale: " << importantColl.size() << " note\n";
                    break;
                }
            }
            
        } catch (const std::exception& e) {
            std::cerr << "Errore: " << e.what() << "\n";
        }
    }
    
    for (Note* n : allNotes) {
        delete n;
    }
    for (Collection* c : collections) {
        delete c;
    }
    for (CollectionCounter* counter : counters) {
        delete counter;
    }
    
    return 0;
}