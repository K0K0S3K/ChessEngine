#include <string>
#include <cctype>
#include "MoveGenerator.h"
#include "Board.h"

using namespace std;


int main() {
    // Inicjalizacja tablic (skoczki i króle)
    initAll(); 
    
    Board board;
    // Ustawienie pozycji startowej
    board.parseFEN("rnbqkbnr/ppppp2p/5p2/6p1/3PP3/8/PPP2PPP/RNBQKBNR w KQkq - 0 1");

    string input;
    while (true) {
        board.drawBoard();
        cout << "Wpisz ruch (np. e2e4, e7e5) lub 'exit': ";
        cin >> input;

        if (input == "exit") break;

        vector<Move> moves = generateMoves(board);
        // Wykorzystujemy Twoją funkcję parseMove
        Move moveIdx = board.parseMove(input,moves);
        
        if (moveIdx.captured != -1) {
            // Pobieramy ruchy ponownie, aby wyciągnąć ten konkretny     
            board.makeMove(moveIdx); // Wykonanie ruchu
            cout << "Wykonano ruch: " << input << endl;
            // Informacja o turze i stanie króla
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

    return 0;
}