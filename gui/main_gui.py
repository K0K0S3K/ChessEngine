from board_view import *

class GameState(Enum):
    MENU = 0
    IN_PROGRESS = 1
    END = 2

class App:
    def __init__(self,WINDOW_WIDTH,WINDOW_HEIGHT):
        self._running = True
        self._display_surf = None
        self._state = GameState.IN_PROGRESS
        self.size = self.width, self.height = WINDOW_WIDTH, WINDOW_HEIGHT 
        pygame.init()
        self.display = pygame.display.set_mode(self.size, pygame.HWSURFACE | pygame.DOUBLEBUF)
        self._board = Board(Side.WHITE,100,25)
        

    def on_event(self, event):
        if event.type == pygame.QUIT:
            self._running = False

    def on_cleanup(self):
        pygame.quit()
 
    def on_execute(self):
        
        while self._running:
            for event in pygame.event.get():
                self.on_event(event)

            if self._state == GameState.MENU:
                self._board.display_menu()
            elif self._state == GameState.IN_PROGRESS:
                self._board.display_game(self.display,"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
            elif self._state == GameState.END:
                self._board.display_endgame()

            pygame.display.flip()

        self.on_cleanup()
 
if __name__ == "__main__" :
    theApp = App(850,850)
    theApp.on_execute()