#include <string>
#include <cctype>
#include <sstream>
#include <unordered_map>
#include "../include/MoveGenerator.h"
#include "../include/Board.h"
#include "../include/EnemyPlayer.h"

using namespace std;

enum class Command {
    UCI, ISREADY, POSITION, GO, GETMOVES, QUIT, UNKNOWN,GETFEN
};

Command hashCommand(const std::string& cmd) {
    static const std::unordered_map<std::string, Command> cmdMap = {
        {"uci",      Command::UCI},
        {"isready",  Command::ISREADY},
        {"position", Command::POSITION},
        {"go",       Command::GO},
        {"getmoves", Command::GETMOVES},
        {"quit",     Command::QUIT},
        {"getfen",   Command::GETFEN}
    };

    auto it = cmdMap.find(cmd);
    if (it != cmdMap.end()) {
        return it->second;
    }
    return Command::UNKNOWN;
}

int main() {
    initAll(); 
    
    Board board;

    board.parseFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    std::string line, cmd;

    vector<Move> moves = generateMoves(board);
    
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        
        std::istringstream is(line);
        is >> cmd;

        switch (hashCommand(cmd)) {
            case Command::UCI:
                std::cout << "uciok" << std::endl;
                break;

            case Command::ISREADY:
                std::cout << "readyok" << std::endl;
                break;

            case Command::POSITION: {
                std::string arg;
                is >> arg;
                if (arg == "startpos") {
                    board.parseFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
                } else {
                    size_t dashPos = arg.find('-');
                    if (dashPos != std::string::npos) {
                        int from = std::stoi(arg.substr(0, dashPos));
                        int to = std::stoi(arg.substr(dashPos + 1));
                        
                        for(const auto mv : moves)
                        {
                            if(mv.source == from && mv.target == to)
                            {
                                board.makeMove(mv);
                                break;
                            }
                        }
                        
                    }
                }
                break;
            }

            case Command::GO: {
                enemyMove(board,board.sideToMove);
                moves = generateMoves(board);
                break;
            }

            case Command::GETMOVES: {
                int source;
                if (is >> source) {
                    cout << "moves: ";

                    for(const auto mv : moves)
                    {
                        if(mv.source == source)
                        {
                            if(isMoveLegal(mv,board))
                            {
                                cout << mv.source << "-" << mv.target;
                            }
                        }
                    }

                    cout << '\n';

                }
                break;
            }

            case Command::GETFEN:
                cout << board.generateFEN() << '\n';
                break;

            case Command::QUIT:
                return 0;

            default:
                // Ignoruj nieznane komendy
                break;
        }
    }
    return 0;
}
