# NAME: Bryan Wong
# EMAIL: bryanjwong@g.ucla.edu
# ID: 805111517

default:
	@gcc -Wall -Wextra -pthread lab2_add.c -o lab2_add
	@gcc -Wall -Wextra -pthread SortedList.c lab2_list.c -o lab2_list

.PHONY: clean tests graphs dist

clean:
	@-rm lab2_add lab2_list lab2a-805111517.tar.gz 2>/dev/null || true

tests: default
	rm -f lab2_add.csv lab2_list.csv

	./lab2_add --threads=1 --iterations=10 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=20 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=40 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=80 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=100000 >> lab2_add.csv

	./lab2_add --threads=2 --iterations=10 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=20 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=40 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=80 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100000 >> lab2_add.csv

	./lab2_add --threads=4 --iterations=10 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=20 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=40 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=80 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100000 >> lab2_add.csv

	./lab2_add --threads=8 --iterations=10 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=20 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=40 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=80 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100000 >> lab2_add.csv

	./lab2_add --threads=12 --iterations=10 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=20 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=40 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=80 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100000 >> lab2_add.csv

	./lab2_add --threads=1 --iterations=10 --yield >> lab2_add.csv
	./lab2_add --threads=1 --iterations=20 --yield >> lab2_add.csv
	./lab2_add --threads=1 --iterations=40 --yield >> lab2_add.csv
	./lab2_add --threads=1 --iterations=80 --yield >> lab2_add.csv
	./lab2_add --threads=1 --iterations=100 --yield >> lab2_add.csv
	./lab2_add --threads=1 --iterations=1000 --yield >> lab2_add.csv
	./lab2_add --threads=1 --iterations=10000 --yield >> lab2_add.csv
	./lab2_add --threads=1 --iterations=100000 --yield >> lab2_add.csv

	./lab2_add --threads=2 --iterations=10 --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=20 --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=40 --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=80 --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100 --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=1000 --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100000 --yield >> lab2_add.csv

	./lab2_add --threads=4 --iterations=10 --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=20 --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=40 --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=80 --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100 --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=1000 --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100000 --yield >> lab2_add.csv

	./lab2_add --threads=8 --iterations=10 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=20 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=40 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=80 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=1000 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100000 --yield >> lab2_add.csv

	./lab2_add --threads=12 --iterations=10 --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=20 --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=40 --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=80 --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100 --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=1000 --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100000 --yield >> lab2_add.csv

	./lab2_add --threads=1 --iterations=10 --sync=m >> lab2_add.csv
	./lab2_add --threads=1 --iterations=20 --sync=m >> lab2_add.csv
	./lab2_add --threads=1 --iterations=40 --sync=m >> lab2_add.csv
	./lab2_add --threads=1 --iterations=80 --sync=m >> lab2_add.csv
	./lab2_add --threads=1 --iterations=100 --sync=m >> lab2_add.csv
	./lab2_add --threads=1 --iterations=1000 --sync=m >> lab2_add.csv
	./lab2_add --threads=1 --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --threads=1 --iterations=100000 --sync=m >> lab2_add.csv

	./lab2_add --threads=2 --iterations=10 --sync=m >> lab2_add.csv
	./lab2_add --threads=2 --iterations=20 --sync=m >> lab2_add.csv
	./lab2_add --threads=2 --iterations=40 --sync=m >> lab2_add.csv
	./lab2_add --threads=2 --iterations=80 --sync=m >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100 --sync=m >> lab2_add.csv
	./lab2_add --threads=2 --iterations=1000 --sync=m >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100000 --sync=m >> lab2_add.csv

	./lab2_add --threads=4 --iterations=10 --sync=m >> lab2_add.csv
	./lab2_add --threads=4 --iterations=20 --sync=m >> lab2_add.csv
	./lab2_add --threads=4 --iterations=40 --sync=m >> lab2_add.csv
	./lab2_add --threads=4 --iterations=80 --sync=m >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100 --sync=m >> lab2_add.csv
	./lab2_add --threads=4 --iterations=1000 --sync=m >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100000 --sync=m >> lab2_add.csv

	./lab2_add --threads=8 --iterations=10 --sync=m >> lab2_add.csv
	./lab2_add --threads=8 --iterations=20 --sync=m >> lab2_add.csv
	./lab2_add --threads=8 --iterations=40 --sync=m >> lab2_add.csv
	./lab2_add --threads=8 --iterations=80 --sync=m >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100 --sync=m >> lab2_add.csv
	./lab2_add --threads=8 --iterations=1000 --sync=m >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100000 --sync=m >> lab2_add.csv

	./lab2_add --threads=12 --iterations=10 --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=20 --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=40 --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=80 --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100 --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=1000 --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100000 --sync=m >> lab2_add.csv

	./lab2_add --threads=2 --iterations=10 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=20 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=40 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=80 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=1000 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100000 --sync=m --yield >> lab2_add.csv

	./lab2_add --threads=4 --iterations=10 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=20 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=40 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=80 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=1000 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100000 --sync=m --yield >> lab2_add.csv

	./lab2_add --threads=8 --iterations=10 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=20 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=40 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=80 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=1000 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100000 --sync=m --yield >> lab2_add.csv

	./lab2_add --threads=12 --iterations=10 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=20 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=40 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=80 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=1000 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 --sync=m --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100000 --sync=m --yield >> lab2_add.csv

	./lab2_add --threads=1 --iterations=10 --sync=s >> lab2_add.csv
	./lab2_add --threads=1 --iterations=20 --sync=s >> lab2_add.csv
	./lab2_add --threads=1 --iterations=40 --sync=s >> lab2_add.csv
	./lab2_add --threads=1 --iterations=80 --sync=s >> lab2_add.csv
	./lab2_add --threads=1 --iterations=100 --sync=s >> lab2_add.csv
	./lab2_add --threads=1 --iterations=1000 --sync=s >> lab2_add.csv
	./lab2_add --threads=1 --iterations=10000 --sync=s >> lab2_add.csv
	./lab2_add --threads=1 --iterations=100000 --sync=s >> lab2_add.csv

	./lab2_add --threads=2 --iterations=10 --sync=s >> lab2_add.csv
	./lab2_add --threads=2 --iterations=20 --sync=s >> lab2_add.csv
	./lab2_add --threads=2 --iterations=40 --sync=s >> lab2_add.csv
	./lab2_add --threads=2 --iterations=80 --sync=s >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100 --sync=s >> lab2_add.csv
	./lab2_add --threads=2 --iterations=1000 --sync=s >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 --sync=s >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100000 --sync=s >> lab2_add.csv

	./lab2_add --threads=4 --iterations=10 --sync=s >> lab2_add.csv
	./lab2_add --threads=4 --iterations=20 --sync=s >> lab2_add.csv
	./lab2_add --threads=4 --iterations=40 --sync=s >> lab2_add.csv
	./lab2_add --threads=4 --iterations=80 --sync=s >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100 --sync=s >> lab2_add.csv
	./lab2_add --threads=4 --iterations=1000 --sync=s >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 --sync=s >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100000 --sync=s >> lab2_add.csv

	./lab2_add --threads=8 --iterations=10 --sync=s >> lab2_add.csv
	./lab2_add --threads=8 --iterations=20 --sync=s >> lab2_add.csv
	./lab2_add --threads=8 --iterations=40 --sync=s >> lab2_add.csv
	./lab2_add --threads=8 --iterations=80 --sync=s >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100 --sync=s >> lab2_add.csv
	./lab2_add --threads=8 --iterations=1000 --sync=s >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 --sync=s >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100000 --sync=s >> lab2_add.csv

	./lab2_add --threads=12 --iterations=10 --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=20 --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=40 --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=80 --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100 --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=1000 --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100000 --sync=s >> lab2_add.csv

	./lab2_add --threads=2 --iterations=10 --sync=s --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=20 --sync=s --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=40 --sync=s --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=80 --sync=s --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100 --sync=s --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=1000 --sync=s --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 --sync=s --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100000 --sync=s --yield >> lab2_add.csv

	./lab2_add --threads=4 --iterations=10 --sync=s --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=20 --sync=s --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=40 --sync=s --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=80 --sync=s --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100 --sync=s --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=1000 --sync=s --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 --sync=s --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100000 --sync=s --yield >> lab2_add.csv

	./lab2_add --threads=8 --iterations=10 --sync=s --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=20 --sync=s --yield >> lab2_add.csv

	./lab2_add --threads=12 --iterations=10 --sync=s --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=20 --sync=s --yield >> lab2_add.csv

	./lab2_add --threads=1 --iterations=10 --sync=c >> lab2_add.csv
	./lab2_add --threads=1 --iterations=20 --sync=c >> lab2_add.csv
	./lab2_add --threads=1 --iterations=40 --sync=c >> lab2_add.csv
	./lab2_add --threads=1 --iterations=80 --sync=c >> lab2_add.csv
	./lab2_add --threads=1 --iterations=100 --sync=c >> lab2_add.csv
	./lab2_add --threads=1 --iterations=1000 --sync=c >> lab2_add.csv
	./lab2_add --threads=1 --iterations=10000 --sync=c >> lab2_add.csv
	./lab2_add --threads=1 --iterations=100000 --sync=c >> lab2_add.csv

	./lab2_add --threads=2 --iterations=10 --sync=c >> lab2_add.csv
	./lab2_add --threads=2 --iterations=20 --sync=c >> lab2_add.csv
	./lab2_add --threads=2 --iterations=40 --sync=c >> lab2_add.csv
	./lab2_add --threads=2 --iterations=80 --sync=c >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100 --sync=c >> lab2_add.csv
	./lab2_add --threads=2 --iterations=1000 --sync=c >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 --sync=c >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100000 --sync=c >> lab2_add.csv

	./lab2_add --threads=4 --iterations=10 --sync=c >> lab2_add.csv
	./lab2_add --threads=4 --iterations=20 --sync=c >> lab2_add.csv
	./lab2_add --threads=4 --iterations=40 --sync=c >> lab2_add.csv
	./lab2_add --threads=4 --iterations=80 --sync=c >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100 --sync=c >> lab2_add.csv
	./lab2_add --threads=4 --iterations=1000 --sync=c >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 --sync=c >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100000 --sync=c >> lab2_add.csv

	./lab2_add --threads=8 --iterations=10 --sync=c >> lab2_add.csv
	./lab2_add --threads=8 --iterations=20 --sync=c >> lab2_add.csv
	./lab2_add --threads=8 --iterations=40 --sync=c >> lab2_add.csv
	./lab2_add --threads=8 --iterations=80 --sync=c >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100 --sync=c >> lab2_add.csv
	./lab2_add --threads=8 --iterations=1000 --sync=c >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 --sync=c >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100000 --sync=c >> lab2_add.csv

	./lab2_add --threads=12 --iterations=10 --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=20 --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=40 --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=80 --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100 --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=1000 --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100000 --sync=c >> lab2_add.csv

	./lab2_add --threads=2 --iterations=10 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=20 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=40 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=80 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=1000 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100000 --sync=c --yield >> lab2_add.csv

	./lab2_add --threads=4 --iterations=10 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=20 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=40 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=80 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=1000 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100000 --sync=c --yield >> lab2_add.csv

	./lab2_add --threads=8 --iterations=10 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=20 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=40 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=80 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=1000 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100000 --sync=c --yield >> lab2_add.csv

	./lab2_add --threads=12 --iterations=10 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=20 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=40 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=80 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=1000 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 --sync=c --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100000 --sync=c --yield >> lab2_add.csv

	./lab2_list --threads=1 --iterations=10 >> lab2_list.csv
	./lab2_list --threads=1 --iterations=100 >> lab2_list.csv
	./lab2_list --threads=1 --iterations=1000 >> lab2_list.csv
	./lab2_list --threads=1 --iterations=10000 >> lab2_list.csv
	./lab2_list --threads=1 --iterations=20000 >> lab2_list.csv

	-./lab2_list --threads=2 --iterations=1 >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=2 >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=4 >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=8 >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=16 >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=32 >> lab2_list.csv

	-./lab2_list --threads=4 --iterations=1 >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=2 >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=4 >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=8 >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=16 >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=32 >> lab2_list.csv

	-./lab2_list --threads=8 --iterations=1 >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=2 >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=4 >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=8 >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=16 >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=32 >> lab2_list.csv

	-./lab2_list --threads=12 --iterations=1 >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2 >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=4 >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=8 >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=16 >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 >> lab2_list.csv

	-./lab2_list --threads=2 --iterations=1 --yield=i >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=2 --yield=i >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=4 --yield=i >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=8 --yield=i >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=16 --yield=i >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=32 --yield=i >> lab2_list.csv

	-./lab2_list --threads=4 --iterations=1 --yield=i >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=2 --yield=i >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=4 --yield=i >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=8 --yield=i >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=16 --yield=i >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=32 --yield=i >> lab2_list.csv

	-./lab2_list --threads=8 --iterations=1 --yield=i >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=2 --yield=i >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=4 --yield=i >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=8 --yield=i >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=16 --yield=i >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=32 --yield=i >> lab2_list.csv

	-./lab2_list --threads=12 --iterations=1 --yield=i >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2 --yield=i >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=4 --yield=i >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=8 --yield=i >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=16 --yield=i >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=i >> lab2_list.csv

	-./lab2_list --threads=2 --iterations=1 --yield=d >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=2 --yield=d >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=4 --yield=d >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=8 --yield=d >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=16 --yield=d >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=32 --yield=d >> lab2_list.csv

	-./lab2_list --threads=4 --iterations=1 --yield=d >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=2 --yield=d >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=4 --yield=d >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=8 --yield=d >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=16 --yield=d >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=32 --yield=d >> lab2_list.csv

	-./lab2_list --threads=8 --iterations=1 --yield=d >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=2 --yield=d >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=4 --yield=d >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=8 --yield=d >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=16 --yield=d >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=32 --yield=d >> lab2_list.csv

	-./lab2_list --threads=12 --iterations=1 --yield=d >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2 --yield=d >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=4 --yield=d >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=8 --yield=d >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=16 --yield=d >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=d >> lab2_list.csv

	-./lab2_list --threads=2 --iterations=1 --yield=il >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=2 --yield=il >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=4 --yield=il >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=8 --yield=il >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=16 --yield=il >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=32 --yield=il >> lab2_list.csv

	-./lab2_list --threads=4 --iterations=1 --yield=il >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=2 --yield=il >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=4 --yield=il >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=8 --yield=il >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=16 --yield=il >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=32 --yield=il >> lab2_list.csv

	-./lab2_list --threads=8 --iterations=1 --yield=il >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=2 --yield=il >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=4 --yield=il >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=8 --yield=il >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=16 --yield=il >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=32 --yield=il >> lab2_list.csv

	-./lab2_list --threads=12 --iterations=1 --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2 --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=4 --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=8 --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=16 --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=il >> lab2_list.csv

	-./lab2_list --threads=2 --iterations=1 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=2 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=4 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=8 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=16 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=32 --yield=dl >> lab2_list.csv

	-./lab2_list --threads=4 --iterations=1 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=2 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=4 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=8 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=16 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=32 --yield=dl >> lab2_list.csv

	-./lab2_list --threads=8 --iterations=1 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=2 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=4 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=8 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=16 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=32 --yield=dl >> lab2_list.csv

	-./lab2_list --threads=12 --iterations=1 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=4 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=8 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=16 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=dl >> lab2_list.csv

	./lab2_list --threads=12 --iterations=2 --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --sync=m >> lab2_list.csv

	./lab2_list --threads=12 --iterations=2 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=i --sync=m >> lab2_list.csv

	./lab2_list --threads=12 --iterations=2 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=d --sync=m >> lab2_list.csv

	./lab2_list --threads=12 --iterations=2 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=il --sync=m >> lab2_list.csv

	./lab2_list --threads=12 --iterations=2 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=dl --sync=m >> lab2_list.csv

	./lab2_list --threads=12 --iterations=2 --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --sync=s >> lab2_list.csv

	./lab2_list --threads=12 --iterations=2 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=i --sync=s >> lab2_list.csv

	./lab2_list --threads=12 --iterations=2 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=d --sync=s >> lab2_list.csv

	./lab2_list --threads=12 --iterations=2 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=il --sync=s >> lab2_list.csv

	./lab2_list --threads=12 --iterations=2 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=dl --sync=s >> lab2_list.csv

	./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2_list.csv

	./lab2_list --threads=1 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2_list.csv

graphs: tests
	./lab2_add.gp
	./lab2_list.gp

dist: graphs
	tar -czvf lab2a-805111517.tar.gz Makefile README lab2_add.c lab2_list.c \
	SortedList.h SortedList.c lab2_add.gp lab2_list.gp lab2_add.csv \
	lab2_list.csv lab2_add-1.png lab2_add-2.png lab2_add-3.png lab2_add-4.png \
	lab2_add-5.png lab2_list-1.png lab2_list-2.png lab2_list-3.png lab2_list-4.png
