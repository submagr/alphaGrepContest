*Circular Printer*

Write a program which takes four inputs (command line)

1. A string
2. char count (integer)
3. thread count - no of Threads(integer).
4. k: no of times each thread will print (integer)
[The program should exit after all threads have printed exactly k number of
times]

Each thread then should start printing the chars in the string sequentially
in the following format:
  eg:

  > myprinter ABCDEFGHIJ 3 4 2

  OutPut:

  Using 4 threads to print ABCDEFGHIJ alternatively in set of 3 chars

Thread1: ABC

Thread2: DEF

Thread3: GHI

Thread4: JAB

Thread1: CDE

Thread2: FGH

Thread3: IJA

Thread4: BCD

-- 
Regards,
Pankaj



Your program output should strictly adhere to the above output apart from
that it should perform well on following grounds -

1. Good synchronization technique (starvation free)
2. Weightage given to C++11 features


*On The Fly Sort*

In an alternate reality there is a company called Itanimulli which trades
in N exchanges around the world. Each exchange maintains a file with the
integers that represent the name of the stocks they buy/sell. This file is
in sorted order.

Itanimulli opens up an office @ Bangalore and wants to maintain a master
file with the list of all integers which represent the names of the stocks
in all N exchanges in sorted order. So they order exchanges to send them
the contents of their file.

The exchange sends them each integer in the file as a 12 bytes TCP packet
in the following format -

  \<Exchange Number\>\<Integer\>

4 bytes for \<Exchange Number\> and 8 bytes for \<Integer\>

These packets from any of the N exchanges can come simultaneously in random
order.

Assume you are working at the Bangalore office of the company, write a
program to receive the packets from the N Exchanges and simultaneously real
time create a sorted list of all the Integers.

If the any of the Exchange sends 0 as the integer value it denotes end of
transmission from that particular exchange. The program should print on
stdout the sorted list of integers and exit gracefully after receiving 0
integer value from all the connected Exchanges.

Points to note.


1) Weightage is given both to the network programming part and the
Algorithm part.

2) Usage of C++ STL datastructures or algorithms is NOT permitted.

3) The running time of the program will be an important criterion for
evaluation.

4) Your program should accept the port number to listen for connections
from N exchanges as a command line argument.

5) Let's assume that some other utility uses the data written to stdout by
the server on the fly, so we don't want to wait until we receive 0 from all
the exchanges before writing anything to stdout. We want to do it asap.


*Version Queue*

Design and implement a Version-Queue. A Version-Queue maintains a version
number along with normal Queue functionality. Every
operation[Enqueue/Dequeue] on the Queue increments its version.

Implement the following functions:

1. Enqueue - appends an element at the end of the queue.

2. Dequeue - returns the top element of the queue.

3. Print - it takes a version number as input and prints the elements of
the queue of the given version. The version number input can also be an
old/historical version number.

E.g. if the current version number of the queue is 7 and the input to this
function is 5, then it should print the elements of the queue when its
version number was 5.

For simplicity, assume the elements are integers.

We expect you to write a helper program to test the above data structure
which will read input from stdin and prints output to stdout.

Input format:
First line should have an integer n (number of operations). This should be
followed by n number of lines, each denoting one operation.
e.g.
6
e 1
e 4
d
e 5
p 2
p 4

'e' stands for enqueue



File Reduce

Consider a text file that has millions of records and has the following
properties:

1. Each line has one record on it.

2. Each record has comma separated values in the following format :-

     type,symbol,price,quantity,expirydate,strikeprice,amendtime,id,parentid

3. A record in the file can be uniquely identified by the "id".

4. The type field can have two values T and P, where T represents the
parent and P represents the child respectively.

5. A record R1 is the child of another record R2, if the type of R1 is P
and the parentid of R1 equals the id of R2.

A sample file:

T,ICICIBANK,1000,100,20121210,120,20121209103030,1234,0

T,AXISBANK,1000,100,20121210,120,20121209103031,1235,0

T,SBIBANK,1000,100,20121210,120,20121209103032,1236,0

P,ICICIBANK,1100,100,20121210,120,20121209103030,1237,1234

P,AXISBANK,1000,100,20121210,120,20121209103031,1238,1235

T,ICICIBANK,1000,100,20121210,120,20121209103035,1239,0

T,.CITIBANK,1000,101,20121210,120,20121209103036,1240,0

P,ICICIBANK,1100,100,20121210,120,20121209103030,1241,1234

P,ICICIBANK,1100,100,20121210,120,20121209103035,1242,1239

Write an optimal C++ program that takes a file and an integer X as command
line arguments and splits the input file into maximum number of smaller
files such that each smaller file contains a minimum of X number of records
with the condition that all the children of a parent record should be in
the same file as the parent record. Each smaller file should be named as
output_<n>.txt where value n is  1, 2, 3 ... N (maximum number of files
possibles)

For e.g. if the sample file above is split into files containing minimum
two rows each, then records -

T,ICICIBANK,1000,100,20121210,120,20121209103030,1234,0

P,ICICIBANK,1100,100,20121210,120,20121209103030,1237,1234

P,ICICIBANK,1100,100,20121210,120,20121209103030,1241,1234
should be in one file.
