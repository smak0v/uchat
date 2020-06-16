CC = clang -std=c11

OBJ = sqlite3

SQLITE_OBJ = sqlite3.o

SQLITE_LIB = sqlite3

SQLITE = $(SQLITE_LIB).a

all: install

install: $(SQLITE)

$(SQLITE): $(SQLITE_OBJ)
	@$(AR) rcs $@ $<
	@printf "$(SQLITE) \t\033[32;1mcreated\033[0m\n"

$(SQLITE_OBJ): src/sqlite3.c inc/sqlite3.h
	@$(CC) -c $<

uninstall: clean
	@rm -rf $(SQLITE)
	@printf "$(SQLITE)\t\033[31;1muninstalled\033[0m\n"

clean:
	@rm -rf $(SQLITE_OBJ)

reinstall: uninstall all
