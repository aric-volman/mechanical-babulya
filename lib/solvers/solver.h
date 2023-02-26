#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <list>
#include <string>
#include <stdio.h>
#include <stdlib.h>

class solver {
	public:
		std::vector<std::vector<int>> grid;

        std::vector<std::vector<int>> getGrid(){return grid;};
        void setGrid(std::vector<std::vector<int>> inputGrid);
        std::list<double> getGridCompletionVector();
        int pass();
        int solve();

        // Constructor method
        solver(std::vector<std::vector<int>> grid);
        solver();

        // Deconstructor
        ~solver();

	private:
        std::vector<std::vector<std::string>> index_combined;
        std::vector<std::string> index_row;
        std::vector<std::string> index_column;
        std::vector<std::vector<std::string>> index_square;
        std::list<double> completionVector;
        int duplicateFound;
        int gridSum;
        int previousSum; // Something impossible to start with

        double getGridCurrentCompletion();
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
#endif // SOLVER_H
