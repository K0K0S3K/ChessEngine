#include "../include/EnemyPlayer.h"
#include "../include/MoveGenerator.h"
#include "../include/Board.h"
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

    return material;
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

    score += PSTeval(board.pieceBB[p],pawnPST,BLACK_TURN);
    score += PSTeval(board.pieceBB[r],rookPST,BLACK_TURN);
    score += PSTeval(board.pieceBB[q],queenPST,BLACK_TURN);
    score += PSTeval(board.pieceBB[n],knightPST,BLACK_TURN);
    score += PSTeval(board.pieceBB[k],kingPST,BLACK_TURN);
    score += PSTeval(board.pieceBB[b],bishopPST,BLACK_TURN);
    
    return score;
}

int minimax(Board &board, int depth, bool isMaximizing)
{
    if(depth == 0)
    {
        return evaluate(board);
    }

    vector<Move> moves = generateMoves(board);

    int result = getGameResult(board, moves);

    if (result != IN_PROGRESS) {
        if (result == WHITE_WINS) return 999999 + depth;
        if (result == BLACK_WINS) return -999999 - depth;
        return 0;
    }

    if(isMaximizing)
    {
        int maxEval = -1000000;
        for(const auto &move : moves)
        {
            if(isMoveLegal(move,board))
            {
                board.makeMove(move);
                int eval = minimax(board,depth - 1, false);
                board.unmakeMove(move);
                maxEval = max(maxEval,eval);
            }
        }
        return maxEval;
    } 
    else
    {
        int minEval = 1000000;
        for(const auto &move : moves)
        {
            if(isMoveLegal(move, board))
            {
                board.makeMove(move);
                int eval = minimax(board,depth, true);
                board.unmakeMove(move);
                minEval = min(minEval,eval);
            }
        }

        return minEval;
    }

    return 0;

}

void enemyMove(Board &board, Turn turn)
{
    vector<Move> moves = generateMoves(board);

    Move bestMove = Move(0,0,0,0);
    int bestValue = (turn == WHITE_TURN) ? -999999 : 999999;

    for(const auto &move : moves)
    {
        if(isMoveLegal(move,board))
        {
            board.makeMove(move);
            int val = minimax(board, depth - 1, (turn == BLACK_TURN));
            board.unmakeMove(move);

            if(turn == WHITE_TURN) {
                if(val > bestValue) {
                    bestValue = val;
                    bestMove = move;
                }
            } else {
                if(val < bestValue) {
                    bestValue = val;
                    bestMove = move;
                }
            }
        }
    }

    board.makeMove(bestMove);

    cout << "bestmove " << bestMove.source << "-" << bestMove.target << endl;
}
