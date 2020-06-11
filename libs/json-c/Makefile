NAME = libjsonc.a
SRC = arraylist.c \
    debug.c \
    json_c_version.c \
    json_object.c \
    json_object_iterator.c \
    json_pointer.c \
    json_tokener.c \
    json_util.c \
    json_visit.c \
    libjson.c \
    linkhash.c \
    printbuf.c \
    random_seed.c \
    strerror_override.c
SRCS = $(addprefix src/, $(SRC))
SRCD = src
OBJD = obj
OBJS = $(SRC:.c=.o)
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic


all: install

install: $(NAME)

$(NAME): $(OBJD)
	@ar -rcs $(NAME) $(OBJS)
	@mkdir -p $(OBJD)
	@mv $(OBJS) $(OBJD)

$(OBJD): $(SRCS)
	@clang -std=c11 -Wall -Wextra -Wpedantic -g -c  src/arraylist.c -o arraylist.o -I inc
	@clang -std=c11 -Wall -Wextra -Wpedantic -g -c  src/debug.c -o debug.o -I inc
	@clang -std=c11 -Wall -Wextra -Wpedantic -g -c  src/json_c_version.c -o json_c_version.o -I inc
	@clang -std=c11 -Wall -Wextra -Wpedantic -g -c  src/json_object.c -o json_object.o -I inc
	@clang -std=c11 -Wall -Wextra -Wpedantic -g -c  src/json_object_iterator.c -o json_object_iterator.o -I inc
	@clang -std=c11 -Wall -Wextra -Wpedantic -g -c  src/json_pointer.c -o json_pointer.o -I inc
	@clang -std=c11 -Wall -Wextra -Wpedantic -g -c  src/json_tokener.c -o json_tokener.o -I inc
	@clang -std=c11 -Wall -Wextra -Wpedantic -g -c  src/json_util.c -o json_util.o -I inc
	@clang -std=c11 -Wall -Wextra -Wpedantic -g -c  src/json_visit.c -o json_visit.o -I inc
	@clang -std=c11 -Wall -Wextra -Wpedantic -g -c  src/libjson.c -o libjson.o -I inc
	@clang -std=c11 -Wall -Wextra -Wpedantic -g -c  src/linkhash.c -o linkhash.o -I inc
	@clang -std=c11 -Wall -Wextra -Wpedantic -g -c  src/libjson.c -o libjson.o -I inc
	@clang -std=c11 -Wall -Wextra -Wpedantic -g -c  src/printbuf.c -o printbuf.o -I inc
	@clang -std=c11 -Wall -Wextra -Wpedantic -g -c  src/random_seed.c -o random_seed.o -I inc
	@clang -std=c11 -Wall -Wextra -Wpedantic -g -c  src/strerror_override.c -o strerror_override.o -I inc

uninstall:
	@rm -rf $(NAME)

clean:
	@rm -rf ./obj

reinstall: uninstall install
