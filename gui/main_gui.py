from board_view import *
from engine_api import *

class GameState(Enum):
    MENU = 0
    IN_PROGRESS = 1
    END = 2

class App:
    def __init__(self,WINDOW_WIDTH,WINDOW_HEIGHT):
        pygame.time.Clock().tick(60)
        self._running = True
        self._display_surf = None
        self._state = GameState.MENU


        self.size = self.width, self.height = WINDOW_WIDTH, WINDOW_HEIGHT 
        pygame.init()
        self.display = pygame.display.set_mode(self.size, pygame.HWSURFACE | pygame.DOUBLEBUF)
        self._board = Board(TILE_SIZE,MARGIN)
        self.pieces_data = []
        self._engine = Engine('./engine/src/main')

        self._pieceUp = Pieces.EMPTY
        self._pieceSrc = -200
        self._legal_moves_for_clicked_tile = []
        

    def on_event(self, event):
        if event.type == pygame.QUIT:
            self._running = False

        if event.type == pygame.MOUSEBUTTONDOWN:

            if self._state == GameState.MENU:

                if event.button == 1:
                    mouse_pos = pygame.mouse.get_pos()
                            
                    if self._board.btn_white.is_clicked(mouse_pos):
                        self._side = Side.WHITE
                        self._state = GameState.IN_PROGRESS
                                     
                    elif self._board.btn_black.is_clicked(mouse_pos):
                        self._side = Side.BLACK
                        self._state = GameState.IN_PROGRESS

                        self._board.display_game(self.display,self.pieces_data,self._side,self._legal_moves_for_clicked_tile,self._pieceSrc)
                            
                        print(self._engine.send_command("go"))

                        self._engine.getfen()
                            
                        self.pieces_data = self._engine.get_pieces_arrangement()


            elif self._state == GameState.IN_PROGRESS:

                if event.button == 1:
                    mouse_pos = pygame.mouse.get_pos()

                click = self._board.clicked_on_players_piece(mouse_pos,self.pieces_data,self._side,self._pieceUp)
                print(click[1])

                if click[0] and self._pieceUp == Pieces.EMPTY:
                    self._pieceUp = self.pieces_data[click[1]]
                    self.pieces_data[click[1]] = Pieces.EMPTY
                    self._pieceSrc = click[1]

                    self._legal_moves_for_clicked_tile = [int(m) for m in self._engine.send_command(f"getmoves {click[1]}").split()]

                elif click[0] and click[1] in self._legal_moves_for_clicked_tile or click[1] == self._pieceSrc:
                    self.pieces_data[click[1]] = self._pieceUp
                    self._pieceUp = Pieces.EMPTY
                    self._legal_moves_for_clicked_tile = []
                    if click[1] != self._pieceSrc:

                        
                        self._engine.getfen()

                        a = (self._engine.send_command(f"position {self._pieceSrc}-{click[1]}"))
                        print(a)

                        self.pieces_data = self._engine.get_pieces_arrangement()
                        self._pieceSrc = -200

                        self._board.display_game(self.display,self.pieces_data,self._side,self._legal_moves_for_clicked_tile,self._pieceSrc)
                            
                        print(self._engine.send_command("go"))

                        self._engine.getfen()
                            
                        self.pieces_data = self._engine.get_pieces_arrangement()
                        
                    else:
                        self._pieceSrc = -200

                    

    def on_cleanup(self):
        pygame.quit()

 
    def on_execute(self):

        self.pieces_data = self._engine.get_pieces_arrangement()

        while self._running:
            for event in pygame.event.get():
                self.on_event(event)

            if self._state == GameState.MENU:
                self._board.display_menu(self.display)
            elif self._state == GameState.IN_PROGRESS:

                self._board.display_game(self.display,self.pieces_data,self._side,self._legal_moves_for_clicked_tile,self._pieceSrc)

                if self._pieceUp != Pieces.EMPTY:
                    mouse_pos = pygame.mouse.get_pos()
                    image = self._board.pieces.get(self._pieceUp)
                    self.display.blit(image, (mouse_pos[0] - TILE_SIZE // 2, mouse_pos[1] - TILE_SIZE // 2))

            elif self._state == GameState.END:
                self._board.display_endgame()

            pygame.display.flip()

        self.on_cleanup()
 
if __name__ == "__main__" :
    theApp = App(WINDOW_SIZE,WINDOW_SIZE)
    theApp.on_execute()