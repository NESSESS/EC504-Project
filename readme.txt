If you want to see the figure:

How to compile the code:
	module load gcc
	g++ Source.cpp

How to use the program:
	a.out argv[0] argv[1]

	The argv[0] decide how many rounds the program will run. Every round we have N=N*2. Argv[1] decide the initial value of N.

How to plot the figure:
	a.out. argv[0] argv[1] >>data     (the name must be 'data')
	./figure.sh			   (show the figure directly, must be run in a machine with gnuplot)

Note: Please delete the old data file when you want to run "a.out. argv[0] argv[1] >>data" to get a new data file.


