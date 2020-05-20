# NAME: Bryan Wong
# EMAIL: bryanjwong@g.ucla.edu
# ID: 805111517

default:
	@gcc -Wall -Wextra -lmraa -lm lab4b.c -o lab4b

.PHONY: clean check dist

clean:
	@-rm lab4b lab4b-805111517.tar.gz 2>/dev/null || true

check: 
	bash tests.sh
	
dist:
	tar -czvf lab4b-805111517.tar.gz lab4b.c tests.sh Makefile README
