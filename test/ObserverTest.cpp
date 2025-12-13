#include <gtest/gtest.h>
#include <sstream>
#include "Collection.h"
#include "CollectionCounter.h"
#include "Note.h"

class MockObserver : public Observer {
public:
    MockObserver() : updateCount(0) {}
    
    void update() override {
        updateCount++;
    }
    
    int getUpdateCount() const {
        return updateCount;
    }
    
    void reset() {
        updateCount = 0;
    }
    
private:
    int updateCount;
};

class ObserverTest : public ::testing::Test {
protected:
    void SetUp() override {
        coll = new Collection("Test");
        observer = new MockObserver();
        note1 = new Note("Nota1", "Testo1");
        note2 = new Note("Nota2", "Testo2");
    }
    
    void TearDown() override {
        delete coll;
        delete observer;
        delete note1;
        delete note2;
    }
    
    Collection* coll;
    MockObserver* observer;
    Note* note1;
    Note* note2;
};

TEST_F(ObserverTest, AddObserver) {
    coll->addObserver(observer);
    
    coll->addNote(note1);
    
    EXPECT_EQ(observer->getUpdateCount(), 1);
}

TEST_F(ObserverTest, MultipleNotifications) {
    coll->addObserver(observer);
    
    coll->addNote(note1);
    coll->addNote(note2);
    coll->removeNote(note1);
    
    EXPECT_EQ(observer->getUpdateCount(), 3);
}

TEST_F(ObserverTest, RemoveObserver) {
    coll->addObserver(observer);
    coll->addNote(note1);
    
    observer->reset();
    coll->removeObserver(observer);
    
    coll->addNote(note2);
    
    EXPECT_EQ(observer->getUpdateCount(), 0);
}

TEST_F(ObserverTest, MultipleObservers) {
    MockObserver* observer2 = new MockObserver();
    
    coll->addObserver(observer);
    coll->addObserver(observer2);
    
    coll->addNote(note1);
    
    EXPECT_EQ(observer->getUpdateCount(), 1);
    EXPECT_EQ(observer2->getUpdateCount(), 1);
    
    delete observer2;
}

TEST_F(ObserverTest, NoNotificationWithoutObserver) {
    coll->addNote(note1);
    EXPECT_EQ(coll->size(), 1);
}

TEST(CollectionCounterTest, PrintsCorrectMessage) {
    Collection coll("TestColl");
    CollectionCounter counter(&coll);
    coll.addObserver(&counter);
    
    Note note("Test", "Contenuto");

    testing::internal::CaptureStdout();
    coll.addNote(&note);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("TestColl") != std::string::npos);
    EXPECT_TRUE(output.find("1") != std::string::npos);
    EXPECT_TRUE(output.find("note") != std::string::npos);
}

TEST(CollectionCounterTest, UpdatesOnRemove) {
    Collection coll("TestColl");
    CollectionCounter counter(&coll);
    coll.addObserver(&counter);
    
    Note note("Test", "Contenuto");
    coll.addNote(&note);
    
    testing::internal::CaptureStdout();
    coll.removeNote(&note);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("0") != std::string::npos);
}