from enum import Enum
from enum import IntEnum

class Pieces(IntEnum):
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
    EMPTY = 12

white = (232, 207, 182)
black = (161, 113, 92)
background = (42, 42, 42)
grey = (140, 130, 129)
red = (255, 102, 102)

class Side(Enum):
    WHITE = 0
    BLACK = 1

fen_to_piece = {
    'P': Pieces.WHITE_PAWN, 'N': Pieces.WHITE_KNIGHT, 'B': Pieces.WHITE_BISHOP,
    'R': Pieces.WHITE_ROOK, 'Q': Pieces.WHITE_QUEEN, 'K': Pieces.WHITE_KING,
    'p': Pieces.BLACK_PAWN, 'n': Pieces.BLACK_KNIGHT, 'b': Pieces.BLACK_BISHOP,
    'r': Pieces.BLACK_ROOK, 'q': Pieces.BLACK_QUEEN, 'k': Pieces.BLACK_KING
    }

MARGIN = 25
TILE_SIZE = 100
WINDOW_SIZE = 850