TESTS=$(sort $(wildcard *.fun))
PROGS=$(subst .fun,,$(TESTS))
OUTS=$(patsubst %.fun,%.out,$(TESTS))
DIFFS=$(patsubst %.fun,%.diff,$(TESTS))
RESULTS=$(patsubst %.fun,%.result,$(TESTS))

.SECONDARY:

.PROCIOUS : %.o %.S %.out

CFLAGS=-g -std=gnu99 -O0 -Werror -Wall

pd : pd.o parser.o Makefile
	gcc $(CFLAGS) -o pd pd.o parser.o

%.o : %.c Makefile
	gcc $(CFLAGS) -MD -c $*.c

%.o : %.S Makefile
	gcc -MD -c $*.S

%.S : %.fun pd
	@echo "========== $* =========="
	./pd < $*.fun > $*.S

progs : $(PROGS)

$(PROGS) : % : %.o
	gcc -o $@ $*.o

outs : $(OUTS)

$(OUTS) : %.out : %
	./$* > $*.out

diffs : $(DIFFS)

$(DIFFS) : %.diff : Makefile %.out %.ok
	@(((diff -b $*.ok $*.out > /dev/null 2>&1) && (echo "===> $* ... pass")) || (echo "===> $* ... fail" ; echo "----- expected ------"; cat $*.ok ; echo "----- found -----"; cat $*.out)) > $*.diff 2>&1

$(RESULTS) : %.result : Makefile %.diff
	@cat $*.diff

test : Makefile $(DIFFS)
	@cat $(DIFFS)

clean :
	rm -f $(PROGS)
	rm -f *.S
	rm -f *.out
	rm -f *.d
	rm -f *.o
	rm -f pd
	rm -f *.diff

-include *.d
