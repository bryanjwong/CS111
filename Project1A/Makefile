# NAME: Bryan Wong
# EMAIL: bryanjwong@g.ucla.edu
# ID: 805111517

default:
	@gcc -Wall -Wextra lab1a.c -o lab1a

.PHONY: clean dist

clean:
	@-rm lab1a lab1a-805111517.tar.gz 2>/dev/null || true

dist:
	tar -czvf lab1a-805111517.tar.gz Makefile README lab1a.c
