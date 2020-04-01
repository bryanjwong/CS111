# NAME: Bryan Wong
# EMAIL: bryanjwong@g.ucla.edu
# ID: 805111517

default:
	@gcc -Wall -Wextra -g lab0.c -o lab0

.PHONY: clean check dist

clean:
	@-rm lab0 lab0-805111517.tar.gz 2>/dev/null || true

check:
	@echo "test input" > inputfile
	@echo "test input" > correct
	@./lab0 < inputfile > outputfile1
	@diff outputfile1 correct || echo "error: incorrect output when copying from stdin to stdout"
	@./lab0 --input=inputfile > outputfile2
	@diff outputfile2 correct || echo "error: incorrect output when copying from --input to stdout"
	@./lab0 --output=outputfile3 < inputfile
	@diff outputfile3 correct || echo "error: incorrect output when copying from stdin to --output"
	@./lab0 --input=inputfile --output=outputfile4
	@diff outputfile4 correct || echo "error: incorrect output when copying from --input to --output"
	@-./lab0 --blah 2> /dev/null || true
	@[ $$? -ne 1 ] || echo "error: expected exit code 1 but received exit code $$?"
	@-./lab0 --input= 2> /dev/null || true
	@[ $$? -ne 2 ] || echo "error: expected exit code 2 but received exit code $$?"
	@-./lab0 --output= 2> /dev/null || true
	@[ $$? -ne 3 ] || echo "error: expected exit code 3 but received exit code $$?"
	@-./lab0 --input= --output= 2> /dev/null || true
	@[ $$? -ne 2 ] || echo "error: expected exit code 2 but received exit code $$?"
	@-./lab0 --output= --input= 2> /dev/null || true
	@[ $$? -ne 3 ] || echo "error: expected exit code 3 but received exit code $$?"
	@-./lab0 --segfault --catch 2> /dev/null || true
	@[ $$? -ne 4 ] || echo "error: expected exit code 4 but received exit code $$?"
	@rm outputfile[1-4] inputfile correct

dist:
	tar -czvf lab0-805111517.tar.gz Makefile README lab0.c backtrace.png breakpoint.png
