# NAME: Bryan Wong
# EMAIL: bryanjwong@g.ucla.edu
# ID: 805111517

default:
	@gcc -Wall -Wextra -lmraa lab4b.c -o lab4b

.PHONY: clean dist

clean:
	@-rm lab4b lab4b-805111517.tar.gz 2>/dev/null || true

dist:
	tar -czvf lab4b-805111517.tar.gz my_beaglebone.jpg connected.png copy.png build_run.png git.png Makefile README
