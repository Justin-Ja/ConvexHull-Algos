# Compiling:
cd into the folder containing all of the C files, necessary text files and the makefile.
Run the command 
```
make
```
This will generate 2 executables: bruteforce, divideAndConquer

The executables can be run as such:
./bruteforce
./divideAndConquer

For both they will prompt for two points s1 and s2
They should be entered with only a space separating the x and y with nothing else -> 123.4 567.8

To remove the executable files, run

```
make clean
```

## Notes on input
For now, both programs are set up to expect an input file of 30,000 pairs of co-ordinate points. Future plans is to update to make this dynamic
- [ ] To allow for input to be of dynamic size

## Execution time
This is a rough expectation of computing speed (Just so you know the program isnt broken, its just running).
Both programs will return the total execution time of the algorithm before printing all extreme points

Locally, the bruteforce program takes around 25 seconds, but expect this to take much longer if using an older processor. 
The divideAndConquer program should run very fast (Less than 0.1 secconds)


### Example output
```
Enter x and y coordinates of s1 separated by a space (x y): 28.3 1205.6
Enter x and y coordinates of s2 separated by a space (x y): 5561.2 6328.5
Computing...
For P21 - Brute force Convex Hull problem
Time taken: 23.552558
Number of hull points found: 22
Printing out ALL of the hull points
Hull Point 1: (4336.6, 6384.0)
Hull Point 2: (3839.0, 6397.2)
Hull Point 3: (5561.2, 6328.5)
Hull Point 4: (145.7, 517.0)
Hull Point 5: (272.6, 240.1)
Hull Point 6: (122.9, 621.9)
Hull Point 7: (5961.6, 6274.5)
Hull Point 8: (6163.4, 6092.8)
Hull Point 9: (6382.1, 5517.2)
Hull Point 10: (6416.4, 3607.9)
Hull Point 11: (1769.3, 6414.2)
Hull Point 12: (2238.7, 6426.4)
Hull Point 13: (484.1, 6266.3)
Hull Point 14: (1156.3, 24.3)
Hull Point 15: (5616.9, 37.6)
Hull Point 16: (3943.7, 32.0)
Hull Point 17: (6265.4, 73.0)
Hull Point 18: (120.9, 6008.0)
Hull Point 19: (30.8, 3064.0)
Hull Point 20: (128.3, 6091.3)
Hull Point 21: (6434.5, 1065.1)
Hull Point 22: (28.3, 1205.6)

The points from s1 to s2 inclusive are:
(28.3, 1205.6)
(30.8, 3064.0)
(120.9, 6008.0)
(128.3, 6091.3)
(484.1, 6266.3)
(1769.3, 6414.2)
(2238.7, 6426.4)
(3839.0, 6397.2)
(4336.6, 6384.0)
(5561.2, 6328.5)
Total points on path: 10
Total Distance: 10371.4
```
