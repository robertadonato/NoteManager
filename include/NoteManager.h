#ifndef NOTEMANAGER_H
#define NOTEMANAGER_H

#include "Note.h"
#include "Collection.h"
#include "ImportantCollection.h"
#include "CollectionCounter.h"
#include <vector>
#include <string>
#include <memory>

class NoteManager {
public:
    NoteManager();
    ~NoteManager() = default;
    
    void run();
    
private:
    std::vector<std::shared_ptr<Note>> allNotes;
    std::vector<std::shared_ptr<Collection>> collections;
    std::shared_ptr<ImportantCollection> importantColl;
    std::vector<std::shared_ptr<CollectionCounter>> counters;
    std::shared_ptr<CollectionCounter> importantCounter;
    
    void displayMenu() const;
    void handleAddNote();
    void handleRemoveNote();
    void handleModifyNote();
    void handleLockUnlock();
    void handleSetImportant();
    void handleDisplayAll() const;
    void handleDisplayCollection() const;
    void handleDisplayImportant() const;

    void handleSearchByText() const;
    void handleFilterByLocked() const;
    void handleFilterByCollection() const;
    void displayNotes(const std::vector<std::shared_ptr<Note>>& notes, const std::string& title) const;
    
    std::vector<std::shared_ptr<Note>> searchByText(const std::string& query) const;
    std::vector<std::shared_ptr<Note>> filterByLocked(bool locked) const;
    std::vector<std::shared_ptr<Note>> filterByCollection(const std::string& collName) const;
    
    std::shared_ptr<Collection> findOrCreateCollection(const std::string& name);
    std::shared_ptr<Note> findNoteByTitle(const std::string& title);
};

#endif