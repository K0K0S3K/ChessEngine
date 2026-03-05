#include <string>
#include <cctype>
#include "MoveGenerator.h"
#include "Board.h"
#include "EnemyPlayer.h"

using namespace std;

int main() {
    initAll(); 
    
    Board board;

    board.parseFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    string input;
    while (true) {
        board.drawBoard();


        if(board.sideToMove == WHITE_TURN)
        {
            cout << "Wpisz ruch (np. e2e4, e7e5) lub 'exit': ";
            cin >> input;

            if (input == "exit") break;

            vector<Move> moves = generateMoves(board);

            Move moveIdx = board.parseMove(input,moves);
            
            if (moveIdx.captured != -1) {
        
                board.makeMove(moveIdx);
                cout << "Wykonano ruch: " << input << endl;

                int kingSq = __builtin_ctzll(board.pieceBB[board.sideToMove == 0 ? K : k]);
                bool check = isSquareAttacked(kingSq, 1 - board.sideToMove, board); 
                if (check) cout << " [SZACH!]";
                int status = getGameResult(board,generateMoves(board));

                if(status == STALEMATE)
                {
                    cout << "KONIEC GRY!! PAT";
                    board.drawBoard();
                    return 0;
                } 
                else if(status == BLACK_WINS)
                {
                    cout << "I MAT, KONIEC GRY!! CZARNY WYGRYWA";
                    board.drawBoard();
                    return 0;
                }
                else if(status == WHITE_WINS)
                {
                    cout << "I MAT, KONIEC GRY!! BIAŁY WYGRYWA";
                    board.drawBoard();
                    return 0;
                }

            } else {
                cout << "\n[!] BLAD: Ruch nielegalny, figura na drodze lub zly format." << endl;
            }
        }
        else
        {   
            enemyMove(board,BLACK_TURN);

            int kingSq = __builtin_ctzll(board.pieceBB[board.sideToMove == 0 ? K : k]);
            bool check = isSquareAttacked(kingSq, 1 - board.sideToMove, board);
            if (check)
                cout << " [SZACH!] ";
            int status = getGameResult(board, generateMoves(board));

            if (status == STALEMATE)
            {
                cout << "KONIEC GRY!! PAT";
                board.drawBoard();
                return 0;
            }
            else if (status == BLACK_WINS)
            {
                cout << "I MAT, KONIEC GRY!! CZARNY WYGRYWA";
                board.drawBoard();
                return 0;
            }
            else if (status == WHITE_WINS)
            {
                cout << "I MAT, KONIEC GRY!! BIAŁY WYGRYWA";
                board.drawBoard();
                return 0;
            }
        }
    }

    return 0;
}