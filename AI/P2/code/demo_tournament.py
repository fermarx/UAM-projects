"""Illustration of tournament.

Authors:
    Alejandro Bellogin <alejandro.bellogin@uam.es>

"""

from __future__ import annotations  # For Python 3.7

import numpy as np

from game import Player, TwoPlayerGameState, TwoPlayerMatch
from heuristic import simple_evaluation_function, heuristic
from tictactoe import TicTacToe
from tournament import StudentHeuristic, Tournament
from typing import Sequence, List
from reversi import (
    Reversi,
    from_array_to_dictionary_board,
    from_dictionary_to_array_board,
)
from strategy import (
    ManualStrategy,
    MinimaxAlphaBetaStrategy,
    MinimaxStrategy,
    RandomStrategy,
)

class Heuristic2(StudentHeuristic):

    def get_name(self) -> str:
        return "random"

    def evaluation_function(self, state: TwoPlayerGameState) -> float:
        return float(np.random.rand())

class Heuristic4(StudentHeuristic):
    def get_name(self) -> str:
        return "FuturisticHeuristic"

    def evaluation_function(self, state: TwoPlayerGameState) -> float:
        scores = state.scores
        assert isinstance(scores, (Sequence, np.ndarray))
        if state.is_player_max(state.player1):
            aux = scores[0] - scores[1] 

        elif state.is_player_max(state.player2):
            aux =  -(scores[0] - scores[1])

        else:
            raise ValueError('Error in FuturisticHeuristic: not MAX player defined.')

        return aux
      
class Heuristic5(StudentHeuristic):
    def get_name(self) -> str:
        return "WeightBait"
    def evaluation_function(self, state: TwoPlayerGameState) -> float:
        """
        sumar score y el peso de la casilla (esquinas=5, laterales=4, centro=2, alrededor de las esquinas=-4)
        si el próximo jugador no puede realizar movimientos sumar 5
        """
        corners = [(1,1),(1,8), (8,8), (8,1)]
        lateral = [(1,3),(1,4),(1,5),(1,6),(3,1),(4,1),(5,1),(6,1),(3,8),(4,8),(5,8),(6,8),(8,3),(8,4),(8,5),(8,6)]
        surrounding_corners = [(1,2),(2,2),(2,1),(1,7),(2,7),(2,8),(7,1),(7,2),(8,2),(8,7),(7,7),(7,8)]
        bestMove = -4
        surrounding_corners_positive = {(1,1): [(1,2), (2,2), (2,1)], 
                                        (1,8): [(1,7), (2,7), (2,8)], 
                                        (8,8): [(8,7), (7,7), (7,8)], 
                                        (8,1): [(7,1), (7,2), (8,2)]}

        totalSum=0
        for square in state.board:
            value = state.board.get(square)
            if value==None:
                pass
            elif square in corners:  
                if(value=="W"):
                    totalSum += 100
                    for surr in surrounding_corners_positive.get(square):
                        if state.board.get(surr)=="W":
                            totalSum += 30
                        elif state.board.get(surr)=="B":
                            totalSum -= 20
                    surrounding_corners.remove(surr)
                elif(value=="B"):
                    totalSum -= 100
                    for surr in surrounding_corners_positive.get(square):
                        if state.board.get(surr)=="W":
                            totalSum += 20
                        elif state.board.get(surr)=="B":
                            totalSum -= 30
                    surrounding_corners.remove(surr)

            elif square in lateral:
                if(value=="W"):
                    totalSum += 40
                elif(value=="B"):
                    totalSum -= 40

            elif square in surrounding_corners:
                if(value=="W"):
                    totalSum -= 50
                elif(value=="B"):
                    totalSum += 50

            else:
                if(value=="W"):
                    totalSum += 10
                elif(value=="B"):
                    totalSum -= 10

        return totalSum

def create_match(player1: Player, player2: Player) -> TwoPlayerMatch:
    dim_board = 8

    #initial_board = np.zeros((dim_board, dim_board))
    initial_player = player1

    # initial_board = None  # Standard initial board.

    initial_board = (
        ['........',
         '........',
         '........',
         '...BW...',
         '...WB...',
         '........',
         '........',
         '........']
    )
    initial_board = from_array_to_dictionary_board(initial_board)

    game = Reversi(
        player1=player1,
        player2=player2,
        height=dim_board,
        width=dim_board,
    )

    game_state = TwoPlayerGameState(
        game=game,
        board=initial_board,
        initial_player=initial_player,
    )
    return TwoPlayerMatch(game_state, max_sec_per_move=1000, gui=False)

tour = Tournament(max_depth=3, init_match=create_match)
strats = {'opt2': [Heuristic2],'opt4': [Heuristic4], 'opt5': [Heuristic5]}

n = 1
scores, totals, names = tour.run(
    student_strategies=strats,
    increasing_depth=False,
    n_pairs=n,
    allow_selfmatch=False,
)

print(
    'Results for tournament where each game is repeated '
    + '%d=%dx2 times, alternating colors for each player' % (2 * n, n),
)

# print(totals)
# print(scores)

print('\ttotal:', end='')
for name1 in names:
    print('\t%s' % (name1), end='')
print()
for name1 in names:
    print('%s\t%d:' % (name1, totals[name1]), end='')
    for name2 in names:
        if name1 == name2:
            print('\t---', end='')
        else:
            print('\t%d' % (scores[name1][name2]), end='')
    print()
