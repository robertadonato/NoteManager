#include "ImportantCollection.h"
#include <stdexcept>

ImportantCollection::ImportantCollection() : Collection("Important") {}

void ImportantCollection::addNote(Note* n) {
    if (n == nullptr) {
        throw std::invalid_argument("Impossibile aggiungere una nota nulla");
    }
    
    if (!n->isImportant()) {
        throw std::runtime_error("Solo note importanti possono essere aggiunte a questa collezione");
    }

    Collection::addNote(n);
}