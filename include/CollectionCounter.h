#ifndef COLLECTIONCOUNTER_H
#define COLLECTIONCOUNTER_H

#include "Observer.h"
#include "Collection.h"
#include <iostream>

class CollectionCounter : public Observer {
public:
    explicit CollectionCounter(const Collection* collection);

    void update() override;

private:
    const Collection* collection;
};

#endif