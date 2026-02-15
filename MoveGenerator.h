#include <iostream>
#include <cstdint>
#include <functional>
#include "Board.h"
#include "Move.h"
#include <vector>

const uint64_t FILE_A = 0x0101010101010101ULL;
const uint64_t FILE_B = 0x0202020202020202ULL;
const uint64_t FILE_G = 0x4040404040404040ULL;
const uint64_t FILE_H = 0x8080808080808080ULL;

enum MoveFlags {
    NO_FLAG = 0,
    DOUBLE_PUSH = 1,
    EN_PASSANT = 2,
    CASTLING = 3,
    PROM_QUEEN = 4,
    PROM_ROOK = 5,
    PROM_BISHOP = 6,
    PROM_KNIGHT = 7
};

#define BOARD_SIZE 64

void initAll();

void generateMovesforKnight(uint64_t knightMoves[BOARD_SIZE]);
void generateMovesforKing(uint64_t kingMoves[BOARD_SIZE]);


uint64_t calculateRookAttacks(int sq, uint64_t occupancy);
uint64_t calculateBishopAttacks(int sq, uint64_t occupancy);
uint64_t calculateQueenAttacks(int sq, uint64_t occupancy);

void generateMovesForPawns(const Board &board,std::vector<Move> &moves,bool side);
void generateMovesForPiece(const Board &board,std::vector<Move> &moves,bool side, int piece_id, uint64_t pieceMoves[], std::function<uint64_t(int, uint64_t)> calculatePieceAttacks);
std::vector<Move> generateMoves(const Board& board);
bool isSquareAttacked(int sq, int side, const Board& board);

bool isMoveLegal(Move m, Board& board);
int getGameResult(Board &board,const std::vector<Move> &moves);