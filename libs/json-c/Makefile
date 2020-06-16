#================================COMMON=======================================#
APP_NAME				= jsonc.a

CC						= clang

DIR						= libs/json-c

#=================================FLAGS=======================================#
C_FLAGS					= -std=c11

ADD_FLAGS				= -g

LINKER_FLAGS			=

#==================================INC========================================#
INCD					= inc

INC						= apps_config.h arraylist.h config.h debug.h \
						  json_c_version.h json_config.h json_inttypes.h \
						  json_object_iterator.h json_object_private.h \
						  json_object.h json_pointer.h json_tokener.h \
						  json_types.h json_util.h json_visit.h json.h \
						  linkhash.h math_compat.h printbuf.h \
						  random_seed.h sprintf_compat.h strdup_compat.h \
						  strerror_override_private.h strerror_override.h \
						  vasprintf_compat.h

INCS					= $(addprefix $(INCD)/, $(INC))

#===================================OBJ=======================================#
OBJD					= obj

OBJ_DIRS				= $(OBJD)

OBJS					= $(addprefix $(OBJD)/, $(SRCS:%.c=%.o))

#===================================SRC=======================================#
SRCS 					= arraylist.c debug.c json_c_version.c json_object.c \
						  json_pointer.c json_object_iterator.c \
						  json_tokener.c json_util.c json_visit.c libjson.c \
						  linkhash.c printbuf.c random_seed.c \
						  strerror_override.c

SRCD					= src

#================================FUNCTIONS====================================#
define compile_dependency
	$(CC) $(C_FLAGS) $(ADD_FLAGS) -c $(1) -o $(2) -I $(INCD)
	@printf "\r\33[2K$(DIR)\t\t\033[33;1mcompile\t\t\033[0m$(<:$(SRCD)/%.c=%)"
endef

#=================================RULES=======================================#
all: install

install: $(APP_NAME) $(INCD)

$(OBJ_DIRS):
	@mkdir -p $@

clean:
	@rm -rf $(OBJD)
	@printf "$(DIR)/$(OBJD)\t\t\033[31;1mdeleted\033[0m\n"

uninstall: clean
	@rm -rf $(APP_NAME)
	@printf "$(APP_NAME)\t\t\t\033[31;1muninstalled\033[0m\n"

reinstall: uninstall install

#================================DEPENDENCIES=================================#
$(APP_NAME): $(OBJS)
	@ar rcs $@ $^
	@printf "\r\33[2K$@\t\t\t\033[32;1mcreated\033[0m\n"

$(OBJD)/%.o: $(SRCD)/%.c
	$(call compile_dependency, $<, $@)

$(OBJS): | $(OBJ_DIRS)
