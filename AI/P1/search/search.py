# search.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


"""
In search.py, you will implement generic search algorithms which are called by
Pacman agents (in searchAgents.py).
"""

import util

class SearchProblem:
    """
    This class outlines the structure of a search problem, but doesn't implement
    any of the methods (in object-oriented terminology: an abstract class).

    You do not need to change anything in this class, ever.
    """

    def getStartState(self):
        """
        Returns the start state for the search problem.
        """
        util.raiseNotDefined()

    def isGoalState(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state.
        """
        util.raiseNotDefined()

    def getSuccessors(self, state):
        """
          state: Search state

        For a given state, this should return a list of triples, (successor,
        action, stepCost), where 'successor' is a successor to the current
        state, 'action' is the action required to get there, and 'stepCost' is
        the incremental cost of expanding to that successor.
        """
        util.raiseNotDefined()

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.
        The sequence must be composed of legal moves.
        """
        util.raiseNotDefined()


def tinyMazeSearch(problem):
    """
    Returns a sequence of moves that solves tinyMaze.  For any other maze, the
    sequence of moves will be incorrect, so only use this for tinyMaze.
    """
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    return  [s, s, w, s, w, w, s, w]

def depthFirstSearch(problem):
    """
    Search the deepest nodes in the search tree first.

    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:

    print("Start:", problem.getStartState())
    print("Is the start a goal?", problem.isGoalState(problem.getStartState()))
    print("Start's successors:", problem.getSuccessors(problem.getStartState()))
    """
    openList = util.Stack() #Open list for those nodes we have not expanded but visited
    closedList = [] #Closed list for nodes that have been expanded

    """ first we add the root node to the open list, after that we start 
    a loop where, if the first node is not the goal, we add it to
    the closed list and we add their successors to the open list.
    We use as queue a stack. This is because we need to have first the successors
    of the recently expanded node because we are in LIFO.
    If the first element of the stack is on the closed list we do not
    add it to the closed list as it would make a infinity cycle.
    If it is the node we are looking for, we add it to the closed list
    and we finish"""
    openList.push((problem.getStartState(), [])) # node, path, cost(we do not need it here)
    while openList.isEmpty() == False:
        ele = openList.pop()
        if problem.isGoalState(ele[0]) == False:
            closedList.append(ele[0])
            for successors in problem.getSuccessors(ele[0]):
                if successors[0] not in closedList:
                    successor = (successors[0], ele[1] + [successors[1]])
                    openList.push(successor)
        else:
            closedList.append(ele[0])
            return ele[1] #return the path
    return [] # if it does not find a solution it returns null

def breadthFirstSearch(problem):
    """Search the shallowest nodes in the search tree first."""
    openList = util.Queue() #Open list for those nodes we have not expanded but visited
    closedList = [] #Closed list for nodes that have been expanded
    
    """first we add the root node to the open list.
    Once we have in the open list the children of the root
    we start a loop where, if the first node (as this is a FIFO)
    is not the goal, we check if it is in the closed list ad if it is not,
    we add their successors to the openList.
    If it is the node we are looking for, we finish"""
    openList.push((problem.getStartState(), [])) # node, path, cost
    while openList.isEmpty() == False:
        ele = openList.pop()
        if problem.isGoalState(ele[0]) == False:
            if ele[0] not in closedList:
                closedList.append(ele[0])
                for successors in problem.getSuccessors(ele[0]):
                    successor = (successors[0], ele[1] + [successors[1]])
                    openList.push(successor)
        else:
            closedList.append(ele[0])
            return ele[1] #return the path
    return [] # if it does not find a solution it returns null

def uniformCostSearch(problem):
    """Search the node of least total cost first."""
    openList = util.PriorityQueue() #Open list for those nodes we have not expanded but visited
    closedList = [] #Closed list for nodes that have been expanded

    """ first we add the root node to the open list, after that we start 
    a loop where, if the first node is not the goal, we add it to
    the closed list and we add their successors to the open list.
    We use as queue a priority list. This is because we need to have into account
	the cost of each patch.
    If the first element of the list is not on the closed list we search for 
	its successors and add it to the open list havieng into account the cost
	of the path
    If it is the node we are looking for, we add it to the closed list
    and we finish"""
    openList.push((problem.getStartState(), [], 0), 0) # node, path, cost
    while openList.isEmpty() == False:
        ele = openList.pop()
        if problem.isGoalState(ele[0]) == False:
            if ele[0] not in closedList:
                closedList.append(ele[0])
                for successors in problem.getSuccessors(ele[0]):
                    successor = (successors[0], ele[1] + [successors[1]], ele[2] + successors[2])
                    openList.update(successor, ele[2] + successors[2])
        else:
            closedList.append(ele[0])
            return ele[1] #return the path
    return [] # if it does not find a solution it returns null

def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    return 0

def aStarSearch(problem, heuristic=nullHeuristic):
    """Search the node that has the lowest combined cost and heuristic first."""
    openList = util.PriorityQueue() #Open list for those nodes we have not expanded but visited
    closedList = [] #Closed list for nodes that have been expanded
    
    openList.push((problem.getStartState(), [], 0), 0) #May put the heuristic of the start state isntead of 0 but is the first one so it doesn't matter
    while openList.isEmpty() == True:
        ele = openList.pop()
        if problem.isGoalState(ele[0]) == False:
            if ele[0] not in closedList:
                closedList.append(ele[0])
                for successors in problem.getSuccessors(ele[0]):
                    fSuccessor = successors[2] + heuristic(successors[0], problem) # f(n) = g(n) + h(n) 
                    realCostSuccessor = ele[2] - heuristic(successors[0], problem) # g(n) = f(n) - h(n)
                    
                    successor = (successors[0], ele[1] + [successors[1]], realCostSuccessor + fSuccessor)
                    openList.update(successor, realCostSuccessor + fSuccessor)
                    
        else:
            closedList.append(ele[0])
            return ele[1]
            
    return [] # if it does not find a solution it returns null

# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch
