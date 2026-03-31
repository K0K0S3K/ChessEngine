import pygame
from enum import Enum
import os

class Pieces(Enum):
    WHITE_PAWN = 0
    WHITE_KNIGHT = 1
    WHITE_BISHOP = 2
    WHITE_ROOK = 3
    WHITE_QUEEN =  4
    WHITE_KING = 5
    BLACK_PAWN = 6
    BLACK_KNIGHT = 7
    BLACK_BISHOP = 8
    BLACK_ROOK = 9
    BLACK_QUEEN = 10
    BLACK_KING = 11

class Side(Enum):
    WHITE = 0
    BLACK = 1

def load_piece(name, size):
    path = os.path.join("./gui/assets", f"{name}.png")
    image = pygame.image.load(path).convert_alpha()
    return pygame.transform.smoothscale(image, (size, size))


class Board:
    def __init__(self,side,tile_size,margin):
        self.side = side
        self.tile_size = tile_size
        self.margin = margin

        self.pieces = {
            Pieces.WHITE_PAWN : load_piece('pawn-w',tile_size),
            Pieces.WHITE_KNIGHT : load_piece('knight-w',tile_size),
            Pieces.WHITE_BISHOP : load_piece('bishop-w',tile_size),
            Pieces.WHITE_ROOK : load_piece('rook-w',tile_size),
            Pieces.WHITE_QUEEN : load_piece('queen-w',tile_size),
            Pieces.WHITE_KING : load_piece('king-w',tile_size),

            Pieces.BLACK_PAWN : load_piece('pawn-b',tile_size),
            Pieces.BLACK_KNIGHT : load_piece('knight-b',tile_size),
            Pieces.BLACK_BISHOP : load_piece('bishop-b',tile_size),   
            Pieces.BLACK_ROOK : load_piece('rook-b',tile_size),
            Pieces.BLACK_QUEEN : load_piece('queen-b',tile_size),
            Pieces.BLACK_KING : load_piece('king-b',tile_size)
        }

        self.fen_to_piece = {
            'P': Pieces.WHITE_PAWN, 'N': Pieces.WHITE_KNIGHT, 'B': Pieces.WHITE_BISHOP,
            'R': Pieces.WHITE_ROOK, 'Q': Pieces.WHITE_QUEEN, 'K': Pieces.WHITE_KING,
            'p': Pieces.BLACK_PAWN, 'n': Pieces.BLACK_KNIGHT, 'b': Pieces.BLACK_BISHOP,
            'r': Pieces.BLACK_ROOK, 'q': Pieces.BLACK_QUEEN, 'k': Pieces.BLACK_KING
        }

    def display_menu(self):
        pass

    def print_board(self,display):

        white = (232, 207, 182)
        black = (161, 113, 92)
        background = (42, 42, 42)
        tile_size = self.tile_size
        col = True
        margin = self.margin
        
        display.fill(background)

        for i in range(64):

            left = margin + (i % 8) * tile_size
            top = margin + 700 - (i//8)*tile_size

            if col:
                pygame.draw.rect(surface=display,color=black,rect=pygame.Rect(left,top,tile_size,tile_size))
            else:
                pygame.draw.rect(surface=display,color=white,rect=pygame.Rect(left,top,tile_size,tile_size))

            if i % 8 != 7:
                col = not col
            

    def print_pieces(self,FEN_string,display):

        board_layout = FEN_string.split(' ')[0]
        rows = board_layout.split('/')
        
        for row_index, row in enumerate(rows):
            col_index = 0
            for char in row:
                if char.isdigit():
                    col_index += int(char)
                else:
                    x = col_index * self.tile_size + self.margin
                    y = row_index * self.tile_size + self.margin
                    
                    image = self.pieces.get(self.fen_to_piece.get(char))
            
                    if image:
                        display.blit(image, (x, y))
                    
                    col_index += 1

    def display_game(self,display,FEN_string):
        self.print_board(display)
        self.print_pieces(FEN_string,display)

    def display_endgame(self):
        pass
