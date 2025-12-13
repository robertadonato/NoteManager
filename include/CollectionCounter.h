#ifndef COLLECTIONCOUNTER_H
#define COLLECTIONCOUNTER_H

#include "Observer.h"
#include "Collection.h"
#include <iostream>
#include <memory>

class CollectionCounter : public Observer {
public:
    explicit CollectionCounter(std::shared_ptr<Collection> collection);

    void update() override;

private:
    std::weak_ptr<Collection> collection;
};

#endif