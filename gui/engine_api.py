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
        self.fen = ""
        out = self.send_command("uci")
        print(out)
        print(self.send_command("position startpos"))
        self.getfen()      

    def send_command(self, cmd):
        self.process.stdin.write(f"{cmd}\n")
        self.process.stdin.flush()

        out = self.process.stdout.readline().strip()

        return out


    def getfen(self):
        self.fen = self.send_command("getfen")
    

    def get_pieces_arrangement(self):
        fen = self.fen

        fen = fen[::-1]

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

        for i in range(0,64,8):
            arrangement[0+i:8+i] = arrangement[0+i:8+i][::-1]

        return arrangement


