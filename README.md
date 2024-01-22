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

## Execution time - 
This is a rough expectation of computing speed (Just so you know the program isnt broken, its just running).
Both programs will return the total execution time of the algorithm before printing all extreme points

Locally, the bruteforce program takes around 25 seconds, but expect this to take much longer if using an older processor
The divideAndConquer program should run very fast (Less than 0.1 secconds)
