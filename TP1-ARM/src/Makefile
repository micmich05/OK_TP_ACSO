sim: shell.c sim.c 
	gcc -g -O0 $^ -o $@

.PHONY: clean
clean:
	rm -rf *.o *~ sim

test:
	gcc sim.c -o sim
	./sim 

