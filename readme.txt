Christopher Valerio

Compilation Instructions: gcc -o CValerio_thd.exe CValerio_thd.c -lpthread

Execution Instructions: CValerio_thd.exe 1000000 10 10

My program using a sumList LinkedList data structure instead of a global sum array

Program takes in three command line arguments. 
1) summation limit  
2) number of Child Threads
3) number of grandchildren threads for each child thread

Summation value is then calculated from 1 to the given summation limit. Program splits the workload of calculating the
sum among many processing threads. From the original summation limit, child threads are created each handeling a 
sub interval from 1 to given summation limit. And from each of those child threads, grand child threads are created and the workload of the summation
interval is further divided up  on more time before calculating the actual sum for each sub-interval.

when choosing values for the number of child and grandchild threads to create
make sure they both divide nicely into the given summation limit 

