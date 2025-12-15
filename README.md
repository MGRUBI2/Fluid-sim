# Fluid-sim

## About

This project is about fluid simulation in language C++ and graphic library SDL3.
It is work in progress. For now spatial hash grid and thread pool are implemented for better
performance. There is still work with project optimisation and bug fix.
After that is will begin implementig flip and pic method for fluid physics.

## Problems

- Some functions have unnecessary nested for loops and should be replaced with specific algorithms

- because of poor planning while programming thread pool i made functions that splits vector (or span) and 
  sends it too thread. Each threads then makes its own spatial hash map which is wrong and slow.
  I need to make one big spatial hash map and then send cells to the threads and check for colision and rest

- Bad colision check. Some objects are not checked. Prior problem is probably the reason for this.

- program might crash. Usualy problem is poor mutex implementation although last update fixed most of crashing
  and made program stable

