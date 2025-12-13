#include <gtest/gtest.h>
#include "Note.h"

TEST(NoteTest, ConstructorValid) {
    Note note("Titolo", "Testo");
    
    EXPECT_EQ(note.getTitle(), "Titolo");
    EXPECT_EQ(note.getText(), "Testo");
    EXPECT_FALSE(note.isLocked());
    EXPECT_FALSE(note.isImportant());
}

TEST(NoteTest, ConstructorEmptyTitle) {
    EXPECT_THROW(
        Note("", "Testo"),
        std::invalid_argument
    );
}

TEST(NoteTest, SetTitle) {
    Note note("Vecchio", "Testo");
    note.setTitle("Nuovo");
    EXPECT_EQ(note.getTitle(), "Nuovo");
}

TEST(NoteTest, SetTitleEmpty) {
    Note note("Titolo", "Testo");
    EXPECT_THROW(
        note.setTitle(""),
        std::invalid_argument
    );
}

TEST(NoteTest, SetText) {
    Note note("Titolo", "Vecchio testo");
    note.setText("Nuovo testo");
    EXPECT_EQ(note.getText(), "Nuovo testo");
}

TEST(NoteTest, LockNote) {
    Note note("Test", "Contenuto");
    
    EXPECT_FALSE(note.isLocked());
    note.lock();
    EXPECT_TRUE(note.isLocked());
}

TEST(NoteTest, UnlockNote) {
    Note note("Test", "Contenuto");
    note.lock();
    note.unlock();
    EXPECT_FALSE(note.isLocked());
}

TEST(NoteTest, CannotModifyLockedTitle) {
    Note note("Test", "Contenuto");
    note.lock();
    
    EXPECT_THROW(
        note.setTitle("Nuovo titolo"),
        std::runtime_error
    );
}

TEST(NoteTest, CannotModifyLockedText) {
    Note note("Test", "Contenuto");
    note.lock();
    
    EXPECT_THROW(
        note.setText("Nuovo testo"),
        std::runtime_error
    );
}

TEST(NoteTest, SetImportant) {
    Note note("Test", "Contenuto");
    
    EXPECT_FALSE(note.isImportant());
    note.setImportant(true);
    EXPECT_TRUE(note.isImportant());
    note.setImportant(false);
    EXPECT_FALSE(note.isImportant());
}

TEST(NoteTest, MultipleLockUnlock) {
    Note note("Test", "Contenuto");
    
    note.lock();
    note.lock();
    EXPECT_TRUE(note.isLocked());
    
    note.unlock();
    EXPECT_FALSE(note.isLocked());
}