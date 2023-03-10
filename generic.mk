
release:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir ; \
	done

debug:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir DEBUG=TRUE; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $(MAKECMDGOALS) $(PROJECT_BASE); \
	done

testclean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $(MAKECMDGOALS); \
	done

tests:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $(MAKECMDGOALS); \
	done

coverage:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $(MAKECMDGOALS); \
	done

