from data import *
import subprocess
import os


class Engine():
    def __init__(self, path_to_exe):
        self.process = subprocess.Popen(
            path_to_exe,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            bufsize=1 # Line buffering
        )
        self.send_command("uci")
        line = self.process.stdout.readline().strip()
        print(line)

        self.fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

    def send_command(self, cmd):
        self.process.stdin.write(f"{cmd}\n")
        self.process.stdin.flush()

    def set_fen(self,arrangement):
        self.fen = "x"
    
    def get_pieces_arrangement(self):
        fen = self.fen

        fen = fen[0:fen.find(' ')][::-1]

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


