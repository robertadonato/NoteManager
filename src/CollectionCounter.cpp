#include "CollectionCounter.h"

CollectionCounter::CollectionCounter(std::shared_ptr<Collection> collection)
    : collection(collection) {
}

void CollectionCounter::update() {
    if (auto coll = collection.lock()) {
        std::cout << "[Counter] Collezione '" << coll->getName() 
                << "' contiene ora " << coll->size() 
                << " note" << std::endl;
    }            
}