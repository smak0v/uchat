#================================COMMON=======================================#
APP_NAME				= libmx.a

CC						= clang

DIR						= libs/libmx

#=================================FLAGS=======================================#
C_FLAGS					= -std=c11 $(addprefix -W, all extra error pedantic)

ADD_FLAGS				= -g

LINKER_FLAGS			=

#==================================INC========================================#
INCD					= inc

INC						= libmx.h

INCS					= $(addprefix $(INCD)/, $(INC))

#===================================OBJ=======================================#
OBJD					= obj

PACK_MEMORY_OBJD		= $(OBJD)/pack_memory
PACK_SET_OBJD			= $(OBJD)/pack_set
PACK_STRING_OBJD		= $(OBJD)/pack_string
PACK_TREE_OBJD			= $(OBJD)/pack_tree
PACK_UTIL_OBJD			= $(OBJD)/pack_util
PACK_LIST_OBJD			= $(OBJD)/pack_list

PACK_DLL_LIST_OBJD		= $(PACK_LIST_OBJD)/doubly_linked_list
PACK_SLL_LIST_OBJD		= $(PACK_LIST_OBJD)/singly_linked_list

OBJ_DIRS				= $(OBJD) $(PACK_MEMORY_OBJD) $(PACK_SET_OBJD) \
						  $(PACK_STRING_OBJD) $(PACK_TREE_OBJD) \
						  $(PACK_UTIL_OBJD) $(PACK_LIST_OBJD) \
						  $(PACK_DLL_LIST_OBJD) $(PACK_SLL_LIST_OBJD)

OBJS					= $(addprefix $(OBJD)/, $(LIST:%.c=%.o) \
						  $(TREE:%.c=%.o) $(SET:%.c=%.o) $(UTIL:%.c=%.o) \
						  $(STRING:%.c=%.o) $(MEMORY:%.c=%.o))

#===================================SRC=======================================#
DLL_SRCS				= mx_dll_create_node.c mx_dll_push_front.c \
						  mx_dll_push_back.c mx_dll_pop_front.c

SLL_SRCS				= mx_create_node.c mx_list_size.c mx_reverse_list.c \
						  mx_push_back.c mx_push_front.c mx_pop_back.c \
						  mx_pop_front.c mx_push_second.c mx_sort_list.c \
						  mx_list_merge_sort.c mx_list_insertion_sort.c \
						  mx_is_equal_lists.c mx_delete_list.c

TREE_SRCS				= mx_create_tree_node.c

SET_SRCS				= mx_check_set.c mx_create_set.c

UTIL_SRCS				= mx_printchar.c mx_printchar_endl.c mx_atoi.c\
						  mx_print_unicode.c mx_printstr.c mx_printstr_endl.c \
						  mx_print_error.c mx_print_error_endl.c mx_pow.c \
						  mx_print_strarr.c mx_strarr_len.c mx_printint.c \
						  mx_printint_endl.c mx_sqrt.c mx_hex_to_nbr.c \
						  mx_nbr_to_hex.c mx_itoa.c mx_lltoa.c mx_ulltoa.c \
						  mx_ctoi.c mx_foreach.c mx_binary_search.c \
						  mx_bubble_sort.c mx_quicksort.c mx_get_arr_length.c \
						  mx_del_intarr.c mx_intdel.c mx_get_file_length.c \
						  mx_count_unequal.c mx_numlen.c mx_unumlen.c \
						  mx_terminate.c mx_printchar_ntimes.c mx_isprime.c

STRINGS_SRCS			= mx_strlen.c mx_swap_char.c mx_str_reverse.c \
						  mx_strdel.c mx_pop_string_array.c mx_del_strarr.c \
						  mx_get_char_index.c mx_strdup.c mx_strndup.c \
						  mx_strcpy.c mx_strncpy.c mx_strcmp.c mx_strncmp.c \
						  mx_strcat.c mx_strstr.c mx_get_substr_index.c \
						  mx_count_substr.c mx_count_words.c mx_strnew.c \
						  mx_strtrim.c mx_del_extra_spaces.c mx_strsplit.c \
						  mx_strjoin.c mx_file_to_str.c mx_read_line.c \
						  mx_replace_substr.c mx_isspace.c mx_isalpha.c \
						  mx_isdigit.c mx_islower.c mx_isupper.c mx_tolower.c \
						  mx_toupper.c mx_strarr_dup.c mx_strarr_to_str.c \
						  mx_strarr_join.c mx_push_to_strarr.c

MEMORY_SRCS				= mx_memset.c mx_memcpy.c mx_memccpy.c mx_memcmp.c \
						  mx_memchr.c mx_memrchr.c mx_memmem.c mx_memmove.c \
						  mx_realloc.c mx_memalloc.c

LIST					= $(addprefix pack_list/, $(DOUBLY_LINKED_LIST) \
												  $(SINGLY_LINKED_LIST))
TREE					= $(addprefix pack_tree/, $(TREE_SRCS))
SET						= $(addprefix pack_set/, $(SET_SRCS))
UTIL					= $(addprefix pack_util/, $(UTIL_SRCS))
STRING					= $(addprefix pack_string/, $(STRINGS_SRCS))
MEMORY					= $(addprefix pack_memory/, $(MEMORY_SRCS))

DOUBLY_LINKED_LIST		= $(addprefix doubly_linked_list/, $(DLL_SRCS))
SINGLY_LINKED_LIST		= $(addprefix singly_linked_list/, $(SLL_SRCS))

SRCD					= src

#================================FUNCTIONS====================================#
define compile_dependency
	$(CC) $(C_FLAGS) $(ADD_FLAGS) -c $(1) -o $(2) -I $(INCD)
	@printf "\r\33[2K$(DIR)\t\t\033[33;1mcompile\t\t\033[0m$(<:$(SRCD)/%.c=%)"
endef

#=================================RULES=======================================#
all: install

install: $(APP_NAME)

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

$(OBJD)/%.o: $(SRCD)/%.c $(INCS)
	$(call compile_dependency, $<, $@)

$(PACK_DLL_LIST_OBJD)/%.o: $(SRCD)/pack_list/doubly_linked_list/%.c $(INCS)
	$(call compile_dependency, $<, $@)

$(PACK_SLL_LIST_OBJD)/%.o: $(SRCD)/pack_list/singly_linked_list/%.c $(INCS)
	$(call compile_dependency, $<, $@)

$(PACK_MEMORY_OBJD)/%.o: $(SRCD)/pack_memory/%.c $(INCS)
	$(call compile_dependency, $<, $@)

$(PACK_SET_OBJD)/%.o: $(SRCD)/pack_set/%.c $(INCS)
	$(call compile_dependency, $<, $@)

$(PACK_STRING_OBJD)/%.o: $(SRCD)/pack_string/%.c $(INCS)
	$(call compile_dependency, $<, $@)

$(PACK_TREE_OBJD)/%.o: $(SRCD)/pack_tree/%.c $(INCS)
	$(call compile_dependency, $<, $@)

$(PACK_UTIL_OBJD)/%.o: $(SRCD)/pack_util/%.c $(INCS)
	$(call compile_dependency, $<, $@)

$(OBJS): | $(OBJ_DIRS)
