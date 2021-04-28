all:
	$(CC) -o m_vector vector.c main.c -g -lm

self:
	$(CC) -o m_self self_vector.c -g -lm

test:
	./m_vector

test_self:
	./m_self

1000:
	number=1 ; while [ $$number -le 1000 ] ; do\
	       ./m_vecotr; echo $$number ;\
	       number=$$((number + 1)) ;\
	done

1000_self:
	number=1 ; while [ $$number -le 1000 ] ; do\
               ./m_self; echo $$number ;\
               number=$$((number + 1)) ;\
        done

massif:
	valgrind --tool=massif ./m_vector

massif_self:
	valgrind --tool=massif ./m_self


perf:
	sudo perf stat --repeat 10000 -e cache-misses,cache-references,instructions,cycles ./m_vector


perf_self:
	sudo perf stat --repeat 10000 -e cache-misses,cache-references,instructions,cycles ./m_self


address:
	gcc -o memory vector.c main.c -g -lm -fsanitize=address

clean_massif:
	rm -rf massif.out.*

clean:
	rm -rf m_vector m_self memory
