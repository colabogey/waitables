SUBDIRS = src

release:
	@for dir in $(SUBDIRS); do \
                $(MAKE) -C $$dir $(MAKECMDGOALS); \
        done

debug:
	@for dir in $(SUBDIRS); do \
                $(MAKE) -C $$dir $(MAKECMDGOALS); \
        done

clean:
	@for dir in $(SUBDIRS); do \
                $(MAKE) -C $$dir $(MAKECMDGOALS); \
        done

testclean:
	@for dir in $(SUBDIRS); do \
                $(MAKE) -C $$dir $(MAKECMDGOALS); \
        done

