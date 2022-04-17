#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>

class solver {
	public:
		std::vector<std::vector<int>> grid;

        std::vector<std::vector<int>> getGrid();
        void setGrid(std::vector<std::vector<int>> inputGrid);
        int pass();
        int solve();

        // Constructor method
        solver(std::vector<std::vector<int>> grid);

	private:
		std::vector<std::vector<std::string>> index_combined;
		std::vector<std::string> index_row;
		std::vector<std::string> index_column;
		std::vector<std::vector<std::string>> index_square;
		int duplicateFound = 0;
		int gridSum = 0;
		int previousSum = -1; // Something impossible to start with

		void findRowIndices() ;
        void findColumnIndices();
        void findSquareIndices();
        void combineIndices();
        void hiddenSinglesToNakedSingles();
        void nakedSingles();
        void printGrid();
        bool checkGridForCompletion();
        std::string not_index(std::string input);
        std::string or_indices(std::string A, std::string B, std::string C, int N);
        int add_char(std::string input);
        int find_num(std::string input);
};
