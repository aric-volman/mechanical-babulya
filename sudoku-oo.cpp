#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>

#include "lib/solvers/solver.cpp"
/*
Primitive Branchless Sudoku Solver 1.0.1 - Aric Volman, July 12th, 2021
Uses indices of rows and columns to generate candidates.
Then converts hidden pairs to naked pairs, which are solved directly.
Most puzzles don't solve completely with just these two methods!

To Compile:  clang++ sudoku.cpp -o a

Try:
// Incomplete solution after 3 passes
./a 002003805968005004340019020827506030000000500530400087700082603000000278200307150
// Medium Puzzle 3,209,599,347 from Web Sudoku - solves completely in seven passes
./a 003006002602900300400100500100007600007040900009300008001004003005001806200500100
*/

int main(int argc, char *argv[]) {

	if (argc != 2) {
		std::cerr << "Only one arg!" << std::endl;
		return -1;
	}
	std::string unprocessed_grid(argv[1]);
	if (unprocessed_grid.size() != 81) {
		std::cerr << "Incorrect size!" << std::endl;
		return -1;
	}

	// Init grid
	std::vector<std::vector<int>> grid(std::sqrt(unprocessed_grid.size()), std::vector<int>(std::sqrt(unprocessed_grid.size()), 0));

	// Converts entire string to 2D vector
	int total = 0;
	for (int y = 0; y < std::sqrt(unprocessed_grid.size()); y++) {
		for (int x = 0; x < std::sqrt(unprocessed_grid.size()); x++) {
			grid.at(y).at(x) = (unprocessed_grid.at(total)-'0');
			total++;
		}
	}
		
	/*
	std::vector<std::vector<int>> grid = { {0, 0, 2, 0, 0, 3, 8, 0, 5},
					{9, 6, 8, 0, 0, 5, 0, 0, 4},
					{3, 4, 0, 0, 1, 9, 0, 2, 0},
					{8, 2, 7, 5, 0, 6, 0, 3, 0},
					{0, 0, 0, 0, 0, 0, 5, 0, 0},
					{5, 3, 0, 4, 0, 0, 0, 8, 7},
					{7, 0, 0, 0, 8, 2, 6, 0, 3},
					{0, 0, 0, 0, 0, 0, 2, 7, 8},
					{2, 0, 0, 3, 0, 7, 1, 5, 0}};
	*/

	solver mySolver(grid);
	int ret = mySolver.solve();
}
