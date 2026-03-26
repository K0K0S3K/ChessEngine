#include "../include/Board.h"
#include "../include/Move.h"
#include "../include/MoveGenerator.h"
#include <map>

const int castlingUpdate[64] = {
    13, 15, 15, 15, 12, 15, 15, 14,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
     7, 15, 15, 15,  3, 15, 15, 11
};

Board::Board() 
{ 
    clear();

}

void Board::clear() {
    for(int i = 0; i < 12; i++) pieceBB[i] = 0ULL;
    for(int i = 0; i < 3; i++) occupancy[i] = 0ULL;
    sideToMove = 0;
    enPassant = -1;
    castleRights = 0;
    gameState = IN_PROGRESS;
}

void Board::updateOccupancy() {
    occupancy[0] = 0; occupancy[1] = 0; occupancy[2] = 0;
    for (int i = P; i <= K; i++) occupancy[0] |= pieceBB[i]; // Białe
    for (int i = p; i <= k; i++) occupancy[1] |= pieceBB[i]; // Czarne
    occupancy[2] = occupancy[0] | occupancy[1];            // Wszystkie
}

void Board::parseFEN(std::string fen) {
    clear();
    int rank = 7; 
    int file = 0;
    int i = 0;

    // 1. Rozstawianie figur
    while (fen[i] != ' ') {
        if (fen[i] == '/') {
            rank--; file = 0;
        } else if (isdigit(fen[i])) {
            file += (fen[i] - '0');
        } else {
           
            std::map<char, int> charToPiece = {
                {'P', P}, {'N', N}, {'B', B}, {'R', R}, {'Q', Q}, {'K', K},
                {'p', p}, {'n', n}, {'b', b}, {'r', r}, {'q', q}, {'k', k}
            };
            int piece = charToPiece[fen[i]];
            pieceBB[piece] |= (1ULL << (rank * 8 + file));
            file++;
        }
        i++;
    }
    i++;
    sideToMove = (fen[i] == 'w') ? 0 : 1;

    // 3. Roszady 
    i += 2;
    while (fen[i] != ' ') {
        if (fen[i] == 'K') castleRights |= 1;
        if (fen[i] == 'Q') castleRights |= 2;
        if (fen[i] == 'k') castleRights |= 4;
        if (fen[i] == 'q') castleRights |= 8;
        if (fen[i] == '-') break;
        i++;
    }

    while (fen[i] != ' ')
        i++; 
    i++;     

    if (fen[i] != '-')
    {
        int f = fen[i] - 'a';
        int r = fen[i + 1] - '1';
        enPassant = r * 8 + f;
    }
    else
    {
        enPassant = -1;
    }

    updateOccupancy();
}

void Board::makeMove(Move move)
{
    uint64_t source = (1ULL << move.source);
    uint64_t target = (1ULL << move.target);

    history.push_back({enPassant,castleRights});

    pieceBB[move.piece] &= ~source;

    if(move.flags >= PROM_QUEEN)
    {
        int promPiece;
        if(sideToMove == 0)
        {
            if(move.flags == PROM_QUEEN)
            {
                promPiece = Q;
            }
            else if(move.flags == PROM_ROOK)
            {
                promPiece = R;
            }
            else if(move.flags == PROM_BISHOP)
            {
                promPiece = B;
            }
            else if(move.flags == PROM_KNIGHT)
            {
                promPiece = N;
            }
        }
        else if(sideToMove == 1)
        {
            if(move.flags == PROM_QUEEN)
            {
                promPiece = q;
            }
            else if(move.flags == PROM_ROOK)
            {
                promPiece = r;
            }
            else if(move.flags == PROM_BISHOP)
            {
                promPiece = b;
            }
            else if(move.flags == PROM_KNIGHT)
            {
                promPiece = n;
            }
        }
        pieceBB[promPiece] |= target;
    }
    else
    {
        pieceBB[move.piece] |= target;
    }

    if(move.captured != 12)
    {

        if(move.flags == EN_PASSANT)
        {
            int epSquare = (sideToMove == 0) ? (move.target - 8) : (move.target + 8);
            pieceBB[move.captured] &= ~(1ULL << epSquare);
        }
        else
        {
            pieceBB[move.captured] &= ~target;
        }
    }

    if (move.flags == CASTLING) {
        switch (move.target) {
            case 6: 
                pieceBB[R] &= ~(1ULL << 7); pieceBB[R] |= (1ULL << 5); break;
            case 2: 
                pieceBB[R] &= ~(1ULL << 0); pieceBB[R] |= (1ULL << 3); break;
            case 62: 
                pieceBB[r] &= ~(1ULL << 63); pieceBB[r] |= (1ULL << 61); break;
            case 58: 
                pieceBB[r] &= ~(1ULL << 56); pieceBB[r] |= (1ULL << 59); break;
        }
    }

    enPassant = -1; 
    if (move.flags == DOUBLE_PUSH) {
        enPassant = (sideToMove == 0) ? (move.source + 8) : (move.source - 8);
    }

    castleRights &= castlingUpdate[move.source];
    castleRights &= castlingUpdate[move.target];

    sideToMove = 1 - sideToMove;
    updateOccupancy();
}

void Board::unmakeMove(Move m) {
    sideToMove = 1 - sideToMove;
    uint64_t fromBit = (1ULL << m.source);
    uint64_t toBit = (1ULL << m.target);
    
    for (int i = 0; i < 12; i++) 
        pieceBB[i] &= ~toBit;

    pieceBB[m.piece] |= fromBit; 

    if (m.captured != 12) {
        if (m.flags == EN_PASSANT) {
            int epSquare = (sideToMove == 0) ? (m.target - 8) : (m.target + 8);
            pieceBB[m.captured] |= (1ULL << epSquare);
        } else {
            pieceBB[m.captured] |= toBit;
        }
    }

    if (m.flags == CASTLING) {
        if (m.target == 6) { pieceBB[R] &= ~(1ULL << 5); pieceBB[R] |= (1ULL << 7); }
        else if (m.target == 2) { pieceBB[R] &= ~(1ULL << 3); pieceBB[R] |= (1ULL << 0); }  // WQ
        else if (m.target == 62) { pieceBB[r] &= ~(1ULL << 61); pieceBB[r] |= (1ULL << 63); } // BK
        else if (m.target == 58) { pieceBB[r] &= ~(1ULL << 59); pieceBB[r] |= (1ULL << 56); } // BQ
    }

    UndoState lastState = history.back();
    enPassant = lastState.enPassant;
    castleRights = lastState.castleRights;
    history.pop_back();

    updateOccupancy();
}

void Board::drawBoard() {
    const char* pieceChars = "PNBRQKpnbrqk";
    std::cout << "\n    a   b   c   d   e   f   g   h" << std::endl;
    std::cout << "  +---+---+---+---+---+---+---+---+" << std::endl;
    for (int r = 7; r >= 0; r--) {
        std::cout << r + 1 << " | ";
        for (int f = 0; f <= 7; f++) {
            int sq = r * 8 + f;
            int piece = -1;
            for (int p = P; p <= k; p++) {
                if (pieceBB[p] & (1ULL << sq)) { piece = p; break; }
            }
            if (piece == -1) std::cout << " ";
            else std::cout << pieceChars[piece];
            std::cout << " | ";
        }
        std::cout << r + 1 << std::endl;
        std::cout << "  +---+---+---+---+---+---+---+---+" << std::endl;
    }
    std::cout << "    a   b   c   d   e   f   g   h" << std::endl;
    std::cout << "Ruch: " << (sideToMove == 0 ? "BIALE" : "CZARNE");
    std::cout << std::endl;
}

Move Board::parseMove(std::string moveStr, std::vector<Move> &moves) {
    if (moveStr.length() < 4) return Move(-1,-1,-1,-1,-1);

    int f_col = tolower(moveStr[0]) - 'a';
    int f_row = moveStr[1] - '1';
    int t_col = tolower(moveStr[2]) - 'a';
    int t_row = moveStr[3] - '1';
    
    int fromSquare = f_row * 8 + f_col;
    int toSquare = t_row * 8 + t_col;

    for (int i = 0; i < moves.size(); i++) {
        Board tempBoard = *this;
        if (!isMoveLegal(moves[i], tempBoard)) continue;

        if (moves[i].source == fromSquare && moves[i].target == toSquare) {
            if (moves[i].flags >= PROM_QUEEN) {
                if (moveStr.length() < 5) continue;
                char prom = tolower(moveStr[4]);
                if (prom == 'q' && moves[i].flags != PROM_QUEEN) continue;
                if (prom == 'r' && moves[i].flags != PROM_ROOK) continue;
                if (prom == 'b' && moves[i].flags != PROM_BISHOP) continue;
                if (prom == 'n' && moves[i].flags != PROM_KNIGHT) continue;
            }
            return moves[i];
        }
    }
    return Move(-1,-1,-1,-1,-1);
}
