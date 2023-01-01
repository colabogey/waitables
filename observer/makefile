#SUBDIRS = src tests
release:
	make -C ./src
	make -C ./src install
	make -C ./test    
	make -C ./test test

debug:
	make -C ./src DEBUG=TRUE
	make -C ./src install
	make -C ./test DEBUG=TRUE
	make -C ./test coverage

clean:
	make -C ./src clean
	make -C ./test clean

testclean:
	make -C ./test testclean

