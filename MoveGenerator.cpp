#include <iostream>
#include <cstdint>
#include <functional>
#include "Board.h"
#include "Move.h"
#include <vector>
#include "MoveGenerator.h"

uint64_t knightMoves[64];
uint64_t kingMoves[64];
uint64_t zeros[64]{0};

void initKnights()
{
    generateMovesforKnight(knightMoves);
}

void initKings()
{
    generateMovesforKing(kingMoves);
}

void initAll()
{
    initKings();
    initKnights();
}

//BEGIN KNIGHT

void generateMovesforKnight(uint64_t knightMoves[BOARD_SIZE])
{
    enum Moves
    {
        UP_RIGHT    = 17,
        RIGHT_UP    = 10,
        RIGHT_DOWN  = 6,
        DOWN_RIGHT  = 15,
        UP_LEFT     = 15,
        LEFT_UP     = 6,
        LEFT_DOWN   = 10,
        DOWN_LEFT   = 17
    };

    

    for (int i = 0; i < BOARD_SIZE; ++i) {
        uint64_t knight = 1ULL << i;
        uint64_t moves = 0;
        
        moves |= (knight & ~(FILE_H)) << UP_RIGHT;
        moves |= (knight & ~(FILE_A)) << UP_LEFT;
        moves |= (knight & ~(FILE_G | FILE_H)) << RIGHT_UP;
        moves |= (knight & ~(FILE_A | FILE_B)) << LEFT_UP;

        moves |= (knight & ~(FILE_A)) >> DOWN_LEFT;
        moves |= (knight & ~(FILE_H)) >> DOWN_RIGHT;
        moves |= (knight & ~(FILE_A | FILE_B)) >> LEFT_DOWN;
        moves |= (knight & ~(FILE_G | FILE_H)) >> RIGHT_DOWN;

        knightMoves[i] = moves;
    }

}

//END KNIGHT


//BEGIN KING

void generateMovesforKing(uint64_t kingMoves[BOARD_SIZE])
{
    

    for(int i = 0; i < BOARD_SIZE; ++i)
    {
        uint64_t king = 1ULL << i;
        uint64_t moves = 0;

        moves |= (king & ~FILE_H) << 1;
        moves |= (king & ~FILE_H) << 9;
        moves |= (king & ~FILE_H) >> 7;

        moves |= (king & ~FILE_A) >> 1;
        moves |= (king & ~FILE_A) >> 9;
        moves |= (king & ~FILE_A) << 7;

        moves |= (king << 8);           
        moves |= (king >> 8);          

        kingMoves[i] = moves;
    }
}
//END KING

//BEGIN ROOK
uint64_t calculateRookAttacks(int sq, uint64_t occupancy)
{
    uint64_t attacks = 0;

    int row = sq / 8;
    int file = sq % 8;

    for(int i = row + 1; i <= 7; ++i)
    {
        attacks |= (1ULL << (i * 8 + file));

        if(occupancy & (1ULL << (i * 8 + file)))
            break;
    }

    for(int i = row - 1; i >= 0; --i)
    {
        attacks |= (1ULL << (i * 8 + file));
        
        if(occupancy & (1ULL << (i * 8 + file)))
            break;
    }

    for(int i = file + 1; i <= 7; ++i)
    {
        attacks |= (1ULL << (row * 8 + i));
        
        if(occupancy & (1ULL << (row * 8 + i)))
            break;
    }

    for(int i = file - 1; i >= 0; --i)
    {
        attacks |= (1ULL << (row * 8 + i));
        
        if(occupancy & (1ULL << (row * 8 + i)))
            break;
    }

    return attacks;
}
//END ROOK


//BEGIN BISHOP
uint64_t calculateBishopAttacks(int sq, uint64_t occupancy)
{
    uint64_t attacks = 0;

    int row = sq / 8;
    int file = sq % 8;

    for(int i = row + 1, j = file + 1; i <= 7 && j <= 7; ++i, ++j)
    {
        attacks |= (1ULL << (i * 8 + j));
        if(occupancy & (1ULL << (i * 8 + j)))
            break;
    }

    for(int i = row + 1, j = file - 1; i <= 7 && j >= 0; ++i, --j)
    {
        attacks |= (1ULL << (i * 8 + j));
        if(occupancy & (1ULL << (i * 8 + j)))
            break;
    }

    for(int i = row - 1, j = file + 1; i >= 0 && j <= 7; i--, j++)
    {
        attacks |= (1ULL << (i * 8 + j));
        if(occupancy & (1ULL << (i * 8 + j)))
            break;
    }

    for(int i = row - 1, j = file - 1; i >= 0 && j >= 0; i--, j--)
    {
        attacks |= (1ULL << (i * 8 + j));
        if(occupancy & (1ULL << (i * 8 + j)))
            break;
    }

    return attacks;
}
//END BISHOP

//BEGIN QUEEN

uint64_t calculateQueenAttacks(int sq, uint64_t occupancy)
{
    return (calculateBishopAttacks(sq,occupancy) | calculateRookAttacks(sq,occupancy));
}

//END QUEEN


void generateMovesForPiece(const Board &board,std::vector<Move> &moves,bool side, int piece_id, uint64_t pieceMoves[], std::function<uint64_t(int, uint64_t)> calculatePieceAttacks)
{
    uint64_t piece = board.pieceBB[piece_id];

    while (piece)
    {
        int source = __builtin_ctzll(piece);

        uint64_t pieceAttacks = (pieceMoves[source] | calculatePieceAttacks(source,board.occupancy[2])) & ~board.occupancy[side];
    
        while (pieceAttacks)
        {
            int target = __builtin_ctzll(pieceAttacks);

            int captured = 12;
            for (int i = 0; i < 12; i++) {
                if (board.pieceBB[i] & (1ULL << target)) {
                    captured = i;
                    break;
                }
            }
            moves.push_back(Move(source, target, piece_id, captured));
            pieceAttacks &= ~(1ULL << target);
        }
        piece &= ~(1ULL << source);
        
    
    }
    
}

void generateMovesForPawns(const Board &board,std::vector<Move> &moves,bool side)
{   
    uint64_t allPieces = board.occupancy[2];

    if(side == 0)//white pawns
    {
        uint64_t pawns = board.pieceBB[P];

        while (pawns)
        {
            int source = __builtin_ctzll(pawns);
            uint64_t sourceBit = (1ULL << source);

            //forward move
            int target = source + 8;
            if(target <= 63 && !(allPieces & (1ULL << target)))
            {
                if((1ULL << target & 0xFF00000000000000))
                {
                    moves.push_back(Move(source, target, P, 12, PROM_QUEEN));
                    moves.push_back(Move(source, target, P, 12, PROM_ROOK));
                    moves.push_back(Move(source, target, P, 12, PROM_BISHOP));
                    moves.push_back(Move(source, target, P, 12, PROM_KNIGHT));
            
                }
                else
                {
                    moves.push_back(Move(source, target, P, 12, NO_FLAG));
                }

                //double jump
                if (0x000000000000FF00 & sourceBit)
                {
                    int target2 = source + 16;
                    if (!(allPieces & (1ULL << target2)))
                        moves.push_back(Move(source, target2, P, 12, DOUBLE_PUSH));
                }

            }

            //right attack
            target = source + 9;
            if (target <= 63 && !(sourceBit & FILE_H) && (board.occupancy[1 - side] & (1ULL << target)))
            {
                int captured = 12;
                for (int i = 0; i <= 12; i++)
                {
                    if (board.pieceBB[i] & (1ULL << target))
                    {
                        captured = i;
                        break;
                    }
                }

                if ((1ULL << target) & 0xFF00000000000000)
                {
                    moves.push_back(Move(source, target, P, captured, PROM_QUEEN));
                    moves.push_back(Move(source, target, P, captured, PROM_ROOK));
                    moves.push_back(Move(source, target, P, captured, PROM_BISHOP));
                    moves.push_back(Move(source, target, P, captured, PROM_KNIGHT));
                }
                else
                {
                    moves.push_back(Move(source, target, P, captured, NO_FLAG));
                }
            }

            //left attack
            target = source + 7;
            if (target <= 63 && !(sourceBit & FILE_A) && (board.occupancy[1 - side] & (1ULL << target)))
            {
                int captured = 12;
                for (int i = 0; i <= 12; i++)
                {
                    if (board.pieceBB[i] & (1ULL << target))
                    {
                        captured = i;
                        break;
                    }
                }

                if ((1ULL << target) & 0xFF00000000000000)
                {
                    moves.push_back(Move(source, target, P, captured, PROM_QUEEN));
                    moves.push_back(Move(source, target, P, captured, PROM_ROOK));
                    moves.push_back(Move(source, target, P, captured, PROM_BISHOP));
                    moves.push_back(Move(source, target, P, captured, PROM_KNIGHT));
                }
                else
                {
                    moves.push_back(Move(source, target, P, captured, NO_FLAG));
                }
            }

            //en passant
            if (board.enPassant != -1)
            {
                if (!(sourceBit & FILE_A) && (source + 7 == board.enPassant))
                    moves.push_back(Move(source, board.enPassant, P, p, EN_PASSANT));
                if (!(sourceBit & FILE_H) && (source + 9 == board.enPassant))
                    moves.push_back(Move(source, board.enPassant, P, p, EN_PASSANT));
            }

            pawns &= ~sourceBit;
        }
        
    }
    else if(side == 1)//black pawns
    {
        uint64_t pawns = board.pieceBB[p];

        while (pawns)
        {
            int source = __builtin_ctzll(pawns);
            uint64_t sourceBit = (1ULL << source);

            int target = source - 8;
            if (target >= 0 && !(allPieces & (1ULL << target)))
            {
                if ((1ULL << target) & 0x00000000000000FF)
                {
                    moves.push_back(Move(source, target, p, 12, PROM_QUEEN));
                    moves.push_back(Move(source, target, p, 12, PROM_ROOK));
                    moves.push_back(Move(source, target, p, 12, PROM_BISHOP));
                    moves.push_back(Move(source, target, p, 12, PROM_KNIGHT));
                }
                else
                {
                    moves.push_back(Move(source, target, p, 12, NO_FLAG));

                    if (0x00FF000000000000 & sourceBit)
                    {
                        int target2 = source - 16;
                        if (!(allPieces & (1ULL << target2)))
                            moves.push_back(Move(source, target2, p, 12, DOUBLE_PUSH));
                    }
                }
            }

            //left attack
            target = source - 9;
            if (target >= 0 && !(sourceBit & FILE_A) && (board.occupancy[1 - side] & (1ULL << target)))
            {
                int captured = 12;
                for (int i = 0; i <= 12; i++)
                {
                    if (board.pieceBB[i] & (1ULL << target))
                    {
                        captured = i;
                        break;
                    }
                }

                if ((1ULL << target) & 0x00000000000000FF)
                {
                    moves.push_back(Move(source, target, p, captured, PROM_QUEEN));
                    moves.push_back(Move(source, target, p, captured, PROM_ROOK));
                    moves.push_back(Move(source, target, p, captured, PROM_BISHOP));
                    moves.push_back(Move(source, target, p, captured, PROM_KNIGHT));
                }
                else
                {
                    moves.push_back(Move(source, target, p, captured, NO_FLAG));
                }
            }

            //right attack
            target = source - 7;
            if (target >= 0 && !(sourceBit & FILE_H) && (board.occupancy[1 - side] & (1ULL << target)))
            {
                int captured = 12;
                for (int i = P; i <= K; i++)
                {
                    if (board.pieceBB[i] & (1ULL << target))
                    {
                        captured = i;
                        break;
                    }
                }

                if ((1ULL << target) & 0x00000000000000FF)
                {
                    moves.push_back(Move(source, target, p, captured, PROM_QUEEN));
                    moves.push_back(Move(source, target, p, captured, PROM_ROOK));
                    moves.push_back(Move(source, target, p, captured, PROM_BISHOP));
                    moves.push_back(Move(source, target, p, captured, PROM_KNIGHT));
                }
                else
                {
                    moves.push_back(Move(source, target, p, captured, NO_FLAG));
                }
            }

            //en passant
            if (board.enPassant != -1)
            {
                if (!(sourceBit & FILE_A) && (source - 9 == board.enPassant))
                    moves.push_back(Move(source, board.enPassant, p, P, EN_PASSANT));
                if (!(sourceBit & FILE_H) && (source - 7 == board.enPassant))
                    moves.push_back(Move(source, board.enPassant, p, P, EN_PASSANT));
            }
            pawns &= ~sourceBit;

        }
    }
}

std::vector<Move> generateMoves(const Board& board)
{
    std::vector<Move> moves;
    int side = board.sideToMove;
    int enemy = 1 - side;

    //pawns
    generateMovesForPawns(board,moves,side);

    //knight
    generateMovesForPiece(board,moves,side,side==0?N:n,knightMoves,([](int,uint64_t){return 0;}));

    //rook
    generateMovesForPiece(board,moves,side,side==0?R:r,zeros,calculateRookAttacks);

    //bishop
    generateMovesForPiece(board,moves,side,side==0?B:b,zeros,calculateBishopAttacks);

    //queen
    generateMovesForPiece(board,moves,side,side==0?Q:q,zeros,calculateQueenAttacks);

    //king
    generateMovesForPiece(board,moves,side,side==0?K:k,kingMoves,([](int,uint64_t){return 0;}));

    //roszady
    if (side == 0)
    {
        // Krótka roszada (Białe)
        if (board.castleRights & 1)
        {
            // Pola f1 i g1 muszą być puste
            if (!(board.occupancy[2] & (1ULL << 5)) && !(board.occupancy[2] & (1ULL << 6)))
            {
                // Król nie może być w szachu ani przechodzić przez szach (pola e1, f1, g1)
                if (!isSquareAttacked(4, 1, board) && !isSquareAttacked(5, 1, board))
                {
                    moves.push_back(Move(4, 6, K, 12, CASTLING));
                }
            }
        }
        // Długa roszada (Białe)
        if (board.castleRights & 2)
        {
            // Pola d1, c1, b1 muszą być puste
            if (!(board.occupancy[2] & (1ULL << 3)) && !(board.occupancy[2] & (1ULL << 2)) && !(board.occupancy[2] & (1ULL << 1)))
            {
                if (!isSquareAttacked(4, 1, board) && !isSquareAttacked(3, 1, board))
                {
                    moves.push_back(Move(4, 2, K, 12, CASTLING));
                }
            }
        }
    }
    else
    {
        // Krótka roszada (Czarne)
        if (board.castleRights & 4)
        {
            if (!(board.occupancy[2] & (1ULL << 61)) && !(board.occupancy[2] & (1ULL << 62)))
            {
                if (!isSquareAttacked(60, 0, board) && !isSquareAttacked(61, 0, board))
                {
                    moves.push_back(Move(60, 62, k, 12, CASTLING));
                }
            }
        }
        // Długa roszada (Czarne)
        if (board.castleRights & 8)
        {
            if (!(board.occupancy[2] & (1ULL << 59)) && !(board.occupancy[2] & (1ULL << 58)) && !(board.occupancy[2] & (1ULL << 57)))
            {
                if (!isSquareAttacked(60, 0, board) && !isSquareAttacked(59, 0, board))
                {
                    moves.push_back(Move(60, 58, k, 12, CASTLING));
                }
            }
        }
    }

    return moves;

}   

bool isSquareAttacked(int sq, int side, const Board& board) {
    // 1. Ataki PIONKÓW
    // Jeśli atakującym jest BIAŁY, szukamy białych pionków poniżej pola sq
    if (side == 0) {
        if (sq >= 8) { // Pionki nie mogą atakować z 1. rzędu
            if (!( (1ULL << sq) & FILE_A ) && (board.pieceBB[P] & (1ULL << (sq - 9)))) return true;
            if (!( (1ULL << sq) & FILE_H ) && (board.pieceBB[P] & (1ULL << (sq - 7)))) return true;
        }
    } 
    // Jeśli atakującym jest CZARNY, szukamy czarnych pionków powyżej pola sq
    else {
        if (sq <= 55) { // Pionki nie mogą atakować z 8. rzędu
            if (!( (1ULL << sq) & FILE_H ) && (board.pieceBB[p] & (1ULL << (sq + 9)))) return true;
            if (!( (1ULL << sq) & FILE_A ) && (board.pieceBB[p] & (1ULL << (sq + 7)))) return true;
        }
    }

    // 2. Ataki SKOCZKÓW
    // Wykorzystujemy symetrię: jeśli skoczek z 'sq' atakuje skoczka przeciwnika, 
    // to ten skoczek przeciwnika atakuje 'sq'.
    uint64_t knights = (side == 0) ? board.pieceBB[N] : board.pieceBB[n];
    if (knightMoves[sq] & knights) return true;

    // 3. Ataki KRÓLA
    uint64_t king = (side == 0) ? board.pieceBB[K] : board.pieceBB[k];
    if (kingMoves[sq] & king) return true;

    // 4. Ataki GOŃCÓW i HETMANÓW (Przekątne)
    uint64_t bishops = (side == 0) ? board.pieceBB[B] : board.pieceBB[b];
    uint64_t queens = (side == 0) ? board.pieceBB[Q] : board.pieceBB[q];
    if (calculateBishopAttacks(sq, board.occupancy[2]) & (bishops | queens)) return true;

    // 5. Ataki WIEŻ i HETMANÓW (Linie proste)
    uint64_t rooks = (side == 0) ? board.pieceBB[R] : board.pieceBB[r];
    if (calculateRookAttacks(sq, board.occupancy[2]) & (rooks | queens)) return true;

    return false;
}

bool isMoveLegal(Move m, Board& board) {
    int us = board.sideToMove;
    board.makeMove(m);
    
    // Po makeMove strona się zmieniła, więc szukamy króla strony, która WŁAŚNIE się ruszyła
    int kingSq = __builtin_ctzll(board.pieceBB[us == 0 ? K : k]);
    
    // Sprawdzamy, czy przeciwnik (teraz board.sideToMove) atakuje naszego króla
    bool underCheck = isSquareAttacked(kingSq, board.sideToMove, board);
    
    board.unmakeMove(m);
    return !underCheck;
}

int getGameResult(Board &board,const std::vector<Move> &moves)
{
    
    int i = 0;

    for(const auto &move : moves)
    {   
        if(isMoveLegal(move,board))
        {
            return IN_PROGRESS;
        }
    }

    int king = 0;

    if(board.sideToMove == WHITE_TURN)
    {
        if(board.pieceBB[K] != 0 && isSquareAttacked(__builtin_ctzll(board.pieceBB[K]), BLACK_TURN, board))
        {
            return BLACK_WINS;
        }
        else
        {
            return STALEMATE;
        }

    } 
    else
    {
        if(board.pieceBB[k] != 0 && isSquareAttacked(__builtin_ctzll(board.pieceBB[k]), WHITE_TURN, board))
        {
            return WHITE_WINS;
        }
        else
        {
            return STALEMATE;
        }
    }

    

    
}

//0 - 16 ok
//17 - 
