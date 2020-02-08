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

For reference, these are the results with my (old) laptop:

```
    type \ operation |    add |    get |    del | re-add | random
            chaining |    118 |     50 |     33 |     67 |    269
             2choice |    184 |     79 |     47 |    117 |    382
              linear |     73 |     24 |     23 |     42 |    186
           quadratic |     77 |     29 |     27 |     49 |    199
         double hash |     88 |     33 |     32 |     55 |    210
           coalesced |    101 |     26 |     27 |     33 |    167
              cuckoo |    114 |     27 |     28 |     53 |    195
           hopscotch |    158 |     29 |     30 |     86 |    244
```
## test/

directory containing programs for unit testing
