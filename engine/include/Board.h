#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "Move.h"

// Indeksy dla naszych 12 bitboardów
enum { P, N, B, R, Q, K, p, n, b, r, q, k };

enum GameResult { IN_PROGRESS, WHITE_WINS, BLACK_WINS, STALEMATE };

enum Turn {WHITE_TURN = 0, BLACK_TURN = 1};

struct UndoState {
    int enPassant;
    int castleRights;
};

class Board {
public:
    std::vector<UndoState> history;

    uint64_t pieceBB[12];    // Bitboardy dla każdej figury
    uint64_t occupancy[3];   // [0] - BIAŁE, [1] - CZARNE, [2] - WSZYSTKIE

    Turn sideToMove;          // 0 (Białe), 1 (Czarne)
    int enPassant;           // Indeks pola (0-63) lub -1
    int castleRights;        // Flagi: 1(WK), 2(WQ), 4(BK), 8(BQ)
    int gameState;           // IN_PROGRESS, WHITE_WINS, BLACK_WINS, STALEMATE

    Board();
    void clear();
    void updateOccupancy();
    void parseFEN(std::string fen);
    void makeMove(Move m);
    void unmakeMove(Move m);
    void drawBoard();
    Move parseMove(std::string moveStr, std::vector<Move> &moves);
    std::string generateFEN() const;
};