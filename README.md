## GridLink: A Mini Excel Clone
  GridLink is a lightweight, spreadsheet-like application built from scratch using C++ and the SFML multimedia library. Instead of a traditional array, this project uses a 2D doubly-linked list to form the grid, making it an interesting exploration of data structures.

The application provides a navigable grid where you can input text or numbers and perform basic calculations like Count, Sum, and Average on the numeric entries.

## Features:
  Navigable Grid: Move between cells using the arrow keys.

  Data Entry: Press Enter to type text or numbers into the selected cell.

  Dynamic Highlighting: The currently selected cell is highlighted for better visibility.

  Real-time Calculations:

  Count: Keep track of how many numbers have been included in the calculation.

  Sum: Calculate the total sum of the included numbers.

  Average: Compute the average of the included numbers.

## Controls: The controls are simple and intuitive:
  KeyActionArrow KeysNavigate up, down, left, or right through the grid. 
  Enter: Start editing the selected cell. Type your value and press Enter again to confirm.
  Spacebar: If the selected cell contains a number, add it to the running calculation (Count, Sum, Average).
  Delete: Reset the Count, Sum, and Average back to zero.
  Escape: Close the application.
## Getting Started: 
  Prerequisites: 
    A C++ compiler (MSVC, GCC, Clang)
    Visual Studio (the project is configured with a .sln file)
    
## Building the Project
  This project is configured for a portable setup, so you don't need to install SFML separately.
