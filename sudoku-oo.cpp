#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
/*
Primitive Branchless Sudoku Solver 1.0.1 - Aric Volman, July 12th, 2021
Uses indices of rows and columns to generate candidates.
Then converts hidden pairs to naked pairs, which are solved directly.
Most puzzles don't solve completely with just these two methods!
TODO entropy research for pass optimization.

To Compile:  clang++ sudoku.cpp -o a

Try:
// Incomplete solution after 3 passes
./a 002003805968005004340019020827506030000000500530400087700082603000000278200307150
// Medium Puzzle 3,209,599,347 from Web Sudoku - solves completely in seven passes
./a 003006002602900300400100500100007600007040900009300008001004003005001806200500100
*/

// Borrowed from https://stackoverflow.com/questions/20965960/shannon-entropy for research purposes.

template <typename T> static float ShannonEntropy(T data[],int elements){
    float entropy=0;
    std::map<T,long> counts;
    typename std::map<T,long>::iterator it;
    //
    for (int dataIndex = 0; dataIndex < elements; ++dataIndex) {
        counts[data[dataIndex]]++;
    }
    //
    it = counts.begin();
    while(it != counts.end()){
        float p_x = (float)it->second/elements;
        if (p_x>0) entropy-=p_x*log(p_x)/log(2);
        it++;
    }
    return entropy;
}

// End of stackoverflow section.

class solver {
	public:
		std::vector<std::vector<int>> grid;
		std::vector<std::vector<int>> getGrid() {
			return grid;
		}

		void setGrid(std::vector<std::vector<int>> inputGrid) {
			grid = inputGrid;
		}
		int pass() {
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
				combineIndices();
				hiddenSinglesToNakedSingles();
				nakedSingles();
				printGrid();
				int complete = int(checkGridForCompletion()) ;
				std::cout << "Complete??:" << complete << std::endl;
				return complete * ec;
			}

		void solve() {
			for (int i = 1; i <= 10; i++) {
				int p = pass();
				if (p) {
					std::cout << p << std::endl;
					break;
				};
				if(p == -1) {
					std::cout << "Error" << std::endl;
				}
			}
		}

		solver(std::vector<std::vector<int>> grid) {
			this->grid = grid;
		}
	private:
		std::vector<std::vector<std::string>> index_combined;
		std::vector<std::string> index_row;
		std::vector<std::string> index_column;
		std::vector<std::vector<std::string>> index_square;
		int ec = 1;

		void findRowIndices() {
			// Row indices
			for (int m = 0; m < grid.size(); m++) {
				for (int i = 0; i < grid.at(0).size(); i++) {
					index_row.at(m)[grid.at(m).at(i)] += (grid.at(m).at(i) != 0);
				}
			}
		}
		void findColumnIndices() {
			// Column indices
			for (int m = 0; m < grid.at(0).size(); m++) {
				for (int i = 0; i < grid.size(); i++) {
					index_column.at(m)[grid.at(i).at(m)] += (grid.at(i).at(m) != 0);
				}
			}
		}
		void findSquareIndices() {
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
		void combineIndices() {
			// OR Indices, skip filled points by AND of grid.at(m).at(j)==0, the variable of which is passed to the function.
			for (int m = 0; m < index_row.size(); m++) {
				for (int j = 0; j < index_column.size(); j++) {
						index_combined.at(m).at(j) = or_indices(index_row.at(m), index_column.at(j), index_square.at((int)m/3).at((int)j/3), grid.at(m).at(j));
				}
			}
			// Final NOT and error check
			for (int i = 0; i < index_combined.at(0).size(); i++) {
				for (int j = 0; j < index_combined.size(); j++) {
					index_combined.at(i).at(j) = not_index(index_combined.at(i).at(j));
				}
			}
		}
		void hiddenSinglesToNakedSingles() {
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

		void nakedSingles() {
			for (int y = 0; y < grid.size(); y++) {
				for (int x = 0; x < grid.at(0).size(); x++) {
					grid.at(y).at(x) += (add_char(index_combined.at(y).at(x))==1)*find_num(index_combined.at(y).at(x));
				}
			}
		}

		void printGrid()  {
			std::cout << "" << std::endl;
			for (int y = 0; y < grid.size(); y++) {
				for (int x = 0; x < grid.at(0).size(); x++) {
					std::cout << grid.at(y).at(x) << " ";
				}
				std::cout << std::endl;
			}
		}

		bool checkGridForCompletion() {
			int sum;
			for (int y = 0; y < grid.size(); y++) {
				for (int x = 0; x < grid.at(0).size(); x++) {
					 sum = grid.at(y).at(x)  + sum;
				}
			}
			return (sum == 414); // 46 x 9
		}
		// --------------------------------------------------------------------------------------------------------------------------------------------------------------

		std::string not_index(std::string input) {
			for (int k = 0; k < input.size(); k++) {
				input.at(k) = input.at(k)^1;
			}
			// std::cout << input << std::endl;

			return input;
		}

		std::string or_indices(std::string A, std::string B, std::string C, int N) {
			std::string D = "";
			for (int k = 0; k < A.size(); k++) {
				// Converts character to int by subtracting '0', aka ascii code 48
				D = D + std::to_string((((A.at(k)-'0')||(B.at(k)-'0')||(C.at(k)-'0'))&&(N==0))^(N!=0));
			}
			return D;
		}

		int add_char(std::string input) {
			int sum = 0;
			for (int k = 1; k < input.size(); k++) {
				sum = (input.at(k)-'0')+sum;
			}
			return sum;
		}

		int find_num(std::string input) {
			int num = 0;
			for (int k = 1; k < input.size(); k++) {
				num = k*(input.at(k)-'0')+num;
			}
			return num;
		}
};


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

	auto entropy = ShannonEntropy<char>(argv[1], unprocessed_grid.size());

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
	// Print array
	
	std::cout << "Before solve:" << std::endl;

	solver mySolver(grid);
	mySolver.solve();
}
