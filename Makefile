MAKEFLAGS += --silent

.PHONY: all clean

all:
	for n in day[012][0-9]; do (cd $$n; make test); done

clean:
	for n in day[012][0-9]; do (cd $$n; make distclean); done
