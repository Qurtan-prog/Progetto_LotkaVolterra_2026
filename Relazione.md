# Simulazione numerica del modello preda-predatore di Lotka-Volterra

**Autori:** Shala Artan, Tonin Anita
**Data:** 27 Agosto 2026

## Introduzione

Il progetto implementa una simulazione numerica del sistema preda-predatore descritto dalle equazioni di Lotka-Volterra. La simulazione integra le equazioni con il metodo di Eulero simplettico, rappresentando lo stato del sistema in coordinate relative rispetto al punto di equilibrio per migliorare la stabilità numerica dell'integrazione.

Questa è la prima consegna del progetto.

## Scelte progettuali e implementative

### Struttura del progetto

Il progetto è organizzato in più file: 
- `simulation.hpp`/`simulation.cpp` in cui si torva la logica del modello fisico (la classe `Simulation` e gli struct `Parameters`/`State`); 
- `input.hpp`/`input.cpp` contengono la lettura e validazione dell'input da tastiera; 
- `plot_axes.hpp`/`plot_axes.cpp` in cui si disegnano gli assi la griglia e si seleziona il font;
- `plotter.hpp`/`plotter.cpp` crea la finestra grafica coni plot dell'andamento delle popolazioni nel tempo e dell'integrale primo H(t);
- `main.cpp` gestisce l'interazione con l'utente e l'esecuzione della simulazione; 
- `simulation_test.cpp` contiene i test automatici scritti con il framework Doctest.

La separazione di `input.hpp`/`input.cpp` da `main.cpp` è stata ritesnuta necessaria al fine di testare la logica di lettura e validazione dei parametri in modo automatico. Le funzioni di lettura che si trovano lì infatti operano su un parametro generico di tipo `std::istream&` anziché direttamente su `std::cin`, potendo quindi simularne i comportamento con un `std::istringstream` costruito a partire da una stringa, non richiedendo l'input da tastiera. Per mantenere questa logica all'interno di `main.cpp` si sarebbe ottenuto un conflitto con la funzione `main` generata automaticamente da Doctest. 
Si è cercato di spartire le responsabilità tra i file: `plot_axes.cpp` e `plotter.cpp` sono separati perchè le funzioni del primo non dipendono da nulla del secondo (gestione finestra separato da disegno di assi e scelta del font), mentre in `simulation.cpp` si ha la sola fisica del sistema, tutto quello che riguarda le scelte dell'utente (il passo dt e il numero di ripetizioni ad esempio) sono gestite nel `main.cpp`.

Tutte le entità definite dal progetto (`Parameters`, `State`, `Simulation`, e le funzioni di lettura dell'input) sono racchiuse all'interno del namespace `lotka_volterra`, per raggrupparle sotto un nome comune e ridurre il rischio di collisioni con nomi definiti altrove.

### La classe `Simulation`

La classe `Simulation` implementa il modello fisico descritto dalle equazioni di Lotka-Volterra, integrate secondo il metodo Eulero simplettico nella loro forma discretizzata, come da consegna.

I parametri del costruttore sono stati raggruppati in due struct, `Parameters` e `State`, per evitare di passare sette valori `double` non distinguibili al costruttore.

Gli stati intermedi sono espressi in coordinate relative rispetto al punto di equilibrio per avere maggiore stabilità numerica. Si convertono poi in valori assoluti quando servono a calcolare H e lo stato (x, y, H).

La classe contiene il metodo `evolve()`, che fa avanzare la simulazione di un solo passo (`simulation.cpp` contiene la sola fisica del sistema, come detto prima). È qui che viene applicato il metodo Eulero simplettico (si calcola `y` con `x_prev` per aggiornare poi `x` con la nuova `y`) con le seguenti equazioni: 
$$
y_i^{rel} = y_{i-1}^{rel} + D\left(x_{i-1}^{rel} - 1\right) y_{i-1}^{rel} \, \Delta t
$$

$$
x_i^{rel} = x_{i-1}^{rel} + A\left(1 - y_i^{rel}\right) x_{i-1}^{rel} \, \Delta t
$$

Il metodo `state(i)` restituisce in un'unica chiamata il terzetto di valori (x, y, H) relativo allo stato i-esimo, invece di prevedere tre metodi distinti: questa scelta evita di triplicare, in tre metodi separati, lo stesso controllo di validità dell'indice.

### Visualizzazione grafica: `plot_axes` e `plotter`

L'implementazione di grafici dell'andamento delle popolazioni x(t), y(t) e dell'integrale primo H(t) e' stato reso possibile grazie all'uso della libreria grafica SFML (Simple and Fast Multimedia Library). 

- `plotter` e' la classe che gestisce la finestra grafica del programma (`sf::RenderWindow`), al cui interno si trova il metodo `show()` che si occupa di disegnare su un'unica finestra due grafici distinti: l'andamento nel tempo delle due popolazioni e l'integrale primo. In particolare, i metodi privati `drawPlot()` e `drawLegend()` vengono usati da `show()`, rispettivamnete, per fare il disegno esplicito delle singole curve e per disegnare la legenda.

- `plot_axes` contiene invece due funzioni libere, `drawAxes()` e `loadAnyFont()`, che non appartengono a nessuna classe. `drawAxes()` disegna bordo, griglia ed etichette numeriche in un riquadro rettangolare (`sf::FloatRect`), dati i range di valori da rappresentare sui due assi; `loadAnyFont()` prova a caricare un font di sistema tra alcuni percorsi noti (diversi a seconda del sistema operativo).

Le funzioni di `plot_axes` e `plotter` sono state separate perche' le funzioni di `plot_axes` ricevono come parametro tutto il necessario per il loro funzionamento e sono indipendenti da ogni stato interno della classe `Plotter`. L divisione dei compiti di `plotter` e `plot_axes` puo' essere utile per implementazioni future, per esempio se si vuole generare finestre grafiche differenti da quella di `plotter` ma con le stesse proprita' grafiche di `plot_axes` (come un possibile disegno di orbite dello spazio delle fasi delle due popolazioni).

### Validazione dell'input

La lettura dei parametri da `std::cin` è affidata alle funzioni `read_positive_double` e `read_positive_int` (file `input.hpp`/`input.cpp`), le quali verificano sia la correttezza del tipo letto sia la sua validità (valore strettamente positivo). In caso di errore viene lanciata un'eccezione `std::runtime_error`, intercettata in `main.cpp`, che stampa un messaggio descrittivo dell'errore e termina il programma senza richiedere nuovamente il valore, come da consegna.

Anche il costruttore di `Simulation` valida i propri parametri, lanciando `std::invalid_argument` se non strettamente positivi; il metodo `state(i)` lancia invece `std::out_of_range` se l'indice richiesto non è disponibile. Si è preferito l'uso di eccezioni rispetto ad `assert` perché queste ultime non possono essere intercettate nei test.alidazione dell'input##

### Costrutti (e simili) non introdotti a lezione 

- **Funzionalità di C++20**: il codice fa uso di due funzionalità introdotte con lo standard C++20.

  La prima sono gli inizializzatori designati (*designated initializers*), che permettono di costruire uno struct specificando esplicitamente a quale membro è assegnato ciascun valore (ad es. `Parameters{.A = 1.0, .B = 0.00125, .C = 0.001, .D = 1.0}`), invece di affidarsi al solo ordine posizionale dei valori. Questa scelta nasce da un errore concreto commesso durante lo sviluppo, in cui i parametri C e D sono stati scambiati per errore causando un'instabilità numerica nella simulazione: rendendo esplicito a quale membro appartiene ogni valore si è ridotto il rischio di questo tipo di errore.

  La seconda funzionalità è la possibilità di richiedere al compilatore la generazione automatica di un operatore di uguaglianza membro-a-membro tramite `bool operator==(...) const = default`, usata sugli struct `Parameters` e `State` per poterli confrontare direttamente nei test, senza scrivere a mano un confronto ripetitivo membro a membro.

- **Struct con metodi**: si sono costruiti struct con metodi (non solo dati): gli struct `Parameters` e `State` non solo raggruppano dati (rispettivamente i quattro parametri del modello e lo stato del sistema in un istante), ma definiscono anche un metodo, l'operatore di uguaglianza descritto sopra.

- **`std::ofstream`**: per scrivere i risultati della simulazione su file, il codice usa la classe `std::ofstream` (header `<fstream>`), che si comporta come `std::cout` ma indirizza l'output verso un file su disco invece che verso lo schermo, tramite lo stesso operatore `<<`.

- **`std::istringstream`**: nei test automatici (nel file `simulation_test.cpp`) si è voluto verificare il comportamento delle funzioni di lettura e validazione dell'input senza richiedere l'inserimento manuale da tastiera. A questo scopo le funzioni di lettura sono state scritte per accettare un parametro di tipo `std::istream&` generico, anziché operare direttamente su `std::cin`: questo permette di passare, nei test, un oggetto `std::istringstream` costruito a partire da una stringa che simula un input (valido o non valido), mentre nel programma vero e proprio si passa `std::cin`.

- **`Libreria SFLM`**: per la parte grafica, quindi `plot_axes.hpp`/`plot_axes.cpp` e `plotter.hpp`/`plotter.cpp`, è stata scritta con supporti esterni alle dispense del corso (tutorial/esempi online/intelligenza artificiale generativa).

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

### Input

Il programma richiede in sequenza, da tastiera, otto valori — tutti
numeri reali strettamente positivi (tranne il numero di passi, un
intero positivo):

| Parametro | Significato |
|---|---|
| `x0`, `y0` | popolazione iniziale di prede e predatori per unità di area |
| `A`, `B` | tasso di natalità e mortalità delle prede |
| `C`, `D` | tasso di natalità e mortalità dei predatori |
| `dt` | passo di integrazione |
| Numero di passi | durata della simulazione, in multipli di `dt` |

**Esempio di input** (valori inseriti in sequenza, uno per riga):
```
1200
1000
1.0
0.00125
0.001
1.0
0.001
5000
```

### Output

Il programma produce due tipi di output.

**File di testo** (`output.txt`): una riga per ogni stato calcolato
(`numero di passi + 1` righe, incluso lo stato iniziale), con quattro
colonne separate da spazio: tempo, popolazione di prede, popolazione
di predatori, valore dell'integrale primo H. Esempio delle prime righe,
con i parametri sopra:
```
0 1200 1000 -11.5478
0.001 1199.7 1000.2 -11.5478
0.002 1199.4 1000.4 -11.5478
```

**Finestra grafica** (libreria SFML): al termine della simulazione si
apre una finestra che resta aperta finché l'utente non la chiude
manualmente, divisa in due grafici: l'andamento nel tempo
delle popolazioni di prede e predatori (grafico superiore), e
dell'integrale primo H (grafico inferiore).

## Risultati e loro interpretazione

Le prove sono state condotte con i parametri riportati come esempio nella sezione precedente (x0 = 1200, y0 = 1000, A = 1.0, B = 0.00125, C = 0.001, D = 1.0), a cui corrisponde il punto di equilibrio (D/C, A/B) = (1000, 800).

**Andamento delle popolazioni** Con dt = 0.001 e 5000 passi (t finale = 5), le prede oscillano nell'intervallo [730.4, 1270.7] e i predatori nell'intervallo [584.3, 1063.1], come atteso per un'orbita chiusa attorno al punto di equilibrio nel piano delle fasi. Prolungando la simulazione a 50000 passi (t finale = 50, circa otto periodi) si osserva che l'ampiezza dell'oscillazione resta stabile nel tempo (prede nell'intervallo [730.4, 1329.0], con l'estremo superiore leggermente più alto solo perché in una simulazione più lunga si campionano più massimi dell'orbita), confermando che la traiettoria non collassa né diverge, ma percorre ripetutamente la stessa orbita chiusa, come previsto dalla teoria per questo sistema.

**Equilibrio** Avviando la simulazione esattamente nel punto di equilibrio (x0 = 1000, y0 = 800) lo stato resta invariato (x = 1000, y = 800) per tutta la durata della simulazione, come atteso: il punto di equilibrio è un punto fisso del sistema, e la trasformazione in coordinate relative usata internamente non introduce alcuna deriva spuria in questo caso limite.

## Strategia di test
I test automatici, raccolti in simulation_test.cpp, sono scritti con il framework header-only Doctest e sono organizzati in TEST_CASE distinti, ciascuno suddiviso in più SUBCASE per raggruppare scenari correlati (ad es. le diverse combinazioni di parametri non validi) senza duplicare il codice di costruzione degli oggetti coinvolti.

La strategia adottata copre tre aspetti distinti:

**Validazione dell'input** Le funzioni read_positive_double e read_positive_int sono testate sia sul caso di successo (lettura di un valore valido), sia sui casi di errore attesi: valore non numerico, valore nullo, valore negativo. In tutti i casi di errore si verifica che venga lanciata un'eccezione std::runtime_error, tramite CHECK_THROWS_AS.

**Validazione dei parametri di Simulation** Per ciascuno dei sette parametri del costruttore (A, B, C, D, x0, y0, dt) è presente una SUBCASE che rende non valido un solo parametro alla volta, lasciando gli altri fissati a valori validi, e verifica che il costruttore lanci std::invalid_argument. Questo approccio "un parametro alla volta" permette di individuare rapidamente quale controllo di validità, se rimosso o modificato per errore, farebbe fallire il test.
Comportamento della simulazione con parametri validi. Un'unica istanza di Simulation, condivisa fra le varie SUBCASE di uno stesso TEST_CASE, viene usata per verificare: che parameters() restituisca esattamente i parametri passati al costruttore (sfruttando l'operatore di uguaglianza generato automaticamente su Parameters); che lo stato iniziale corrisponda a x0/y0; che ogni chiamata a evolve() aggiunga esattamente uno stato; che l'accesso con un indice fuori intervallo lanci std::out_of_range; che l'integrale primo H si mantenga approssimativamente costante (con doctest::Approx) dopo mille passi di evoluzione.

**Validazinone del punto di equilibrio** Un test che verifica che, partendo esattamente dal punto di equilibrio, lo stato non cambi dopo l'evoluzione, così da controllare che la trasformazione in coordinate relative sia corretta anche in questo caso limite.

**Validazione dell'andamento di `evolve()`** Un test che confronta il risultato di un singolo passo di evolve() con il valore calcolato a mano applicando direttamente la formula di Eulero simplettico, così da verificare l'esattezza numerica dell'implementazione e non solo le sue proprietà qualitative. Questo test, in particolare, è quello che avrebbe individuato lo scambio accidentale dei parametri C e D menzionato in precedenza.
## Strumenti utilizzati

## Uso di intelligenza artificiale generativa