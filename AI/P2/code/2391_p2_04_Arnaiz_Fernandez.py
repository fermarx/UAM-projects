
from __future__ import annotations  # For Python 3.7

import time
import numpy as np
from game import Player, TwoPlayerGameState, TwoPlayerMatch, TwoPlayerGame
from tournament import StudentHeuristic, Tournament
from typing import Sequence
from game import Player
from tictactoe import TicTacToe
from reversi import (
    Reversi,
    from_array_to_dictionary_board,
    from_dictionary_to_array_board,
)
from heuristic import simple_evaluation_function

class Solution1(StudentHeuristic):
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
      

class Solution2(StudentHeuristic):
    def get_name(self) -> str:
        return "CornerAdorner"
    def evaluation_function(self, state: TwoPlayerGameState) -> float:
        corners = [(1,1),(1,8), (8,8), (8,1)]
        lateral = [(1,3),(1,4),(1,5),(1,6),(3,1),(4,1),(5,1),(6,1),(3,8),(4,8),(5,8),(6,8),(8,3),(8,4),(8,5),(8,6)]
        surrounding_corners = [(1,2),(2,2),(2,1),(1,7),(2,7),(2,8),(7,1),(7,2),(8,2),(8,7),(7,7),(7,8)]
        bestMove = -100

        max_checkboard = state.game.generate_successors(state)

        for maxc in max_checkboard:
            m = maxc.move_code 
            if m in corners:
                bestMove = 100
                break
            elif m in lateral:
                if(bestMove<40):
                    bestMove = 40
            elif m in surrounding_corners:
                if(bestMove<-50):
                    bestMove = -50
            else:
                if(bestMove<10):
                    bestMove = 10

        return bestMove


class Solution3(StudentHeuristic):
    def get_name(self) -> str:
        return "WeightBait"
    def evaluation_function(self, state: TwoPlayerGameState) -> float:
        
        corners = [(1,1),(1,8), (8,8), (8,1)]
        lateral = [(1,3),(1,4),(1,5),(1,6),(3,1),(4,1),(5,1),(6,1),(3,8),(4,8),(5,8),(6,8),(8,3),(8,4),(8,5),(8,6)]
        surrounding_corners = [(1,2),(2,2),(2,1),(1,7),(2,7),(2,8),(7,1),(7,2),(8,2),(8,7),(7,7),(7,8)]
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