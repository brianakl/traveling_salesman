# traveling_salesman
This program will input a distance matrix and uses the boost graph library
to implement various algorithms to find solutions to the tsp 
These algorithms include:
- Brute Force
- Nearest Neighbor
- Genetic Algorithm
- Christofides

To run you must have Boost Graph Library installed.

After intstalling you should run:

```
make main
./main
```
Now you can type in your input file and choose an algorithm to solve TSP (more examples in txt_files directory). For example:

```
p01_d.txt
```
Then enter the input file type by entering 1. Then select the algorithm to run.

The algorithm you selected will then run and output the path it generated with the total distance found.

Note: this is a NP-Hard problem and some sample problems in txt_files will take a long time to execute.


