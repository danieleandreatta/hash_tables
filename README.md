# hash_tables
Simple proof of concept implementations of different hash tables in C

I tried to implement most of the different types of hash tables I found on
Wikipedia: https://en.wikipedia.org/wiki/Hash_table

- chaining
- linear
- quadratic
- double hash
- 2choice
- coalesced
- cuckoo
- hopscotch


## performace
This simple program does a few simple workloads to measure the relative 
performace of the different implementations. All times are in ms

**add**: add all the words in **words** (about 90k) to the table

**get**: access all the data in file **words** and validate it is contained
in the table

**del**: delete all the words

**re-add**: re-add all the words in the file to the emptied table

**random**: randomly add/del 300k words (same seed for reproducibility)

## test/

directory containing programs for unit testing
