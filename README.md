# Labyrinth-Game

## Introduction
This project was done as part of the C Language module at Polytech Sorbonne.

## Description
This program implements 2 different algorithms to play The Amazeing Labyrinth game.
In a first approach, we use an algorithm that bruteforces all next moves (and the moves after for the opponent) and tries to take the move that is best for us and worse for the opponent.
This algorithm is fast and produces satisfactory results.

In a second approach, we implemented the Minimax algorithm with pruning to speed up the phase of graph building. This algorithm produced similar results compared to the first approach albeit being slower.
The maximum depth that produced the best results was 2, and the efficacy of the Minimax algorithm decreased as we increased the maximum depth.

The Minimax algorithm has a slight flaw, when it needs to reach the last item it loops around for a while before taking the item, which is why we use the first approach to find the last item.

We included 2 files, a Makefile as well as a CMakeLists.txt file so you can use the editor of your choice to compile it and run.

You can set debug to 1 to show debugging messages or 0 to just show whether we won or not.
At the end of the specified amount of runs (variable right before the game's uppermost loop), the program calculates the percentage of wins against the opponent.

## /!\ Note
Some files are not included, such as external libraries that enable communication with the game server. For those libraries, refer to [thilaire/Labyrinthe-Polytech](https://github.com/thilaire/Labyrinthe-Polytech).

## License
This project is licensed under the MPL2.0 License, which allows you to use, modify, and distribute the code freely. See the LICENSE file for more information.
