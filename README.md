# EC504-Project

Run the make file to generate executable called trees.

This executable can do two things:

1. test/demo insertion and successor opertaions on an x-fast and y-fast tries. 
2. Compare run time between x-fast and y-fast tries and BSTs.

## Insertion and Successor Demo

Included in this repo are two text files. 

numbers.txt includes a list of numbers to insert into the tries.

successor.txt includes a list of numbers we want to call sucessor operation on after the insetions.

execute:

```
./trees demo numbers.txt successor.txt
```

This will produce the following output:

```
demo
Inserting following numbers: [1, 4, 5, 12, 23, 32, 32, 43, 52, 53, 54, 86, 123, 235, 523, 532, ]
 -----------------------------------------------
Resulting Y-fast trie:
content of X-fast: [18446744073709551615, ]
 BST under 18446744073709551615: [ 1, 4, 5, 12, 23, 32, 43, 52, 53, 54, 86, 123, 235, 523, 532, ]
 -----------------------------------------------
successor of 1 is 1
successor of 21 is 23
successor of 24 is 32
successor of 42 is 43
successor of 64 is 86
 -----------------------------------------------
Resulting X-fast trie:
 -----------------------------------------------
level: 0 []
level: 1 [0, ]
level: 2 [0, ]
level: 3 [0, ]
level: 4 [0, ]
level: 5 [0, ]
level: 6 [0, ]
level: 7 [0, ]
level: 8 [0, ]
level: 9 [0, ]
level: 10 [0, ]
level: 11 [0, ]
level: 12 [0, ]
level: 13 [0, ]
level: 14 [0, ]
level: 15 [0, ]
level: 16 [0, ]
level: 17 [0, ]
level: 18 [0, ]
level: 19 [0, ]
level: 20 [0, ]
level: 21 [0, ]
level: 22 [0, ]
level: 23 [0, ]
level: 24 [0, ]
level: 25 [0, ]
level: 26 [0, ]
level: 27 [0, ]
level: 28 [0, ]
level: 29 [0, ]
level: 30 [0, ]
level: 31 [0, ]
level: 32 [0, ]
level: 33 [0, ]
level: 34 [0, ]
level: 35 [0, ]
level: 36 [0, ]
level: 37 [0, ]
level: 38 [0, ]
level: 39 [0, ]
level: 40 [0, ]
level: 41 [0, ]
level: 42 [0, ]
level: 43 [0, ]
level: 44 [0, ]
level: 45 [0, ]
level: 46 [0, ]
level: 47 [0, ]
level: 48 [0, ]
level: 49 [0, ]
level: 50 [0, ]
level: 51 [0, ]
level: 52 [0, ]
level: 53 [0, ]
level: 54 [0, ]
level: 55 [0, 1, ]
level: 56 [0, 2, ]
level: 57 [0, 1, 4, ]
level: 58 [8, 0, 1, 3, ]
level: 59 [16, 0, 1, 2, 3, 7, ]
level: 60 [32, 0, 33, 1, 2, 3, 5, 7, 14, ]
level: 61 [0, 65, 1, 10, 66, 2, 4, 29, 5, 6, 15, ]
level: 62 [8, 0, 1, 3, 21, 13, 133, 5, 10, 30, 58, 130, ]
level: 63 [16, 0, 26, 2, 6, 43, 27, 11, 21, 61, 117, 261, 266, ]
level: 64 [1, 52, 12, 4, 53, 5, 23, 32, 235, 43, 54, 86, 123, 523, 532, ]
 -----------------------------------------------
 -----------------------------------------------
successor of 1 is 1
successor of 21 is 23
successor of 24 is 32
successor of 42 is 43
successor of 64 is 86
 -----------------------------------------------
results.txt
Displaying results.txt.
```

For the Y-fast Trie, there is only one BST and one value in the X-Fast Trie because each BST can hold up to 128 numbers. You would have to insert more than 128 numbers into the Y-fast Trie to see multiple BSTs.

*Important*: Y-fast Tries only store data in the BSTs. The stuff in the top-level X-fast trie is auxiliary information.

For the X-Fast Trie, note that there are 64 levels because we use 64 bit integers. All the top levels are empty because we use small integers. Small integers have a lot of zeros in front. 

*Important*: X-Fast Tries only staor data in the bottom level (level 64). The other levels store auxiliary information.

