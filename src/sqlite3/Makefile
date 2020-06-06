CC = gcc -std=c89

OBJ = sqlite3

SQLITE_OBJ = sqlite3.o

SQLITE_LIB = sqlite3lib

SQLITE = ../../$(SQLITE_LIB).a

all: sql

sql: $(SQLITE)

$(SQLITE): $(SQLITE_OBJ)
	cp sqlite3.h ../../inc
	@$(AR) rcs $@ $<
	@printf "\033[32;1m$(notdir $@) created\033[0m\n"

$(SQLITE_OBJ): sqlite3.c sqlite3.h
	@$(CC) -c $<

uninstall: clean
	@rm -rf $(SQLITE)

clean:
	@rm -rf $(SQLITE_OBJ)
	@rm -rf ../../inc/sqlite3.h

reinstall: uninstall all
