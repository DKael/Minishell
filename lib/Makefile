liball: readline

clean:
	rm -rf tmp
	rm -rf readline-8.2
	rm -f readline

fclean: clean
	rm -rf bin include lib share

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re

readline:
	$(MAKE) fclean
	tar -xzf ./readline-8.2.tar.gz
	cd readline-8.2 && sh ./configure --prefix="$$(cd .. && pwd)" && make && make install
	touch $@
