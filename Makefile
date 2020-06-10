#*****************************************************************************#
#**********************************COMMON*************************************#
#*****************************************************************************#

#================================DIFFERENT====================================#
CLIENT_APP_NAME			= uchat

SERVER_APP_NAME			= uchat_server

CC						= clang

DIR						= uchat

#=================================FLAGS=======================================#
C_FLAGS					= -std=c11 $(addprefix -W, all extra pedantic error)

ADD_FLAGS				= -g

LINKER_FLAGS			=

#=================================LIBMX=======================================#
LIBMXD					= libmx

LIBMXA					:= $(LIBMXD)/libmx.a

LIBMXI					:= $(LIBMXD)/inc

SQLITE_D = src/sqlite3

SQLITE_A := $(SQLITE_D)/sqlite3lib.a

#==================================INC========================================#
INCD					= inc

INC						= uchat.h

INCS					= $(addprefix $(INCD)/, $(INC))

#==================================OBJ========================================#
OBJD					= obj

#===================================SRC=======================================#
SRCD					= src

#================================FUNCTIONS====================================#
define compile_dependency
	@$(CC) $(C_FLAGS) $(ADD_FLAGS) -c $(1) -o $(2) -I $(INCD) -I $(LIBMXI)
	@printf "\r\33[2K$(DIR)\t\t\033[33;1mcompile\t\t\033[0m$(<:$(SRCD)%.c=%)"
endef

#=================================RULES=======================================#
all: install

install: $(SQLITE_D) $(LIBMXD) $(CLIENT_APP_NAME) $(SERVER_APP_NAME)

$(SQLITE_D): $(SQLITE_A)

$(SQLITE_A):
	@make -sC $(SQLITE_D)

$(LIBMXD): $(LIBMXA)

$(LIBMXA):
	@make -sC $(LIBMXD)

clean:
	@make -sC $(SQLITE_D) $@
	@make -sC $(LIBMXD) $@
	@rm -rf $(OBJD)
	@printf "$(DIR)/$(OBJD)\t\033[31;1mdeleted\033[0m\n"

uninstall: clean
	@make -sC $(SQLITE_D) $@
	@make -sC $(LIBMXD) $@
	@rm -rf $(SERVER_APP_NAME)
	@printf "$(SERVER_APP_NAME)\t\033[31;1muninstalled\033[0m\n"
	@rm -rf $(CLIENT_APP_NAME)
	@printf "$(CLIENT_APP_NAME)\t\t\033[31;1muninstalled\033[0m\n"

reinstall: uninstall install

#==================================OBJ========================================#
COMMON_OBJD				= $(OBJD)/common

COMMON_OBJ_DIRS			= $(COMMON_OBJD)

COMMON_OBJS				= $(addprefix $(OBJD)/, $(COMMON:%.c=%.o))

#===================================SRC=======================================#
COMMON_SRCS				= check_port.c check_ip.c

COMMON					= $(addprefix common/, $(COMMON_SRCS))

#================================DEPENDENCIES=================================#
$(COMMON_OBJ_DIRS):
	@mkdir -p $@

$(COMMON_OBJD)/%.o: $(SRCD)/common/%.c $(INCS)
	$(call compile_dependency, $<, $@)

$(CLIENT_OBJS): | $(CLIENT_OBJ_DIRS)]




#*****************************************************************************#
#**********************************SERVER*************************************#
#*****************************************************************************#
#==================================OBJ========================================#
SERVER_OBJD				= $(OBJD)/server

SERVER_OBJ_DIRS			= $(SERVER_OBJD)

SERVER_OBJS				= $(addprefix $(OBJD)/, $(SERVER:%.c=%.o))

#===================================SRC=======================================#
SERVER_SRCS				= main.c dbfunc.c db_user.c db_group_members.c \
						new_table.c db_user_del.c db_gr_members_del.c \
						db_dialog.c db_dialog_del.c db_group.c db_messages.c \


SERVER					= $(addprefix server/, $(SERVER_SRCS))

#================================DEPENDENCIES=================================#
$(SERVER_OBJ_DIRS):
	@mkdir -p $@

$(SERVER_APP_NAME): $(SERVER_OBJS) $(COMMON_OBJS)
	@$(CC) $(C_FLAGS) $(ADD_FLAGS) $(LINKER_FLAGS) $(SQLITE_A) $(COMMON_OBJS) \
										$(SERVER_OBJS) -L $(LIBMXD) -lmx -o $@
	@printf "\r\33[2K$@\t\033[32;1mcreated\033[0m\n"

$(SERVER_OBJD)/%.o: $(SRCD)/server/%.c $(INCS)
	$(call compile_dependency, $<, $@)

$(SERVER_OBJS): | $(SERVER_OBJ_DIRS) $(COMMON_OBJ_DIRS)





#*****************************************************************************#
#**********************************CLIENT*************************************#
#*****************************************************************************#

#==================================OBJ========================================#
CLIENT_OBJD				= $(OBJD)/client

CLIENT_OBJ_DIRS			= $(CLIENT_OBJD)

CLIENT_OBJS				= $(addprefix $(OBJD)/, $(CLIENT:%.c=%.o))

#===================================SRC=======================================#
CLIENT_SRCS				= main.c

CLIENT					= $(addprefix client/, $(CLIENT_SRCS))

#================================DEPENDENCIES=================================#
$(CLIENT_OBJ_DIRS):
	@mkdir -p $@

$(CLIENT_APP_NAME): $(CLIENT_OBJS) $(COMMON_OBJS)
	@$(CC) $(C_FLAGS) $(ADD_FLAGS) $(LINKER_FLAGS) $(COMMON_OBJS) \
										$(CLIENT_OBJS) -L $(LIBMXD) -lmx -o $@
	@printf "\r\33[2K$@\t\t\033[32;1mcreated\033[0m\n"

$(CLIENT_OBJD)/%.o: $(SRCD)/client/%.c $(INCS)
	$(call compile_dependency, $<, $@)

$(CLIENT_OBJS): | $(CLIENT_OBJ_DIRS) $(COMMON_OBJ_DIRS)
