# NAME: Bryan Wong
# EMAIL: bryanjwong@g.ucla.edu
# ID: 805111517

default:
	@gcc -Wall -Wextra -pthread lab2_add.c -o lab2_add

.PHONY: clean dist

clean:
	@-rm lab2_add 2>/dev/null || true

dist:
	tar -czvf lab2a-805111517.tar.gz Makefile README lab2_add.c 
