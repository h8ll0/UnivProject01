# University-Project-1
My first univetsity project. Work with .txt, strings.\
Commands: \
arow - Adds a new table row to the end of the table\
icol - Inserts an empty column before the column given by the number C\
acol - Adds an empty column after the last column\
dcol - Deletes the column given by the number C\
cset - The string STR will be set to the cell in column C\
tolower - The string in column C will be converted to lowercase
toupper - The string in column C will be converted to uppercase
round - In column C, rounds the number to an integer
int - Removes the decimal part of the number in column C
copy - Copies the values of the cells in columns N and M
swap - Swaps the values of the cells in columns N and M
move - Moves column N before column M
beginwith - Process only those rows whose cell contents in column C begin with a string STR
contains - Process only those rows whose cells in column C contain the string STR
csum - Sums all cells between N and M and assigns it to C
cavg - Finds the average value between N and M and assigns it to C
cmin - Finds the minimal value between N and M and assigns it to C
cmax - Finds the maximal value between N and M and assigns it to C

Example input.txt:

A1:B1:C1 \
a2:b2:C2 \
A3:B3:c3 \

Commands: ./sheet -d ":" icol 1

Example output:

:A1:B1:C1
:a2:b2:C2
:A3:B3:c3

Commands: ./sheet -d ":" icol 1 cset 2 hello

Example output:

:hello:B1:C1
:hello:b2:C2
:hello:B3:c3

Commands: ./sheet -d ":" icol 1 cset 2 "hello" dcol 3

Example output:

:hello:B1
:hello:b2
:hello:B3
