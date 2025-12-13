#include <gtest/gtest.h>
#include "Note.h"
#include "Collection.h"
#include "ImportantCollection.h"
#include "CollectionCounter.h"

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        workColl = new Collection("Lavoro");
        personalColl = new Collection("Personale");
        importantColl = new ImportantCollection();
        
        workCounter = new CollectionCounter(workColl);
        personalCounter = new CollectionCounter(personalColl);
        importantCounter = new CollectionCounter(importantColl);
        
        workColl->addObserver(workCounter);
        personalColl->addObserver(personalCounter);
        importantColl->addObserver(importantCounter);
    }
    
    void TearDown() override {
        delete workColl;
        delete personalColl;
        delete importantColl;
        delete workCounter;
        delete personalCounter;
        delete importantCounter;
    }
    
    Collection* workColl;
    Collection* personalColl;
    ImportantCollection* importantColl;
    CollectionCounter* workCounter;
    CollectionCounter* personalCounter;
    CollectionCounter* importantCounter;
};

TEST_F(IntegrationTest, CompleteWorkflow) {
    Note* meeting = new Note("Meeting", "Riunione alle 15:00");
    Note* shopping = new Note("Spesa", "Latte, pane, uova");
    Note* deadline = new Note("Deadline", "Consegnare progetto");
    
    workColl->addNote(meeting);
    workColl->addNote(deadline);
    personalColl->addNote(shopping);
    
    EXPECT_EQ(workColl->size(), 2);
    EXPECT_EQ(personalColl->size(), 1);
    
    deadline->setImportant(true);
    importantColl->addNote(deadline);
    
    EXPECT_EQ(importantColl->size(), 1);
    
    deadline->lock();
    EXPECT_TRUE(deadline->isLocked());
    
    EXPECT_THROW(
        workColl->removeNote(deadline),
        std::runtime_error
    );
    
    deadline->unlock();
    workColl->removeNote(deadline);
    importantColl->removeNote(deadline);
    
    EXPECT_EQ(workColl->size(), 1);
    EXPECT_EQ(importantColl->size(), 0);
    
    delete meeting;
    delete shopping;
    delete deadline;
}

TEST_F(IntegrationTest, ImportantCollectionOnlyAcceptsImportantNotes) {
    Note* regular = new Note("Regular", "Nota normale");
    Note* important = new Note("Important", "Nota importante");
    
    important->setImportant(true);
    
    EXPECT_THROW(
        importantColl->addNote(regular),
        std::runtime_error
    );
    
    EXPECT_NO_THROW(
        importantColl->addNote(important)
    );
    
    EXPECT_EQ(importantColl->size(), 1);
    
    delete regular;
    delete important;
}

TEST_F(IntegrationTest, NoteInMultipleCollections) {
    Note* sharedNote = new Note("Shared", "Nota condivisa");
    
    workColl->addNote(sharedNote);
    personalColl->addNote(sharedNote);
    
    EXPECT_EQ(workColl->size(), 1);
    EXPECT_EQ(personalColl->size(), 1);
    
    workColl->removeNote(sharedNote);
    
    EXPECT_EQ(workColl->size(), 0);
    EXPECT_EQ(personalColl->size(), 1);
    
    delete sharedNote;
}

TEST_F(IntegrationTest, LockedNoteCannotBeModified) {
    Note* note = new Note("Protected", "Contenuto protetto");
    workColl->addNote(note);
    
    note->lock();
    
    EXPECT_THROW(
        note->setTitle("Nuovo titolo"),
        std::runtime_error
    );
    
    EXPECT_THROW(
        note->setText("Nuovo testo"),
        std::runtime_error
    );
    
    EXPECT_THROW(
        workColl->removeNote(note),
        std::runtime_error
    );
    
    note->unlock();
    EXPECT_NO_THROW(note->setText("Nuovo testo"));
    
    delete note;
}

TEST(EdgeCaseTest, EmptyCollectionOperations) {
    Collection coll("Empty");
    Note note("Test", "Text");
    
    EXPECT_THROW(
        coll.removeNote(&note),
        std::runtime_error
    );
    
    EXPECT_EQ(coll.size(), 0);
}

TEST(EdgeCaseTest, AddRemoveAddSameNote) {
    Collection coll("Test");
    Note note("Test", "Text");
    
    coll.addNote(&note);
    coll.removeNote(&note);
    coll.addNote(&note);
    
    EXPECT_EQ(coll.size(), 1);
}