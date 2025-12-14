# Note Manager
Applicazione C++ per la gestione di note personali con supporto a collezioni multiple e notifiche basate sul pattern Observer.

## Caratteristiche Principali
- Gestione Note: Creazione, modifica ed eliminazione di note con titolo e testo
- Blocco Note: Protezione delle note per prevenire modifiche o cancellazioni accidentali
- Collezioni Multiple: Organizzazione delle note in collezioni personalizzate
- Note Importanti: Collezione speciale per note prioritarie
- Ricerca Avanzata: Ricerca case-insensitive per testo (titolo + contenuto)
- Filtri Dinamici: Filtraggio per stato (bloccate/sbloccate) e per collezione
- Pattern Observer: Notifiche in tempo reale sui cambiamenti nelle collezioni
- Smart Pointers: Gestione automatica della memoria con RAII (C++17)
- CLI Intuitiva: Interfaccia a riga di comando user-friendly

## Design Patterns Implementati
Observer Pattern
- Subject: Collection notifica i cambiamenti
- Observer: CollectionCounter riceve e traccia le modifiche
RAII (Resource Acquisition Is Initialization)
- Smart Pointers: std::shared_ptr per gestione automatica della memoria
- Zero memory leaks, exception-safe code, no manual delete

## Movel View Controller
- Model: Note, Collection, ImportantCollection (logica business)
- View: CLI tramite std::cout/cin (interfaccia utente)
- Controller: NoteManager (orchestrazione e coordinamento)

## Struttura del Progetto
NoteManager/
├── 📁 include/                   # Header files
│   ├── Note.h                    # Classe nota (entity)
│   ├── Collection.h              # Collezione (Subject)
│   ├── ImportantCollection.h     # Collezione importante
│   ├── CollectionCounter.h       # Observer concreto
│   ├── NoteManager.h             # Controller principale
│   ├── Subject.h                 # Interfaccia Subject
│   └── Observer.h                # Interfaccia Observer
│
├── 📁 src/                       # Implementazioni
│   ├── Note.cpp
│   ├── Collection.cpp
│   ├── ImportantCollection.cpp
│   ├── CollectionCounter.cpp
│   └── NoteManager.cpp
│
├── 📁 test/                      # Test suite (Google Test)
│   ├── RunAllTests.cpp           # Entry point test
│   ├── NoteTest.cpp              # Unit test Note
│   ├── CollectionTest.cpp        # Unit test Collection
│   ├── ObserverTest.cpp          # Unit test Observer
│   ├── NoteManagerTest.cpp       # Integration test Manager
│   ├── IntegrationTest.cpp       # Integration test completi
│   ├── CMakeLists.txt            # Build config test
│   └── lib/googletest/           # Framework testing
│
├── main.cpp                      # Entry point applicazione
├── CMakeLists.txt                # Build configuration
├── .gitignore                    # Git ignore rules
└── README.md                     # Questo file

## Compilazione ed Esecuzione
Requisiti
- CMake 3.30 o superiore
- Compilatore C++ con supporto C++20
  - MinGW (Windows)
  - GCC/G++ (Linux)
  - Clang (macOS)
- Google Test (incluso in `test/lib/googletest/`)
Compilazione (Windows con MinGW)
powershell
# Crea directory build
mkdir build
cd build
# Configura con CMake
cmake -G "MinGW Makefiles" ..
# Compila
cmake --build .

Compilazione (Linux/macOS)
bash
# Crea directory build
mkdir build
cd build
# Configura con CMake
cmake ..
# Compila
make

Esecuzione
bash
# Esegui l'applicazione
./NoteManager        # Linux/macOS
.\NoteManager.exe    # Windows
# Esegui i test
./test/RunAllTests        # Linux/macOS
.\test\RunAllTests.exe    # Windows

## Testing
Il progetto include test con Google Test.
Copertura Test
- Unit Test: 36 test per singole classi (Note, Collection, Observer)
- Integration Test: 13 test per workflow completi (NoteManager, scenari reali)
- Totale: 49 test automatizzati

## Funzionalità CLI
Menu Principale
=== GESTORE NOTE ===
1. Aggiungi nota
2. Rimuovi nota
3. Modifica nota
4. Blocca/Sblocca nota
5. Segna come importante
6. Visualizza tutte le note
7. Visualizza collezione
8. Visualizza note importanti
9. Cerca note per testo
10. Filtra note bloccate/sbloccate
11. Filtra per collezione
0. Esci

## Funzionalità Avanzate
Ricerca per Testo (Opzione 9)
- Case-insensitive: cerca "meeting" trova anche "Meeting" e "MEETING"
- Ricerca completa: cerca sia nel titolo che nel testo della nota
Filtro Note Bloccate (Opzione 10)
- Visualizza solo note bloccate o sbloccate
- Utile per gestire protezione delle note critiche
Filtro per Collezione (Opzione 11)
- Visualizzazione dettagliata di tutte le note in una collezione
- Include metadati completi (titolo, testo, stato blocco, importanza)
- Differenza con opzione 7: Vista completa vs. vista rapida (solo titoli)

## Flusso delle Notifiche
1. Aggiunta nota → Collection notifica CollectionCounter aggiorna → Stampa: "[Counter] Collezione 'X' contiene ora N note"
2. Rimozione nota → Notifica osservatori → Contatore decrementa
3. Note bloccate → Previene modifiche/rimozioni → Sistema rimane consistente
            

## Classi Principali
|        Classe         |        Ruolo        |               Responsabilità                |
|-----------------------|---------------------|---------------------------------------------|
|  Note                 | Entity              | Rappresenta una nota (titolo, testo, stato) |
|  Collection           | Subject             | Gestisce note, notifica cambiamenti         |
|  ImportantCollection  | Specialized Subject | Collezione per note prioritarie             |
|  CollectionCounter    | Observer            | Traccia e notifica numero di note           |
|  NoteManager          | Controller          | Coordina operazioni, gestisce lifecycle     |
|  Subject              | Interface           | Contratto per oggetti osservabili           |
|  Observer             | Interface           | Contratto per osservatori                   |