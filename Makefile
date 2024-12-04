.PHONY: all clean

all:
	for n in day*; do (cd $$n; make test); done

clean:
	for n in day*; do (cd $$n; make distclean); done
