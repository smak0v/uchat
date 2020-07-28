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
		-I /usr/local/opt/openssl/include -I libs/libportaudio/include \
		-I libs/libsndfile/include

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
COMMON_SRCS				= check_port.c check_ip.c mkdir.c

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

SERVER_OBJS_FILES		= $(SERVER_OBJS) $(SERVER_DB_OBJS)

#===================================SRC=======================================#
DB_SRCS					= dbfunc.c db_user.c db_group_members.c \
						new_table.c db_user_del.c db_group_members2.c \
						db_dialog.c db_dialog_del.c db_group.c sock_table.c \
						db_messages.c db_get_messages.c db_auth.c \
						db_group_members3.c db_dialog2.c db_get_messages2.c \
						db_get_messages3.c db_profile.c db_profile2.c \
						db_user2.c sock_table2.c db_group2.c

SERVER_SRCS				= main.c threads.c request_processing.c register.c\
						sign_in.c sign_out.c groups.c ssl_tls.c messages.c \
						message_extraction.c validate_token.c json_builder.c \
						wrappers.c json_array_builders.c loaders.c \
						load_messages.c get_user.c groups2.c json_builders2.c \
						user_profile.c send_to_all_clients.c sockets.c \
						remove_socket.c profile_json.c notifications.c \
						recv_file.c unpackers.c file_transfer.c daemonize.c \
						ssl_list.c groups3.c json_builders3.c utils.c \
						utils2.c send_file_serv.c

SERVER					= $(addprefix server/, $(SERVER_SRCS))

#================================DEPENDENCIES=================================#
$(SERVER_OBJ_DIRS):
	@mkdir -p $@

$(SERVER_APP_NAME): $(COMMON_OBJS) $(SERVER_OBJS_FILES)
	@$(CC) $(C_FLAGS) $(ADD_FLAGS) $(LINKER_FLAGS) $(LIBJSONA) $(SQLITEA) \
		$(COMMON_OBJS) $(SERVER_OBJS_FILES) \
		-L $(LIBMXD) \
		-L $(LIBJSOND) \
		-L /usr/local/opt/openssl/lib \
		-lmx \
		-lssl \
		-lcrypto \
		-fsanitize=address,undefined -g3 -rdynamic \
		-o $@

	@printf "\r\33[2K$@\t\t\033[32;1mcreated\033[0m\n"

$(SERVER_OBJD)/%.o: $(SRCD)/server/%.c $(INCS)
	$(call compile_dependency, $<, $@)

$(SERVER_OBJS): | $(SERVER_OBJ_DIRS) $(COMMON_OBJ_DIRS)





#*****************************************************************************#
#**********************************CLIENT*************************************#
#*****************************************************************************#

#==================================OBJ========================================#
CLIENT_OBJD				= $(OBJD)/client

VALIDATORS_OBJD			= $(CLIENT_OBJD)/validators

UTILS_OBJD				= $(CLIENT_OBJD)/utils

PARSERS_OBJD			= $(CLIENT_OBJD)/parsers

BUILDERS_OBJD			= $(CLIENT_OBJD)/builders

GUI_FUNCS_OBJD			= $(CLIENT_OBJD)/gui_funcs

PROCESSORS_OBJD			= $(CLIENT_OBJD)/processors

AUDIO_OBJD				= $(CLIENT_OBJD)/audio

CLIENT_OBJ_DIRS			= $(CLIENT_OBJD) $(VALIDATORS_OBJD) $(UTILS_OBJD) \
						  $(PARSERS_OBJD) $(BUILDERS_OBJD) $(GUI_FUNCS_OBJD) \
						  $(PROCESSORS_OBJD) $(AUDIO_OBJD)

CLIENT_OBJS				= $(addprefix $(OBJD)/, $(CLIENT:%.c=%.o))

CLIENT_VALIDATORS_OBJS	= $(addprefix $(OBJD)/client/validators/, \
						  $(VALIDATORS_SRCS:%.c=%.o))

CLIENT_UTILS_OBJS		= $(addprefix $(OBJD)/client/utils/, \
						  $(UTILS_SRCS:%.c=%.o))

CLIENT_PARSERS_OBJS		= $(addprefix $(OBJD)/client/parsers/, \
						  $(PARSERS_SRCS:%.c=%.o))

CLIENT_BUILDERS_OBJS	= $(addprefix $(OBJD)/client/builders/, \
						  $(BUILDERS_SRCS:%.c=%.o))

CLIENT_GUI_FUNCS_OBJS	= $(addprefix $(OBJD)/client/gui_funcs/, \
						  $(GUI_FUNCS_SRCS:%.c=%.o))

CLIENT_PROCESSORS_OBJS	= $(addprefix $(OBJD)/client/processors/, \
						  $(PROCESSORS_SRCS:%.c=%.o))

CLIENT_AUDIO_OBJS		= $(addprefix $(OBJD)/client/audio/, \
						  $(AUDIO_SRCS:%.c=%.o))

CLIENT_OBJS_FILES		= $(CLIENT_OBJS) $(CLIENT_UTILS_OBJS) \
						  $(CLIENT_VALIDATORS_OBJS) $(CLIENT_PARSERS_OBJS) \
						  $(CLIENT_BUILDERS_OBJS) $(CLIENT_GUI_FUNCS_OBJS) \
						  $(CLIENT_PROCESSORS_OBJS) $(CLIENT_AUDIO_OBJS)

#===================================SRC=======================================#
CLIENT_SRCS				= main.c

VALIDATORS_SRCS			= validate_login_data.c validate_signup_data.c

UTILS_SRCS				= clear_jobj.c read_server_response.c ssl_tls.c \
						  threads.c sockets.c send_file.c get_time.c \
						  main_thread_struct_utils.c is_audio.c \
						  play_audio_thread.c recv_file_cli.c

PARSERS_SRCS			= login_response.c signup_response.c \
						  logout_response.c new_group_response.c \
						  load_groups_response.c load_messages_response.c \
						  send_message_response.c load_dialogs_response.c \
						  get_profile_response.c search_user_response.c \
						  invite_user_to_group_response.c \
						  search_invite_user_response.c \
						  parse_load_group_members_response.c

BUILDERS_SRCS			= login_signup_builder.c logout_builder.c \
						  new_group_builder.c load_dialogs_groups_builder.c \
						  send_message_builder.c load_messages_builder.c \
						  search_user_builder.c edit_profile_builder.c \
						  send_file_builder.c get_profile_builder.c \
						  invite_user_to_group_builder.c \
						  leave_group_builder.c delete_account_builder.c \
						  group_members_builder.c download_file_builder.c

PROCESSORS_SRCS			= main_processor.c s_in_s_up.c s_out.c \
						  load_dialogues.c load_groups.c find_user.c n_grp.c \
						  load_messages.c edit_profile.c search_users.c \
						  s_msg.c inv.c notif_add_to_gr.c leave_group.c \
						  delete_account.c load_gr_members.c

GUI_FUNCS_SRCS			= clear_login_inputs.c clear_signup_inputs.c \
						  win_login_signup.c win_chat.c gui_utils.c \
						  auth.c find_gtk_objects.c download_file.c \
						  edit_profile.c get_gtk_obj.c add_chat.c add_group.c \
						  send_message.c load_groups.c delete_childs.c \
						  load_messages.c close_chat.c attach_file.c \
						  load_dialogues.c get_profile.c profile.c \
						  message_input_utils.c messages.c open_dialog.c \
						  gtk_quit.c invite_user_to_group.c audio.c \
						  append_file_to_msg_block.c leave_group.c \
						  delete_account.c change_theme.c \
						  open_group_members_dialog.c pilin_pilin.c \
						  enter_actions.c

AUDIO_SRCS				= audio_init.c audio_playback.c audio_recording.c \
						  audio_utils.c

CLIENT					= $(addprefix client/, $(CLIENT_SRCS))

#================================DEPENDENCIES=================================#
$(CLIENT_OBJ_DIRS):
	@mkdir -p $@

$(CLIENT_APP_NAME): $(CLIENT_OBJS_FILES) $(COMMON_OBJS)
	@$(CC) $(C_FLAGS) $(ADD_FLAGS) $(LINKER_FLAGS) $(LIBJSONA) $(COMMON_OBJS) \
		$(CLIENT_OBJS_FILES) \
		-L $(LIBMXD) \
		-L $(LIBJSOND) \
		-L /usr/local/opt/openssl/lib \
		-lmx \
		-lssl \
		-lcrypto \
		libs/libportaudio/libportaudio.a \
		libs/libsndfile/libsndfile.a \
		-framework CoreAudio \
		-framework AudioToolbox \
		-framework AudioUnit \
		-framework Carbon \
		-fsanitize=address,undefined -g3 -rdynamic \
		-o $@ $(GTK_LIBS)

	@printf "\r\33[2K$@\t\t\t\033[32;1mcreated\033[0m\n"

$(CLIENT_OBJD)/%.o: $(SRCD)/client/%.c $(INCS)
	$(call compile_dependency, $<, $@)

$(CLIENT_OBJS): | $(CLIENT_OBJ_DIRS) $(COMMON_OBJ_DIRS)
