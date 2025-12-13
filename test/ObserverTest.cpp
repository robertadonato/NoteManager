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
        coll = std::make_shared<Collection>("Test");
        observer = std::make_shared<MockObserver>();
        note1 = std::make_shared<Note>("Nota1", "Testo1");
        note2 = std::make_shared<Note>("Nota2", "Testo2");
    }
    
    std::shared_ptr<Collection> coll;
    std::shared_ptr<MockObserver> observer;
    std::shared_ptr<Note> note1;
    std::shared_ptr<Note> note2;
};

TEST_F(ObserverTest, AddObserver) {
    coll->addObserver(observer.get());
    
    coll->addNote(note1);
    
    EXPECT_EQ(observer->getUpdateCount(), 1);
}

TEST_F(ObserverTest, MultipleNotifications) {
    coll->addObserver(observer.get());
    
    coll->addNote(note1);
    coll->addNote(note2);
    coll->removeNote(note1);
    
    EXPECT_EQ(observer->getUpdateCount(), 3);
}

TEST_F(ObserverTest, RemoveObserver) {
    coll->addObserver(observer.get());
    coll->addNote(note1);
    
    observer->reset();
    coll->removeObserver(observer.get());
    
    coll->addNote(note2);
    
    EXPECT_EQ(observer->getUpdateCount(), 0);
}

TEST_F(ObserverTest, MultipleObservers) {
    auto observer2 = std::make_shared<MockObserver>();
    
    coll->addObserver(observer.get());
    coll->addObserver(observer2.get());
    
    coll->addNote(note1);
    
    EXPECT_EQ(observer->getUpdateCount(), 1);
    EXPECT_EQ(observer2->getUpdateCount(), 1);
}

TEST_F(ObserverTest, NoNotificationWithoutObserver) {
    coll->addNote(note1);
    EXPECT_EQ(coll->size(), 1);
}

TEST(CollectionCounterTest, PrintsCorrectMessage) {
    auto coll = std::make_shared<Collection>("TestColl");
    auto counter = std::make_shared<CollectionCounter>(coll);
    coll->addObserver(counter.get());
    
    auto note = std::make_shared<Note>("Test", "Contenuto");

    testing::internal::CaptureStdout();
    coll->addNote(note);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("TestColl") != std::string::npos);
    EXPECT_TRUE(output.find("1") != std::string::npos);
    EXPECT_TRUE(output.find("note") != std::string::npos);
}

TEST(CollectionCounterTest, UpdatesOnRemove) {
    auto coll = std::make_shared<Collection>("TestColl");
    auto counter = std::make_shared<CollectionCounter>(coll);
    coll->addObserver(counter.get());
    
    auto note = std::make_shared<Note>("Test", "Contenuto");
    coll->addNote(note);
    
    testing::internal::CaptureStdout();
    coll->removeNote(note);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("0") != std::string::npos);
}