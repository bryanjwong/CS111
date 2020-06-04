# NAME: Bryan Wong
# EMAIL: bryanjwong@g.ucla.edu
# ID: 805111517

default:
	@gcc -Wall -Wextra -lmraa -lm lab4c_tcp.c -o lab4c_tcp

.PHONY: clean check dist

clean:
	@-rm lab4c lab4c-805111517.tar.gz 2>/dev/null || true

check:
	bash tests.sh

dist:
	tar -czvf lab4c-805111517.tar.gz lab4c.c tests.sh Makefile README
