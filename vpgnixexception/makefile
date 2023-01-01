SUBDIRS = src test

release:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $(MAKECMDGOALS); \
	done

debug:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir DEBUG=TRUE; \
	done
	@make -C ./test coverage

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $(MAKECMDGOALS); \
	done

testclean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $(MAKECMDGOALS); \
	done

