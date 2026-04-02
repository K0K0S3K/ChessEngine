from data import *

class Engine():

    def __init__(self):
        pass

    def get_fen(self) -> str:
        return "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    
    def get_pieces_arrangement(self,side):
        fen = self.get_fen()

        if(side == Side.WHITE):
            fen = fen[0:fen.find(' ')][::-1]
        else:
            fen = fen[0:fen.find(' ')]

        arrangement = []

        i = 0

        for piece in fen:
            
            if str.isdigit(piece):
                
                for n in range(int(piece)):
                    arrangement.append(Pieces.EMPTY)
                    i += 1
                continue
            elif str.isalpha(piece):
                arrangement.append(fen_to_piece.get(piece))
                i += 1

        return arrangement


