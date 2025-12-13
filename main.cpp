#include "NoteManager.h"
#include <iostream>
#include <exception>

int main() {
    try {
        NoteManager manager;
        manager.run();
        return 0;
    } catch (const std::bad_alloc& e) {
        std::cerr << "Errore: Memoria insufficiente - " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Errore critico nel programma: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Errore critico sconosciuto nel programma" << std::endl;
        return 1;
    }
}