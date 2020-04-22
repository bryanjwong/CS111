# NAME: Bryan Wong
# EMAIL: bryanjwong@g.ucla.edu
# ID: 805111517

default:
	@gcc -Wall -Wextra -lz lab1b-server.c -o lab1b-server
	@gcc -Wall -Wextra -lz lab1b-client.c -o lab1b-client

.PHONY: clean dist

clean:
	@-rm server client lab1b-805111517.tar.gz 2>/dev/null || true

dist:
	tar -czvf lab1b-805111517.tar.gz Makefile README lab1b-server.c lab1b-client.c
