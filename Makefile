CFLAGS=-fPIC -D SIMULATE_FIELDS -g -I/usr/include
LIBNAME=blowfish.so
INCLUDEDIR=/usr/include

all: src/blowfish.o src/lua_blowfish.o
ifneq ($(wildcard "./build"),)
	mkdir build
endif
	cd src && \
	pwd && \
	$(CC) -shared -L$(INCLUDEDIR) -o ../build/$(LIBNAME) blowfish.o lua_blowfish.o

src/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	cd ./src && \
	find -name "*.o" -delete
	cd ./build && \
	if [ -a $(LIBNAME) ]; then rm $(LIBNAME); fi;
