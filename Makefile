CC = gcc
CCFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -g -fsanitize=address,undefined,leak

SRC_INCLUDE = ./includes/nm.h
NAME = ft_nm
SRC_PATH = ./srcs
SRC = $(addprefix $(SRC_PATH)/, main.c parse_cli_arguments.c utils.c error.c read_bytes.c nm.c nm32.c symbol_type.c)
OBJ_PATH = ./obj
OBJ = $(SRC:$(SRC_PATH)/%.c=$(OBJ_PATH)/%.o)

all : creat_obj_dir $(NAME)

$(NAME) : $(OBJ) $(SRC_INCLUDE)
	$(CC) $(CCFLAGS) -o $@ $(OBJ)

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c $(SRC_INCLUDE)
	$(CC) $(CCFLAGS) -c $< -o $@

creat_obj_dir:
	@if [ ! -d $(OBJ_PATH) ]; then mkdir $(OBJ_PATH); fi

clean:
	@rm -rf $(OBJ_PATH)

fclean: clean
	@rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re
