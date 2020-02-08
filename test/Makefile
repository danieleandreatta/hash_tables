all: test_choice2 test_hopscotch test_cuckoo test_coalesced test_double test_quadratic test_linear test_chain

test_choice2: test_choice2.c ../choice2.c ../strhash.c ../key_list.c
	gcc -o test_choice2 -DDEBUG -Wall test_choice2.c ../choice2.c ../strhash.c ../key_list.c

test_hopscotch: test_hopscotch.c ../hopscotch.c ../strhash.c ../key_list.c
	gcc -o test_hopscotch -DDEBUG -Wall test_hopscotch.c ../hopscotch.c ../strhash.c ../key_list.c

test_cuckoo: test_cuckoo.c ../cuckoo.c ../strhash.c ../key_list.c
	gcc -o test_cuckoo -DDEBUG -Wall test_cuckoo.c ../cuckoo.c ../strhash.c ../key_list.c

test_coalesced: test_coalesced.c ../coalesced.c ../strhash.c ../key_list.c
	gcc -o test_coalesced -DDEBUG -Wall test_coalesced.c ../coalesced.c ../strhash.c ../key_list.c

test_double: test_double.c ../double.c ../strhash.c ../key_list.c
	gcc -o test_double -DDEBUG -Wall test_double.c ../double.c ../strhash.c ../key_list.c

test_quadratic: test_quadratic.c ../quadratic.c ../strhash.c ../key_list.c
	gcc -o test_quadratic -DDEBUG -Wall test_quadratic.c ../quadratic.c ../strhash.c ../key_list.c

test_linear: test_linear.c ../linear.c ../strhash.c ../key_list.c
	gcc -o test_linear -DDEBUG -Wall test_linear.c ../linear.c ../strhash.c ../key_list.c

test_chain: test_chain.c ../chain.c ../strhash.c ../key_list.c
	gcc -o test_chain -DDEBUG -Wall test_chain.c ../chain.c ../strhash.c ../key_list.c
