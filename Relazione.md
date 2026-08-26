# Simulazione numerica del modello preda-predatore di Lotka-Volterra

**Autori:** Shala Artan, Tonin Anita
**Data:** 27 Agosto 2026

## Introduzione

Il progetto implementa una simulazione numerica del sistema preda-predatore descritto dalle equazioni di Lotka-Volterra. La simulazione integra le equazioni con il metodo di Eulero simplettico, rappresentando lo stato del sistema in coordinate relative rispetto al punto di equilibrio per migliorare la stabilità numerica dell'integrazione.

Questa è la prima consegna del progetto.

## Scelte progettuali e implementative

### Struttura del progetto

Il progetto è organizzato in più file: `simulation.hpp`/`simulation.cpp` contengono la logica del modello fisico (la classe `Simulation` e gli struct `Parameters`/`State`); `input.hpp`/`input.cpp` contengono la lettura e validazione dell'input da uno stream; `main.cpp` gestisce l'interazione con l'utente e l'esecuzione della simulazione; `simulation_test.cpp` contiene i test automatici, scritti con il framework Doctest.

La separazione di `input.hpp`/`input.cpp` da `main.cpp` è motivata dalla necessità di testare la logica di lettura e validazione dei parametri in modo automatico. Le funzioni di lettura operano su un parametro generico di tipo `std::istream&` anziché direttamente su `std::cin`, permettendo di simularne il comportamento nei test con un `std::istringstream` costruito a partire da una stringa, senza richiedere input manuale da tastiera. Se si fosse mantenuta questa logica all'interno di `main.cpp` si sarebbe causato un conflitto con la funzione `main` generata automaticamente da Doctest.

Tutte le entità definite dal progetto (`Parameters`, `State`, `Simulation`, e le funzioni di lettura dell'input) sono racchiuse all'interno del namespace `lotka_volterra`, per raggrupparle sotto un nome comune e ridurre il rischio di collisioni con nomi definiti altrove.

### La classe `Simulation`

La classe `Simulation` implementa il modello fisico descritto dalle equazioni di Lotka-Volterra, integrate secondo il metodo di Eulero simplettico nella loro forma discretizzata.

Gli stati intermedi sono espressi in coordinate relative rispetto al punto di equilibrio, per risolvere un problema di instabilità numerica che si presenta quando i valori assoluti delle popolazioni sono molto maggiori dell'unità, rendendo l'aggiornamento ad ogni passo troppo grande rispetto al Δt scelto.

La classe contiene il metodo `evolve()`, che fa avanzare la simulazione di un solo passo. Si è scelto di separare le responsabilità tra `simulation.cpp` e `main.cpp`: la classe `Simulation` si occupa esclusivamente della fisica del modello, mentre il ciclo che determina quante volte invocare `evolve()` è lasciato a `main.cpp`. Questa separazione rende `Simulation` indipendente dal numero di passi che si desidera simulare e permette, in linea di principio, di ispezionare o utilizzare lo stato della simulazione tra un passo e l'altro, senza dover eseguire l'intera evoluzione in un unico blocco.

I parametri del costruttore sono stati raggruppati in due struct, `Parameters` e `State`, per evitare di passare sette valori `double` non distinguibili al costruttore, scelta che in fase di implementazione ha effettivamente causato uno scambio accidentale di due parametri (si veda la sezione "Costrutti non introdotti a lezione").

Infine, il metodo `state(i)` restituisce in un'unica chiamata il terzetto di valori (x, y, H) relativo allo stato i-esimo, invece di prevedere tre metodi distinti: questa scelta rende il codice chiamante più leggibile ed evita di triplicare, in tre metodi separati, lo stesso controllo di validità dell'indice.

### Validazione dell'input

La lettura dei parametri da `std::cin` è affidata alle funzioni `read_positive_double` e `read_positive_int` (file `input.hpp`/`input.cpp`), che verificano sia la correttezza del tipo letto sia la sua validità (valore strettamente positivo). In caso di errore viene lanciata un'eccezione `std::runtime_error`, intercettata in `main.cpp`, che stampa un messaggio descrittivo e termina il programma senza richiedere nuovamente il valore, come richiesto dalla consegna.

Anche il costruttore di `Simulation` valida i propri parametri, lanciando `std::invalid_argument` se non strettamente positivi; il metodo `state(i)` lancia invece `std::out_of_range` se l'indice richiesto non è disponibile. Si è preferito l'uso di eccezioni rispetto ad `assert` perché queste ultime non possono essere intercettate nei test.

### Costrutti non introdotti a lezione

- **Funzionalità di C++20**: il codice fa uso di due funzionalità introdotte con lo standard C++20.

  La prima sono gli inizializzatori designati (*designated initializers*), che permettono di costruire uno struct specificando esplicitamente a quale membro è assegnato ciascun valore (ad es. `Parameters{.A = 1.0, .B = 0.00125, .C = 0.001, .D = 1.0}`), invece di affidarsi al solo ordine posizionale dei valori. Questa scelta nasce da un errore concreto commesso durante lo sviluppo, in cui i parametri C e D sono stati scambiati per errore causando un'instabilità numerica nella simulazione: rendendo esplicito a quale membro appartiene ogni valore si è ridotto il rischio di questo tipo di errore.

  La seconda funzionalità è la possibilità di richiedere al compilatore la generazione automatica di un operatore di uguaglianza membro-a-membro tramite `bool operator==(...) const = default`, usata sugli struct `Parameters` e `State` per poterli confrontare direttamente nei test, senza scrivere a mano un confronto ripetitivo membro a membro.

- **Struct con metodi**: si sono costruiti struct con metodi (non solo dati): gli struct `Parameters` e `State` non solo raggruppano dati (rispettivamente i quattro parametri del modello e lo stato del sistema in un istante), ma definiscono anche un metodo, l'operatore di uguaglianza descritto sopra.

- **`std::ofstream`**: per scrivere i risultati della simulazione su file, il codice usa la classe `std::ofstream` (header `<fstream>`), che si comporta come `std::cout` ma indirizza l'output verso un file su disco invece che verso lo schermo, tramite lo stesso operatore `<<`.

- **`std::istringstream`**: nei test automatici (nel file `simulation_test.cpp`) si è voluto verificare il comportamento delle funzioni di lettura e validazione dell'input senza richiedere l'inserimento manuale da tastiera. A questo scopo le funzioni di lettura sono state scritte per accettare un parametro di tipo `std::istream&` generico, anziché operare direttamente su `std::cin`: questo permette di passare, nei test, un oggetto `std::istringstream` costruito a partire da una stringa che simula un input (valido o non valido), mentre nel programma vero e proprio si passa `std::cin`.

## Dipendenze e istruzioni di compilazione ed esecuzione

Il progetto richiede un compilatore C++ con supporto a C++20, la
libreria grafica SFML (versione 2.5 o successiva), CMake (versione
3.17 o successiva) e Ninja.

Su sistemi Debian/Ubuntu, le dipendenze si installano con:
```bash
sudo apt install libsfml-dev ninja-build
```
Per l'esecuzione dei test è inoltre necessario l'header singolo di
Doctest (`doctest.h`), scaricabile da
<https://github.com/doctest/doctest> e da posizionare nella cartella
del progetto.

La compilazione è gestita tramite CMake, con generatore Ninja
Multi-Config:
```bash
cmake -S . -B build -G "Ninja Multi-Config"
cmake --build build --config Debug
```
Per eseguire il programma:
```bash
./build/Debug/lotka_volterra
```
Per eseguire i test:
```bash
cmake --build build --config Debug --target test
```
Per generare una build ottimizzata (facoltativo):
```bash
cmake --build build --config Release
cmake --build build --config Release --target test
```

## Parametri di input e formato di output

## Risultati e loro interpretazione

## Strategia di test

## Strumenti utilizzati

## Uso di intelligenza artificiale generativa