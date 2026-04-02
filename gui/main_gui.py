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

        self.pieces_data = []
        self._board = Board(Side.WHITE,TILE_SIZE,MARGIN)
        self._engine = Engine()
        

    def on_event(self, event):
        if event.type == pygame.QUIT:
            self._running = False

        if event.type ==pygame.MOUSEBUTTONDOWN:

            if self._state == GameState.MENU:

                if event.button == 1:
                    mouse_pos = pygame.mouse.get_pos()
                            
                    if self._board.btn_white.is_clicked(mouse_pos):
                        self._side = Side.WHITE
                        self._state = GameState.IN_PROGRESS
                                     
                    elif self._board.btn_black.is_clicked(mouse_pos):
                        self._side = Side.BLACK
                        self._state = GameState.IN_PROGRESS

    def on_cleanup(self):
        pygame.quit()

 
    def on_execute(self):
        
        while self._running:
            for event in pygame.event.get():
                self.on_event(event)

            if self._state == GameState.MENU:
                self._board.display_menu(self.display)
            elif self._state == GameState.IN_PROGRESS:
                self._board.display_game(self.display,self._engine.get_pieces_arrangement(self._side))
            elif self._state == GameState.END:
                self._board.display_endgame()

            pygame.display.flip()

        self.on_cleanup()
 
if __name__ == "__main__" :
    theApp = App(WINDOW_SIZE,WINDOW_SIZE)
    theApp.on_execute()