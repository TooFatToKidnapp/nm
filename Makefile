CC = cc
CCFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -g -fsanitize=address,undefined,integer

SRC_INCLUDE = includes/nm.h
NAME = ft_nm
INC = $(addprefix -I, $(SRC_INCLUDE))
SRC_PATH = ./srcs
SRC = $(addprefix $(SRC_PATH)/, main.c)
OBJ_PATH = ./obj
OBJ = $(SRC:$(SRC_PATH)/%.c=$(OBJ_PATH)/%.o)

all : $(NAME)

$(NAME) : creat_obj_dir $(OBJ)
	$(CC) $(CCFLAGS) -o $@ $(OBJ)

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	$(CC) $(CCFLAGS) -c $< -o $@

creat_obj_dir:
	@if [ ! -d $(OBJ_PATH) ]; then mkdir $(OBJ_PATH); fi

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re
