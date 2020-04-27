# NAME: Bryan Wong
# EMAIL: bryanjwong@g.ucla.edu
# ID: 805111517

default:
	@gcc -Wall -Wextra helloworld.c -o helloworld

.PHONY: clean dist

clean:
	@-rm helloworld lab4a-805111517.tar.gz 2>/dev/null || true

dist:
	tar -czvf lab4a-805111517.tar.gz my_beaglebone.jpg connected.png copy.png build_run.png git.png Makefile README
