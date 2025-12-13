#ifndef COLLECTION_H
#define COLLECTION_H

#include "Subject.h"
#include "Note.h"
#include <string>
#include <vector>

class Collection : public Subject {
public:
    explicit Collection(const std::string& name);
    virtual ~Collection();

    const std::string& getName() const;

    virtual void addNote(Note* n);
    virtual void removeNote(Note* n);

    const std::vector<Note*>& getNotes() const;
    int size() const;

    void addObserver(Observer* o) override;
    void removeObserver(Observer* o) override;
    void notifyObservers() const override;

private:
    std::string name;
    std::vector<Note*> notes;
    std::vector<Observer*> observers;
};

#endif