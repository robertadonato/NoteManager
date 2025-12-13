#ifndef COLLECTION_H
#define COLLECTION_H

#include "Subject.h"
#include "Note.h"
#include <string>
#include <vector>
#include <memory>
#include <list>

class Collection : public Subject {
public:
    explicit Collection(const std::string& name);
    virtual ~Collection();

    const std::string& getName() const;

    virtual void addNote(std::shared_ptr<Note> n);
    virtual void removeNote(std::shared_ptr<Note> n);

    const std::vector<std::shared_ptr<Note>>& getNotes() const;
    int size() const;

    void addObserver(Observer* o) override;
    void removeObserver(Observer* o) override;
    void notifyObservers() const override;

private:
    std::string name;
    std::vector<std::shared_ptr<Note>> notes;
    std::vector<Observer*> observers;
};

#endif