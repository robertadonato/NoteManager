#ifndef IMPORTANTCOLLECTION_H
#define IMPORTANTCOLLECTION_H

#include "Collection.h"

class ImportantCollection : public Collection {
public:
    ImportantCollection();

    void addNote(Note* n) override;
};

#endif