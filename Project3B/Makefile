# NAME: Bryan Wong
# EMAIL: bryanjwong@g.ucla.edu
# ID: 805111517

default:
	@rm -f lab3b
	@ln lab3b-script lab3b
	@chmod u+x lab3b

.PHONY: clean dist

clean:
	@-rm lab3b lab3b-805111517.tar.gz 2>/dev/null || true

dist: default
	tar -czvf lab3b-805111517.tar.gz Makefile README lab3b.py lab3b-script
