# GomokuGame

**An object-oriented C++ project to play Gomoku (Five in a Row).**

Gomoku is a traditional Japanese game, also known as "five in a row". The game is played on a 19x19 board called the Goban, where the pieces of the two players (Nero and Bianco) are placed at the intersections. 

The first move is always made by the player using the black pieces. The goal is to align five pieces of the same color in one of the four possible directions: vertical, horizontal, diagonal, or anti-diagonal. When a player aligns five pieces in a row, the game ends.

The proposed version is a simple implementation of the Gomoku that creates a Goban, asks both players to input their moves, keeps track of the game state, and ends the game if either the Nero or Bianco player wins.

Description of the Classes
-----------------------

- The **Pedina** class is characterized by a single field "colore" which is initialized as non-existent (with the value '.'). When a player places a piece on the board, it takes the color of the player ('N' or 'B').

- The **Goban** class is defined as a vector of vectors of Pedine. It handles printing the game state, registering the input move, checking for a win by counting the pieces in the allowed directions, and stopping the game if a winner is found.

Instructions for Using the Program
-----------------------

Once the game begins, the Goban is printed, and alternating input requests are made to the two players.

To make a move, the player must input the row and column where they want to place their piece, separated by a space.
