# ♟️ C++ Chess Engine

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17%2F20-blue.svg)](https://en.cppreference.com/w/cpp/language/history)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()

Wydajny silnik szachowy napisany od podstaw w C++, skupiający się na szybkości obliczeń i czystości architektury. Projekt rozwijany z myślą o implementacji nowoczesnych technik programowania szachowego.

## 🚀 Kluczowe Funkcje

* **Bitboard Engine:** Reprezentacja planszy oparta na 64-bitowych liczbach całkowitych dla maksymalnej wydajności.
* **Algorytm Search:** Implementacja algorytmu minimax (w planach rozszerzenie go o Alpha-Beta Pruning)
* **Move Generation:** Szybki generator ruchów wspierający roszady i bicia w przelocie.

## 🛠️ Architektura Projektu

Projekt został podzielony na logiczne moduły, aby zapewnić łatwą rozbudowę:

* `Board`: Zarządzanie stanem gry i bitboardami.
* `MoveGen`: Logika generowania i walidacji ruchów.
* `EnemyPlayer`: Algorytmy decyzyjne (minimax) i ocena pozycji.

## 🏗️ Kompilacja i Uruchomienie
Projekt wykorzystuje **CMake**, co pozwala na łatwą kompilację na wielu platformach.

### Wymagania
* Kompilator wspierający C++17 (np. GCC 9+, Clang 10+, MSVC 2019+)
* CMake 3.15+

### Instrukcja
```bash
# Sklonuj repozytorium
git clone https://github.com/K0K0S3K/ChessEngine.git
cd ChessEngine

# Skonfiguruj i zbuduj projekt
mkdir build && cd build
cmake ..
cmake --build .

# Uruchom silnik
./ChessEngine
