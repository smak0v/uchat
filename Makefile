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

GTK_CFLAGS 				= `pkg-config --cflags gtk+-3.0`

GTK_LIBS				= `pkg-config --libs gtk+-3.0`

#=================================LIBMX=======================================#
LIBMXD					= libs/libmx

LIBMXA					:= $(LIBMXD)/libmx.a

LIBMXI					:= $(LIBMXD)/inc

#=================================JSON-C======================================#
LIBJSOND				= libs/json-c

LIBJSONA				:= $(LIBJSOND)/jsonc.a

LIBJSONI				:= $(LIBJSOND)/inc
#=================================SQLITE======================================#
SQLITED 				= libs/sqlite3

SQLITEA 				:= $(SQLITED)/sqlite3.a

SQLITEI					:= $(SQLITED)/inc

#==================================INC========================================#
INCD					= inc

INC						= uchat.h client.h server.h

INCS					= $(addprefix $(INCD)/, $(INC))

#==================================OBJ========================================#
OBJD					= obj

#===================================SRC=======================================#
SRCD					= src

#================================FUNCTIONS====================================#
define compile_dependency
	@$(CC) $(C_FLAGS) $(ADD_FLAGS) $(GTK_CFLAGS) -c $(1) -o $(2) \
		-I $(INCD) -I $(LIBMXI) -I $(LIBJSONI) -I $(SQLITEI) \
		-I /usr/local/opt/openssl/include

	@printf "\r\33[2K$(DIR)\t\t\t\033[33;1mcompile\t\t\033[0m$(<:$(SRCD)%.c=%)"
endef

#=================================RULES=======================================#
all: install

install: $(SQLITED) $(LIBMXD) $(LIBJSOND) $(CLIENT_APP_NAME) \
		 $(SERVER_APP_NAME)

$(LIBJSOND): $(LIBJSONA)

$(LIBJSONA):
	@make -sC $(LIBJSOND)

$(SQLITED): $(SQLITEA)

$(SQLITEA):
	@make -sC $(SQLITED)

$(LIBMXD): $(LIBMXA)

$(LIBMXA):
	@make -sC $(LIBMXD)

clean:
	@make -sC $(SQLITED) $@
	@make -sC $(LIBMXD) $@
	@make -sC $(LIBJSOND) $@
	@rm -rf $(OBJD)
	@printf "$(DIR)/$(OBJD)\t\t\033[31;1mdeleted\033[0m\n"

uninstall: clean
	@make -sC $(SQLITED) $@
	@make -sC $(LIBMXD) $@
	@make -sC $(LIBJSOND) $@
	@rm -rf $(SERVER_APP_NAME)
	@printf "$(SERVER_APP_NAME)\t\t\033[31;1muninstalled\033[0m\n"
	@rm -rf $(CLIENT_APP_NAME)
	@printf "$(CLIENT_APP_NAME)\t\t\t\033[31;1muninstalled\033[0m\n"

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

DB_OBJD					= $(SERVER_OBJD)/db

SERVER_OBJ_DIRS			= $(SERVER_OBJD) $(DB_OBJD)

SERVER_OBJS				= $(addprefix $(OBJD)/, $(SERVER:%.c=%.o))

SERVER_DB_OBJS			= $(addprefix $(OBJD)/server/db/, $(DB_SRCS:%.c=%.o))

#===================================SRC=======================================#
DB_SRCS					= dbfunc.c db_user.c db_group_members.c \
						new_table.c db_user_del.c db_gr_members_del.c \
						db_dialog.c db_dialog_del.c db_group.c \
						db_messages.c db_get_messages.c db_auth.c

SERVER_SRCS				= main.c threads.c request_processing.c reg_sign_in.c \
						clients_linked_list.c sign_out.c

SERVER					= $(addprefix server/, $(SERVER_SRCS))

DB						= $(addprefix server/db/, $(DB_SRCS))

#================================DEPENDENCIES=================================#
$(SERVER_OBJ_DIRS):
	@mkdir -p $@

$(SERVER_APP_NAME): $(SERVER_OBJS) $(COMMON_OBJS) $(SERVER_DB_OBJS)
	@$(CC) $(C_FLAGS) $(ADD_FLAGS) $(LINKER_FLAGS) $(LIBJSONA) $(SQLITEA) \
		$(COMMON_OBJS) $(SERVER_OBJS) $(SERVER_DB_OBJS) -L $(LIBMXD) \
		-L $(LIBJSOND) -L /usr/local/opt/openssl/lib -lmx -lssl -lcrypto  -o $@

	@printf "\r\33[2K$@\t\t\033[32;1mcreated\033[0m\n"

$(SERVER_OBJD)/%.o: $(SRCD)/server/%.c $(INCS)
	$(call compile_dependency, $<, $@)

$(SERVER_OBJD)/db/%.o: $(SRCD)/server/db/%.c $(INCS)
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
CLIENT_SRCS				= main.c init.c build_ui_path.c

CLIENT					= $(addprefix client/, $(CLIENT_SRCS))

#================================DEPENDENCIES=================================#
$(CLIENT_OBJ_DIRS):
	@mkdir -p $@

$(CLIENT_APP_NAME): $(CLIENT_OBJS) $(COMMON_OBJS)
	@$(CC) $(C_FLAGS) $(ADD_FLAGS) $(LINKER_FLAGS) $(COMMON_OBJS) \
		$(CLIENT_OBJS) -L $(LIBMXD) -L $(LIBJSOND) \
		-L /usr/local/opt/openssl/lib -lmx -lssl -lcrypto -o $@ $(GTK_LIBS)

	@printf "\r\33[2K$@\t\t\t\033[32;1mcreated\033[0m\n"

$(CLIENT_OBJD)/%.o: $(SRCD)/client/%.c $(INCS)
	$(call compile_dependency, $<, $@)

$(CLIENT_OBJS): | $(CLIENT_OBJ_DIRS) $(COMMON_OBJ_DIRS)
