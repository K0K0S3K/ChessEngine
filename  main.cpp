#include <iostream>

#define BOARD_SIZE 64

using namespace std;

void generateMovesforKnight(uint64_t knightMoves[BOARD_SIZE])
{
    enum Moves
    {
        UP_RIGHT    = 17,
        RIGHT_UP    = 10,
        RIGHT_DOWN  = -6,
        DOWN_RIGHT  = -15,
        UP_LEFT     = 15,
        LEFT_UP     = 6,
        LEFT_DOWN   = -10,
        DOWN_LEFT   = -17
    };

    const uint64_t FILE_A = 0x0101010101010101ULL;
    const uint64_t FILE_B = 0x0202020202020202ULL;
    const uint64_t FILE_G = 0x4040404040404040ULL;
    const uint64_t FILE_H = 0x8080808080808080ULL;

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

        //moves |= knight;

        knightMoves[i] = moves;
    }

}

void generateMovesforKing(uint64_t kingMoves[BOARD_SIZE])
{
    const uint64_t FILE_A = 0x0101010101010101ULL;
    const uint64_t FILE_H = 0x8080808080808080ULL;




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
        
        //moves |= king;

        kingMoves[i] = moves;
    }
}

void printBoard(uint64_t board)
{
    cout << "| |a|b|c|d|e|f|g|h| |" << endl;
    for(int i = 7; i >= 0; --i)
    {
        cout << "|"<< i + 1 <<"|";
        for(int j = 0; j <= 7; ++j)
        {
            uint64_t mask = 1ULL << ((i * 8) + j);
            bool current = (board & mask);

            if(board & mask)
                cout << "+|";
            else
                cout << ".|";
        }
        cout << i + 1 << "|" << endl;
    }
    cout << "| |a|b|c|d|e|f|g|h| |" << endl;
}

//0000000000000000000000000000000000000000000000000000000011111111

typedef uint64_t Bitboard;

Bitboard getRookAttacks(int sq, Bitboard occupancy)
{
    Bitboard res = (1ULL << sq);

    int sq_cpy = sq;

    while (true)
    {   
        if(occupancy & (1ULL << sq))
            break;

        res |= (1ULL << sq);

        if(sq % 8 == 7)
            break;

        sq += 1;
    }

    sq = sq_cpy;

    while (true)
    {   
        if(occupancy & (1ULL << sq))
            break;

        res |= (1ULL << sq);

        if(sq % 8 == 0)
            break;

        sq -= 1;
    }

    sq = sq_cpy;

    while (true)
    {   
        if(occupancy & (1ULL << sq))
            break;

        res |= (1ULL << sq);

        if(56 <= sq && sq <= 63)
            break;

        sq += 8;
    }

    sq = sq_cpy;

    while (true)
    {   
        if(occupancy & (1ULL << sq))
            break;

        res |= (1ULL << sq);

        if(0 <= sq && sq <= 7)
            break;

        sq -= 8;
    }
    
    return res;
}


int main()
{
    
    uint64_t board[64];
    generateMovesforKnight(board);

    for(int i = 1; i < 20; i++)
    {
        printBoard(board[i * 2]);
    }

   /* uint64_t occ = 0;

    occ |= (1ULL << 30);
    occ |= (1ULL << 51);

    Bitboard att = getRookAttacks(27, occ);

    printBoard(att);*/

    
}

