# NAME: Bryan Wong
# EMAIL: bryanjwong@g.ucla.edu
# ID: 805111517

default:
	@gcc -Wall -Wextra -lmraa -lm lab4c_tcp.c -o lab4c_tcp
	@gcc -Wall -Wextra -lmraa -lm -lssl -lcrypto lab4c_tls.c -o lab4c_tls

.PHONY: clean check dist

clean:
	@-rm lab4c_tcp lab4c_tls lab4c-805111517.tar.gz 2>/dev/null || true

dist:
	tar -czvf lab4c-805111517.tar.gz lab4c_tcp.c lab4c_tls.c Makefile README
