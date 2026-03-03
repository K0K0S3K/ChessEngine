#include "Board.h"

//jeżeli materiał jest większy od 0 to biały ma przewagę, jeżeli mniejszy od 0, to czarny ma przewagę.
double shannonFunction(const Board& board, const std::vector<Move>& moves)
{
    double material = 0;

    material += 200 * (__builtin_popcountll(board.pieceBB[P]) - __builtin_popcountll(board.pieceBB[p]));
    material += 9 * ( __builtin_popcountll(board.pieceBB[Q]) - __builtin_popcountll(board.pieceBB[q]) );


}