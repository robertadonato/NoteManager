#include <gtest/gtest.h>
#include "Collection.h"
#include "Note.h"

class CollectionTest : public ::testing::Test {
protected:
    void SetUp() override {
        note1 = new Note("Nota1", "Testo1");
        note2 = new Note("Nota2", "Testo2");
        note3 = new Note("Nota3", "Testo3");
        coll = new Collection("Nome collezione");
    }
    
    void TearDown() override {
        delete note1;
        delete note2;
        delete note3;
        delete coll;
    }
    
    Note* note1;
    Note* note2;
    Note* note3;
    Collection* coll;
};

TEST(CollectionConstructorTest, ValidName) {
    Collection coll("Test");
    EXPECT_EQ(coll.getName(), "Test");
    EXPECT_EQ(coll.size(), 0);
}

TEST(CollectionConstructorTest, EmptyName) {
    EXPECT_THROW(
        Collection(""),
        std::invalid_argument
    );
}

TEST_F(CollectionTest, AddSingleNote) {
    coll->addNote(note1);
    EXPECT_EQ(coll->size(), 1);
}

TEST_F(CollectionTest, AddMultipleNotes) {
    coll->addNote(note1);
    coll->addNote(note2);
    coll->addNote(note3);
    
    EXPECT_EQ(coll->size(), 3);
}

TEST_F(CollectionTest, AddNullNote) {
    EXPECT_THROW(
        coll->addNote(nullptr),
        std::invalid_argument
    );
}

TEST_F(CollectionTest, AddDuplicateNote) {
    coll->addNote(note1);
    
    EXPECT_THROW(
        coll->addNote(note1),
        std::runtime_error
    );
}

TEST_F(CollectionTest, RemoveNote) {
    coll->addNote(note1);
    coll->addNote(note2);
    
    coll->removeNote(note1);
    
    EXPECT_EQ(coll->size(), 1);
}

TEST_F(CollectionTest, RemoveNullNote) {
    EXPECT_THROW(
        coll->removeNote(nullptr),
        std::invalid_argument
    );
}

TEST_F(CollectionTest, RemoveNonExistentNote) {
    coll->addNote(note1);
    
    EXPECT_THROW(
        coll->removeNote(note2),
        std::runtime_error
    );
}

TEST_F(CollectionTest, RemoveLockedNote) {
    coll->addNote(note1);
    note1->lock();
    
    EXPECT_THROW(
        coll->removeNote(note1),
        std::runtime_error
    );
}

TEST_F(CollectionTest, GetNotes) {
    coll->addNote(note1);
    coll->addNote(note2);
    
    const std::vector<Note*>& notes = coll->getNotes();
    
    EXPECT_EQ(notes.size(), 2);
    EXPECT_EQ(notes[0], note1);
    EXPECT_EQ(notes[1], note2);
}

TEST_F(CollectionTest, EmptyCollection) {
    EXPECT_EQ(coll->size(), 0);
    EXPECT_TRUE(coll->getNotes().empty());
}