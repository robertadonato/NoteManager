#ifndef NOTE_H
#define NOTE_H

#include <string>

class Note {
public:
    Note(const std::string& title, const std::string& text);

    const std::string& getTitle() const;
    const std::string& getText() const;

    void setTitle(const std::string& newTitle);
    void setText(const std::string& newText);

    void lock();
    void unlock();
    bool isLocked() const;

    void setImportant(bool important);
    bool isImportant() const;

private:
    std::string title;
    std::string text;
    bool locked;
    bool important;
};

#endif