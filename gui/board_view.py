import pygame
import os
from data import *

def load_piece(name, size):
    path = os.path.join("./gui/assets", f"{name}.png")
    image = pygame.image.load(path).convert_alpha()
    return pygame.transform.smoothscale(image, (size, size))

class StartGameButton:
    def __init__(self, center_pos, width, height, text, color, font):
        self.rect = pygame.Rect(0, 0, width, height)
        self.rect.center = center_pos
        self.color = color
        
        self.text_surf = font.render(text, True, (255, 255, 255))
        self.text_rect = self.text_surf.get_rect(center=self.rect.center)

    def draw(self, display):
        pygame.draw.rect(display, self.color, self.rect, border_radius=8)
        display.blit(self.text_surf, self.text_rect)

    def is_clicked(self, mouse_pos):
        return self.rect.collidepoint(mouse_pos)
        

class Text:
    def __init__(self, text, font, color, center_pos):
        self.surface = font.render(text, True, color)
        self.rect = self.surface.get_rect(center=center_pos)
    
    def print_text(self, display):
        display.blit(self.surface, self.rect)
    

class Board:
    def __init__(self,tile_size,margin):
        self.tile_size = tile_size
        self.margin = margin

        self.window_size = WINDOW_SIZE
        mid_x = WINDOW_SIZE // 2
        
        chess_text_font = pygame.font.SysFont('Arial', 80, bold=True)
        author_text_font = pygame.font.SysFont('Arial', 20)
        button_font = pygame.font.SysFont('Arial', 32)

        self.chess_text = Text("Szachy", chess_text_font, (255, 255, 255), (mid_x, 150))

        
        buttons_y = 300 
        button_w, button_h = 220, 60
        spacing = 130 

        self.btn_white = StartGameButton(
            center_pos=(mid_x - spacing, buttons_y),
            width=button_w, height=button_h,
            text="Graj Białymi", color=(80, 80, 80), font=button_font
        )

        self.btn_black = StartGameButton(
            center_pos=(mid_x + spacing, buttons_y),
            width=button_w, height=button_h,
            text="Graj Czarnymi", color=(30, 30, 30), font=button_font
        )

        author_h = author_text_font.get_height()
        self.author_text = Text(
            "author: Adam Kwiatkowski", 
            author_text_font, 
            (200, 200, 200), 
            (10 + 115, WINDOW_SIZE - author_h) 
        )




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

    def display_menu(self,display : pygame.surface.Surface):

        display.fill(background)

        top = WINDOW_SIZE - 350
        left = 65
        p_size = 120

    
        pieces = [Pieces.BLACK_KING,Pieces.WHITE_QUEEN,Pieces.BLACK_BISHOP,Pieces.WHITE_KNIGHT,Pieces.BLACK_ROOK,Pieces.WHITE_PAWN]

        for piece in pieces:
            img = self.pieces.get(piece)
            img = pygame.transform.smoothscale(img,(p_size,p_size))

            if img:
                display.blit(img, (left,top))
                left += p_size

        self.btn_black.draw(display)
        self.btn_white.draw(display)
        self.author_text.print_text(display)
        self.chess_text.print_text(display)
        

    def print_board(self,display):

        col = True
        
        display.fill(background)

        for i in range(64):

            left = self.margin + (i % 8) * self.tile_size
            top = self.margin + 7 * self.tile_size - (i//8)*self.tile_size

            if col:
                pygame.draw.rect(surface=display,color=black,rect=pygame.Rect(left,top,self.tile_size,self.tile_size))
            else:
                pygame.draw.rect(surface=display,color=white,rect=pygame.Rect(left,top,self.tile_size,self.tile_size))

            if i % 8 != 7:
                col = not col
            

    def print_pieces(self,display,pieces_arrangement,side):

        for i, piece in enumerate(pieces_arrangement):

            if piece != Pieces.EMPTY:

                if side == Side.WHITE:
                    left = self.margin + (i % 8) * self.tile_size
                    top = self.margin + 7*self.tile_size - (i//8)*self.tile_size
                else:
                    left = self.margin + (7 - (i % 8)) * self.tile_size
                    top = self.margin + (i // 8) * self.tile_size
                

                image = self.pieces.get(piece)
            
                if image:
                    display.blit(image, (left, top))

    def print_legalMoves(self,display,moves,side):
        
        for i in moves:

            if side == Side.WHITE:
                left = self.margin + (i % 8) * self.tile_size
                top = self.margin + 7*self.tile_size - (i//8)*self.tile_size
            else:
                left = self.margin + (7 - (i % 8)) * self.tile_size
                top = self.margin + (i // 8) * self.tile_size
            
            pygame.draw.circle(surface=display,color=grey,center=(left + (self.tile_size//2),top + (self.tile_size//2)),radius=(self.tile_size//5))

            


    def clicked_on_players_piece(self,mouse_pos,pieces,side,pieceUp):
        
        left = mouse_pos[0] - 25
        top = mouse_pos[1] - 25

        top = (((8 * TILE_SIZE) - top) // TILE_SIZE)
        left = left // 100

        tile = left + top*8
        
        if side == Side.WHITE:
            if Pieces.WHITE_PAWN <= pieces[tile] and pieces[tile] <= Pieces.WHITE_KING:
                print(tile)
                return (True,tile)
        else:
            tile = 63 - tile
            if Pieces.BLACK_PAWN <= pieces[tile] and pieces[tile] <= Pieces.BLACK_KING:
                print(tile)
                return (True,tile)
            
        if pieceUp == Pieces.EMPTY:
            return (False,tile)
        else:
            return (True,tile)

        
            
    def display_game(self,display,pieces_arrangement,side,legalMoves):
        self.print_board(display)
        self.print_legalMoves(display,legalMoves,side)
        self.print_pieces(display,pieces_arrangement,side)

    def display_endgame(self):
        pass
