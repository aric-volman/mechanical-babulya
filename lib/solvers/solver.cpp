#include "solver.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>


std::vector<std::vector<int>> solver::getGrid() {
	return grid;
}

void solver::setGrid(std::vector<std::vector<int>> inputGrid) {
	grid = inputGrid;
}
int solver::pass() {
	std::vector<std::vector<std::string>> init_index_combined(grid.size(), std::vector<std::string>(grid.size(), "0000000000")); // Turns into combined indices later. index_combined is a variable

	std::vector<std::vector<std::string>> init_index_square(std::sqrt(grid.size()), std::vector<std::string>(std::sqrt(grid.size()), "0000000000"));
	std::vector<std::string> init_index_row;
	std::vector<std::string> init_index_column;

    // Init row indices, with a size of the number of rows.
	for (int i=0; i<grid.size(); i++) init_index_row.push_back("0000000000");

	// Init column indices, with a size of the number of columns.
	// Note that grid.at(0).size() is the size of the first row.
	for (int i=0; i<grid.at(0).size(); i++) init_index_column.push_back("0000000000");

	index_combined = init_index_combined;
	index_row = init_index_row;
	index_column = init_index_column;
	index_square = init_index_square;

	findRowIndices();
	findColumnIndices();
	findSquareIndices();
	combineIndices(); // Also checks for duplicates here

	hiddenSinglesToNakedSingles();
	nakedSingles();
	printGrid();

	int complete = int(checkGridForCompletion());
	return complete;
}

int solver::solve() {
    int retStatus = 0;
    for (int i = 1; i <= 10; i++) {
        if (pass()) {
            std::cout << "Solved!" << std::endl;
            retStatus = 1;
            break;
        } else if  (duplicateFound) {
            std::cout << "Duplicate found" << std::endl;
            retStatus = 2;
            break;
        } else if (gridSum == previousSum) {
            std::cout << "Impossible puzzle!" << std::endl;
            retStatus = 3;
            break;
        }
        previousSum = gridSum;
    }
    return retStatus;
}

solver::solver(std::vector<std::vector<int>> grid) {
    this->grid = grid;
}

// Begin section of Private methods
void solver::findRowIndices() {
// Row indices
    for (int m = 0; m < grid.size(); m++) {
        for (int i = 0; i < grid.at(0).size(); i++) {
            index_row.at(m)[grid.at(m).at(i)] += (grid.at(m).at(i) != 0);
        }
    }
}
		void solver::findColumnIndices() {
			// Column indices
			for (int m = 0; m < grid.at(0).size(); m++) {
				for (int i = 0; i < grid.size(); i++) {
					index_column.at(m)[grid.at(i).at(m)] += (grid.at(i).at(m) != 0);
				}
			}
		}
		void solver::findSquareIndices() {
			// Square indices
			for (int y = 0; y < std::sqrt(grid.size()); y++) {
				for (int x = 0; x < std::sqrt(grid.at(0).size()); x++) {
					for (int b = 0; b < std::sqrt(grid.size()); b++) {
						for (int a = 0; a < std::sqrt(grid.at(0).size()); a++) {
							// std::cout << grid.at(3*y + b).at(3*x + a);
							index_square.at(y).at(x)[grid.at(3*y + b).at(3*x + a)] += (grid.at(3*y + b).at(3*x + a) != 0);
						}
						// std::cout << std::endl;
					}
					//std::cout << index_square.at(y).at(x) << std::endl;
				}
			}
		}
		void solver::combineIndices() {
			// OR Indices, skip filled points by AND of grid.at(m).at(j)==0, the variable of which is passed to the function.
			for (int m = 0; m < index_row.size(); m++) {
				for (int j = 0; j < index_column.size(); j++) {
						index_combined.at(m).at(j) = or_indices(index_row.at(m), index_column.at(j), index_square.at((int)m/3).at((int)j/3), grid.at(m).at(j));
				}
			}
			// Final NOT
			for (int i = 0; i < index_combined.at(0).size(); i++) {
				for (int j = 0; j < index_combined.size(); j++) {
					index_combined.at(i).at(j) = not_index(index_combined.at(i).at(j));
				}
			}
		}
		void solver::hiddenSinglesToNakedSingles() {
			// Hidden singles to naked singles conversion

			std::vector<std::vector<std::string>> sums(std::sqrt(grid.size()), std::vector<std::string>(std::sqrt(grid.size()), "000000000"));
			for (int y = 0; y < std::sqrt(grid.size()); y++) {
				for (int x = 0; x < std::sqrt(grid.at(0).size()); x++) {
					std::vector<std::vector<std::string>> sums(std::sqrt(grid.size()), std::vector<std::string>(std::sqrt(grid.size()), "000000000"));
					std::vector<std::vector<std::string>> square_and(std::sqrt(grid.size()), std::vector<std::string>(std::sqrt(grid.size()), "0000000000"));
					for (int b = 0; b < std::sqrt(grid.size()); b++) {
						for (int a = 0; a < std::sqrt(grid.at(0).size()); a++) {
							for (int i = 1; i < index_combined.at(3*y + b).at(3*x + a).size(); i++) {
								sums.at(y).at(x)[i-1] += (index_combined.at(3*y + b).at(3*x + a)[i]-'0');
							}
						}
						// std::cout << std::endl;
					}
					// std::cout << sums.at(y).at(x) << std::endl;
					for (int i = 0; i < sums.at(y).at(x).size(); i++) {
						int buff = (1*(sums.at(y).at(x)[i]=='1'));
						sums.at(y).at(x)[i] = (char)(buff+'0'); // Convert int to char
						// std::cout << sums.at(y).at(x)[i] << std::endl;
						// std::cout << "Result: " << typeid((char)(1*(sums.at(y).at(x)[i]=='1'))).name() << std::endl;

					}
					// std::cout << sums.at(y).at(x) << std::endl;
					for (int b = 0; b < std::sqrt(grid.size()); b++) {
						for (int a = 0; a < std::sqrt(grid.at(0).size()); a++) {
							for (int i = 1; i < index_combined.at(3*y + b).at(3*x + a).size(); i++) {
								square_and.at(b).at(a)[i] += ((index_combined.at(3*y + b).at(3*x + a)[i]-'0')&&(sums.at(y).at(x)[i-1]-'0')); // AND the index with sums
							}
							for (int i = 1; i < index_combined.at(3*y + b).at(3*x + a).size(); i++) {
								// Either leave it alone if the add_char returns something that is not 1, or convert to naked single if 1.
								index_combined.at(3*y + b).at(3*x + a)[i] = (index_combined.at(3*y + b).at(3*x + a)[i]*(add_char(square_and.at(b).at(a))!=1)) + ((square_and.at(b).at(a)[i])*(add_char(square_and.at(b).at(a))==1));
							}
							// std::cout << index_combined.at(3*y + b).at(3*x + a) << std::endl;
						}
						// std::cout << std::endl;
					}
					// std::cout << sums.at(y).at(x) << std::endl;
				}
			}
		}

		void solver::nakedSingles() {
			for (int y = 0; y < grid.size(); y++) {
				for (int x = 0; x < grid.at(0).size(); x++) {
					grid.at(y).at(x) += (add_char(index_combined.at(y).at(x))==1)*find_num(index_combined.at(y).at(x));
				}
			}
		}

		void solver::printGrid()  {
			std::cout << "" << std::endl;
			for (int y = 0; y < grid.size(); y++) {
				for (int x = 0; x < grid.at(0).size(); x++) {
					std::cout << grid.at(y).at(x) << " ";
				}
				std::cout << std::endl;
			}
		}

		bool solver::checkGridForCompletion() {
			gridSum = 0;
                        for (int y = 0; y < grid.size(); y++) {
                                for (int x = 0; x < grid.at(0).size(); x++) {
                                         gridSum = grid.at(y).at(x) + gridSum;
                                }
                        }
			std::cout << "Sum: " << gridSum << std::endl;
			return (gridSum == 405); // 45 x 9
		}
		// --------------------------------------------------------------------------------------------------------------------------------------------------------------

		std::string solver::not_index(std::string input) {
			// std::cout << input << std::endl;
			for (int k = 0; k < input.size(); k++) {
				input.at(k) = input.at(k)^1;
			}
			// std::cout << input << std::endl;

			return input;
		}

		std::string solver::or_indices(std::string A, std::string B, std::string C, int N) {
			std::string D = "";
			for (int k = 0; k < A.size(); k++) {
				// Converts character to int by subtracting '0', aka ascii code 48
				D = D + std::to_string((((A.at(k)-'0')||(B.at(k)-'0')||(C.at(k)-'0'))&&(N==0))^(N!=0));
				// Checks for duplicates
				if ((A.at(k)-'0') > 1 || (B.at(k)-'0') > 1 || (C.at(k)-'0') > 1) {
					duplicateFound = 1;
				}
			}
			return D;
		}

		int solver::add_char(std::string input) {
			int sum = 0;
			for (int k = 1; k < input.size(); k++) {
				sum = (input.at(k)-'0')+sum;
			}
			return sum;
		}

		int solver::find_num(std::string input) {
			int num = 0;
			for (int k = 1; k < input.size(); k++) {
				num = k*(input.at(k)-'0')+num;
			}
			return num;
		}
