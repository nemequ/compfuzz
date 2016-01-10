SRCS    := $(wildcard src/*.c) $(wildcard src/density/src/spookyhash/src/*.c) $(wildcard src/density/src/*.c)
OBJS    := $(SRCS:.c=.o)

SANITIZE = -fstack-protector -DFORTIFY_SOURCE=2 -fsanitize=undefined -fsanitize=address -fsanitize-trap=array-bounds,bool,enum,float-cast-overflow,float-divide-by-zero,function,integer-divide-by-zero,nonnull-attribute,null,object-size,returns-nonnull-attribute,shift-base,shift-exponent,signed-integer-overflow,vla-bound -fno-sanitize=alignment
CFLAGS += $(SANITIZE)

all:
	$(CC) --std=c99 -fgnu89-inline $(CFLAGS) $(SRCS) -o density

clean:
	rm $(OBJS) density