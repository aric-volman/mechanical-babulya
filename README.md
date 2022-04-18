A basic sudoku solver

## What is implemented

# Algorithm
- Naked Singles
- Hidden Singles
- Detection of solver states (duplicates and the "stuck" condition)
# Other
- Import puzzles onto the grid
- Export puzzle on the grid to text file

## Goals - Near Term

# Algorithmn
- Implement Hidden Pairs
# Interface
- Graph the completion of the sudoku with every pass of the solver
- Find "difficulty" of puzzle
- Output these statistics to a JSON file
# Other
- Find a way to build on Windows - KDE Arch Linux was more convenient with Qt included

## Goals - Long Term
- Release a 1.0.0 release with dynamically-linked Qt, in the form of a Linux AppImage and Windows exe
- Create documentation of how the solver works (how naked singles are found, how hidden singles are converted to naked singles)
- A visual form of documentation? (GIFs of algorithms)