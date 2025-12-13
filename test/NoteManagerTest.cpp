#include <gtest/gtest.h>
#include <sstream>
#include <memory>
#include "NoteManager.h"

class NoteManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        manager = std::make_unique<NoteManager>();
    }
    
    std::unique_ptr<NoteManager> manager;
};

TEST_F(NoteManagerTest, Constructor) {
    EXPECT_NO_THROW(NoteManager());
}

TEST(NoteManagerDestructorTest, NoMemoryLeak) {
    for (int i = 0; i < 100; ++i) {
        auto mgr = std::make_unique<NoteManager>();
    }
    SUCCEED();
}

TEST(NoteManagerRunTest, ExitsCleanly) {
    std::istringstream input("0\n");
    std::ostringstream output;
    
    std::streambuf* cinbuf = std::cin.rdbuf();
    std::streambuf* coutbuf = std::cout.rdbuf();
    
    std::cin.rdbuf(input.rdbuf());
    std::cout.rdbuf(output.rdbuf());
    
    NoteManager manager;
    EXPECT_NO_THROW(manager.run());
    
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
}

TEST(NoteManagerIntegrationTest, AddAndDisplayNote) {
    std::istringstream input("1\nTest Note\nTest Text\nWork\n6\n0\n");
    std::ostringstream output;
    
    std::streambuf* cinbuf = std::cin.rdbuf();
    std::streambuf* coutbuf = std::cout.rdbuf();
    
    std::cin.rdbuf(input.rdbuf());
    std::cout.rdbuf(output.rdbuf());
    
    NoteManager manager;
    manager.run();
    
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
    
    std::string result = output.str();
    
    EXPECT_TRUE(result.find("Test Note") != std::string::npos);
    EXPECT_TRUE(result.find("Test Text") != std::string::npos);
}

TEST(NoteManagerIntegrationTest, AddAndRemoveNote) {
    std::istringstream input(
        "1\nRemoveMe\nText\nWork\n"
        "2\nRemoveMe\n"
        "6\n"
        "0\n"
    );
    std::ostringstream output;
    
    std::streambuf* cinbuf = std::cin.rdbuf();
    std::streambuf* coutbuf = std::cout.rdbuf();
    
    std::cin.rdbuf(input.rdbuf());
    std::cout.rdbuf(output.rdbuf());
    
    NoteManager manager;
    manager.run();
    
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
    
    std::string result = output.str();
    
    EXPECT_TRUE(result.find("Nota rimossa") != std::string::npos);
}

TEST(NoteManagerIntegrationTest, LockNote) {
    std::istringstream input(
        "1\nLockMe\nText\nWork\n"
        "4\nLockMe\n"
        "6\n"
        "0\n"
    );
    std::ostringstream output;
    
    std::streambuf* cinbuf = std::cin.rdbuf();
    std::streambuf* coutbuf = std::cout.rdbuf();
    
    std::cin.rdbuf(input.rdbuf());
    std::cout.rdbuf(output.rdbuf());
    
    NoteManager manager;
    manager.run();
    
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
    
    std::string result = output.str();
    
    EXPECT_TRUE(result.find("Nota bloccata") != std::string::npos);
    EXPECT_TRUE(result.find("Bloccata: Si") != std::string::npos);
}

TEST(NoteManagerIntegrationTest, MarkImportant) {
    std::istringstream input(
        "1\nImportant\nText\nWork\n"
        "5\nImportant\n"
        "8\n"
        "0\n"
    );
    std::ostringstream output;
    
    std::streambuf* cinbuf = std::cin.rdbuf();
    std::streambuf* coutbuf = std::cout.rdbuf();
    
    std::cin.rdbuf(input.rdbuf());
    std::cout.rdbuf(output.rdbuf());
    
    NoteManager manager;
    manager.run();
    
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
    
    std::string result = output.str();
    
    EXPECT_TRUE(result.find("segnata come importante") != std::string::npos);
    EXPECT_TRUE(result.find("Important") != std::string::npos);
}

TEST(NoteManagerIntegrationTest, DisplayCollection) {
    std::istringstream input(
        "1\nNote1\nText1\nWork\n"
        "1\nNote2\nText2\nPersonal\n"
        "7\nWork\n"
        "0\n"
    );
    std::ostringstream output;
    
    std::streambuf* cinbuf = std::cin.rdbuf();
    std::streambuf* coutbuf = std::cout.rdbuf();
    
    std::cin.rdbuf(input.rdbuf());
    std::cout.rdbuf(output.rdbuf());
    
    NoteManager manager;
    manager.run();
    
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
    
    std::string result = output.str();
    
    EXPECT_TRUE(result.find("Work") != std::string::npos);
    EXPECT_TRUE(result.find("Note1") != std::string::npos);
}

TEST(NoteManagerRobustnessTest, InvalidMenuChoice) {
    std::istringstream input("99\n0\n");
    std::ostringstream output;
    
    std::streambuf* cinbuf = std::cin.rdbuf();
    std::streambuf* coutbuf = std::cout.rdbuf();
    
    std::cin.rdbuf(input.rdbuf());
    std::cout.rdbuf(output.rdbuf());
    
    NoteManager manager;
    EXPECT_NO_THROW(manager.run());
    
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
    
    std::string result = output.str();
    EXPECT_TRUE(result.find("non valida") != std::string::npos);
}

TEST(NoteManagerObserverTest, CounterNotifications) {
    std::istringstream input(
        "1\nNote1\nText\nWork\n"
        "1\nNote2\nText\nWork\n"
        "0\n"
    );
    std::ostringstream output;
    
    std::streambuf* cinbuf = std::cin.rdbuf();
    std::streambuf* coutbuf = std::cout.rdbuf();
    
    std::cin.rdbuf(input.rdbuf());
    std::cout.rdbuf(output.rdbuf());
    
    NoteManager manager;
    manager.run();
    
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
    
    std::string result = output.str();
    
    EXPECT_TRUE(result.find("[Counter]") != std::string::npos);
    EXPECT_TRUE(result.find("contiene ora 1 note") != std::string::npos);
    EXPECT_TRUE(result.find("contiene ora 2 note") != std::string::npos);
}