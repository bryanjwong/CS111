# NAME: Bryan Wong
# EMAIL: bryanjwong@g.ucla.edu
# ID: 805111517

default:
	@gcc -Wall -Wextra lab3a.c -o lab3a

.PHONY: clean dist

clean:
	@-rm lab3a lab3a-805111517.tar.gz 2>/dev/null || true

dist:
	tar -czvf lab3a-805111517.tar.gz Makefile README lab3a.c ext2_fs.h
