A basic sudoku solver

# What is implemented

### Algorithm
- Naked Singles
- Hidden Singles
- Detection of solver states (duplicates and the "stuck" condition)
### Other
- Import puzzles onto the grid
- Export puzzle on the grid to text file

# Goals - Near Term

### Algorithm
- Implement Hidden Pairs
### Interface
- Graph the completion of the sudoku with every pass of the solver
- Find "difficulty" of puzzle
- Output these statistics to a JSON file

# Goals - Long Term
- Release a 0.0.1 release with dynamically-linked Qt, in the form of a Linux AppImage and Windows Appx
- Create documentation of how the solver works (how naked singles are found, how hidden singles are converted to naked singles)
- A visual form of documentation? (GIFs of algorithms)

# Building on Windows
- Install Qt SDK
- Open the .pro file (the one that doesn't say Linux)
### Making a release for Windows - WIP
- Use windeployqt
- Copy files into Appx folder??
- Compress and rename

# Building on Linux
- Install Qt Creator only
- Open the Linux .pro file
### Making a release for Linux - WIP
- Copy files into Appimage folder??
- Compress and rename