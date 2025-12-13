#include <gtest/gtest.h>
#include <memory>
#include "Note.h"
#include "Collection.h"
#include "ImportantCollection.h"
#include "CollectionCounter.h"

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        workColl = std::make_shared<Collection>("Lavoro");
        personalColl = std::make_shared<Collection>("Personale");
        importantColl = std::make_shared<ImportantCollection>();
        
        workCounter = std::make_shared<CollectionCounter>(workColl);
        personalCounter = std::make_shared<CollectionCounter>(personalColl);
        importantCounter = std::make_shared<CollectionCounter>(importantColl);
        
        workColl->addObserver(workCounter.get());
        personalColl->addObserver(personalCounter.get());
        importantColl->addObserver(importantCounter.get());
    }
    
    std::shared_ptr<Collection> workColl;
    std::shared_ptr<Collection> personalColl;
    std::shared_ptr<ImportantCollection> importantColl;
    std::shared_ptr<CollectionCounter> workCounter;
    std::shared_ptr<CollectionCounter> personalCounter;
    std::shared_ptr<CollectionCounter> importantCounter;
};

TEST_F(IntegrationTest, CompleteWorkflow) {
    auto meeting = std::make_shared<Note>("Meeting", "Riunione alle 15:00");
    auto shopping = std::make_shared<Note>("Spesa", "Latte, pane, uova");
    auto deadline = std::make_shared<Note>("Deadline", "Consegnare progetto");
    
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
}

TEST_F(IntegrationTest, ImportantCollectionOnlyAcceptsImportantNotes) {
    auto regular = std::make_shared<Note>("Regular", "Nota normale");
    auto important = std::make_shared<Note>("Important", "Nota importante");
    
    important->setImportant(true);
    
    EXPECT_THROW(
        importantColl->addNote(regular),
        std::runtime_error
    );
    
    EXPECT_NO_THROW(
        importantColl->addNote(important)
    );
    
    EXPECT_EQ(importantColl->size(), 1);
}

TEST_F(IntegrationTest, NoteInMultipleCollections) {
    auto sharedNote = std::make_shared<Note>("Shared", "Nota condivisa");
    
    workColl->addNote(sharedNote);
    personalColl->addNote(sharedNote);
    
    EXPECT_EQ(workColl->size(), 1);
    EXPECT_EQ(personalColl->size(), 1);
    
    workColl->removeNote(sharedNote);
    
    EXPECT_EQ(workColl->size(), 0);
    EXPECT_EQ(personalColl->size(), 1);
}

TEST_F(IntegrationTest, LockedNoteCannotBeModified) {
    auto note = std::make_shared<Note>("Protected", "Contenuto protetto");
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
}

TEST(EdgeCaseTest, EmptyCollectionOperations) {
    auto coll = std::make_shared<Collection>("Empty");
    auto note = std::make_shared<Note>("Test", "Text");
    
    EXPECT_THROW(
        coll->removeNote(note),
        std::runtime_error
    );
    
    EXPECT_EQ(coll->size(), 0);
}

TEST(EdgeCaseTest, AddRemoveAddSameNote) {
    auto coll = std::make_shared<Collection>("Test");
    auto note = std::make_shared<Note>("Test", "Text");
    
    coll->addNote(note);
    coll->removeNote(note);
    coll->addNote(note);
    
    EXPECT_EQ(coll->size(), 1);
}