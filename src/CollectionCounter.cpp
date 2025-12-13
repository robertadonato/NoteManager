#include "CollectionCounter.h"

CollectionCounter::CollectionCounter(const Collection* collection)
    : collection(collection) {
}

void CollectionCounter::update() {
    std::cout << "[Counter] Collezione '" << collection->getName() 
              << "' contiene ora " << collection->size() 
              << " note" << std::endl;
}