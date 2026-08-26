# Progetto Lotka-Volterra 2026

Simulazione numerica del sistema preda-predatore descritto dalle
equazioni di Lotka-Volterra integrate con il metodo di Eulero
simplettico. Include visualizzazione grafica dell'andamento delle
popolazioni nel tempo tramite SFML.

## Dipendenze

- Compilatore C++ con supporto a C++20
- [CMake](https://cmake.org/) ≥ 3.17
- [Ninja](https://ninja-build.org/)
- [SFML](https://www.sfml-dev.org/) ≥ 2.6

Su sistemi Debian/Ubuntu:
```bash
sudo apt install libsfml-dev ninja-build
```

Per eseguire i test è inoltre necessario l'header singolo di
[Doctest](https://github.com/doctest/doctest) (`doctest.h`), da
posizionare nella cartella del progetto.

## Compilazione ed esecuzione

```bash
cmake -S . -B build -G "Ninja Multi-Config"
cmake --build build --config Debug
./build/lotka_volterra
```

## Test

```bash
cmake --build build --config Debug --target test
```

Per una build ottimizzata (facoltativo):
```bash
cmake --build build --config Release
cmake --build build --config Release --target test
```

## Struttura del progetto

- `simulation.hpp`/`simulation.cpp` — logica del modello fisico
- `input.hpp`/`input.cpp` — lettura e validazione dell'input
- `Plotter.hpp`/`Plotter.cpp`, `PlotAxes.hpp`/`PlotAxes.cpp` — visualizzazione grafica
- `main.cpp` — programma principale
- `simulation_test.cpp` — test automatici (Doctest)

Per una descrizione dettagliata delle scelte progettuali, si veda `Relazione.md`.

## Autori

Shala Artan, Tonin Anita