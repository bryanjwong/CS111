# NAME: Bryan Wong
# EMAIL: bryanjwong@g.ucla.edu
# ID: 805111517

GPERF=/w/home.06/ee/ugrad/bryan/CS111/Project2B/gperf

default:
	@gcc -Wall -Wextra -lpthread -g -lprofiler SortedList.c lab2_list.c -o lab2_list

.PHONY: clean tests graphs dist

clean:
	@-rm lab2_list 2>/dev/null || true

tests: default
	rm -f lab2b_list.csv

	./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2b_list.csv

	./lab2_list --threads=1 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2b_list.csv

graphs: tests
	./lab2b_list.gp

profile: default
	rm -f ./raw.gperf profile.out
	LD_PRELOAD=($GPERF)/lib/libprofiler.so.0
	CPUPROFILE=./raw.gperf ./lab2_list --iterations=1000 --threads=12 --sync=s
	$(GPERF)/bin/pprof --text ./lab2_list ./raw.gperf > profile.out
	rm -f ./raw.gperf

dist: graphs
