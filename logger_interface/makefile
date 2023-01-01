#SUBDIRS = src
release:
	make -C ./src
	make -C ./src install

debug:
	make -C ./src DEBUG=TRUE
	make -C ./src install

clean:
	make -C ./src clean

testclean:
	@echo "nothing for testclean"

