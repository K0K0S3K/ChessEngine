#include "EnemyPlayer.h"
#include "MoveGenerator.h"
#include "Board.h"
#include <vector>
#include <iostream>

using namespace std;

//jeżeli materiał jest większy od 0 to biały ma przewagę, jeżeli mniejszy od 0, to czarny ma przewagę.
int shannonFunction(const Board& board)
{
    int material = 0;

    material += PAWN_VALUE * (__builtin_popcountll(board.pieceBB[P]) - __builtin_popcountll(board.pieceBB[p]));
    material += QUEEN_VALUE * ( __builtin_popcountll(board.pieceBB[Q]) - __builtin_popcountll(board.pieceBB[q]) );
    material += KNIGHT_VALUE * ( __builtin_popcountll(board.pieceBB[N]) - __builtin_popcountll(board.pieceBB[n]) );
    material += BISHOP_VALUE * ( __builtin_popcountll(board.pieceBB[B]) - __builtin_popcountll(board.pieceBB[b]) );
    material += ROOK_VALUE * ( __builtin_popcountll(board.pieceBB[R]) - __builtin_popcountll(board.pieceBB[r]) );
    material += KING_VALUE * ( __builtin_popcountll(board.pieceBB[K]) - __builtin_popcountll(board.pieceBB[k]) );

}

int PSTeval(uint64_t piece, const int pstTable[], Turn side)
{
    int score = 0;

    while (piece)
    {
        int sq = __builtin_ctzll(piece);

        if(side == WHITE_TURN)
        {
            score += pstTable[sq];
        }
        else
        {
            score -= pstTable[sq ^ 56];
        }

        piece &= (piece - 1);
    }

    return score;
}

int evaluate(const Board &board)
{
    int score = shannonFunction(board);

    score += PSTeval(board.pieceBB[P],pawnPST,WHITE_TURN);
    score += PSTeval(board.pieceBB[R],rookPST,WHITE_TURN);
    score += PSTeval(board.pieceBB[Q],queenPST,WHITE_TURN);
    score += PSTeval(board.pieceBB[N],knightPST,WHITE_TURN);
    score += PSTeval(board.pieceBB[K],kingPST,WHITE_TURN);
    score += PSTeval(board.pieceBB[B],bishopPST,WHITE_TURN);

    score -= PSTeval(board.pieceBB[p],pawnPST,BLACK_TURN);
    score -= PSTeval(board.pieceBB[r],rookPST,BLACK_TURN);
    score -= PSTeval(board.pieceBB[q],queenPST,BLACK_TURN);
    score -= PSTeval(board.pieceBB[n],knightPST,BLACK_TURN);
    score -= PSTeval(board.pieceBB[k],kingPST,BLACK_TURN);
    score -= PSTeval(board.pieceBB[b],bishopPST,BLACK_TURN);
    
    return score;
}