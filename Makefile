CC = cc
RM = rm -rf
CFLAGS = -Wall -Wextra -Werror -g3
LIB = -L./libft -lft -lreadline

SRC = chicken.c \
	  lexicaltokenizer.c \
	  parser.c \
	  setenv.c \
	  cd.c echo.c env.c exit.c export.c pwd.c unset.c chkn_execute.c get_path.c redirect.c \
	  expand.c command_parse_logic.c lex_expression.c count_tokens.c catagorize.c clean.c init.c \
	  execute_utis.c

OBJ = $(SRC:.c=.o)

NAME = chicken

all: $(NAME)

$(NAME): $(OBJ)
	$(MAKE) -C ./libft
	$(CC) $(OBJ) $(LIB) -o $(NAME) $(CFLAGS)

clean:
	$(RM) $(OBJ)
	$(MAKE) clean -C ./libft

fclean: clean
	$(RM) $(NAME)
	$(MAKE) fclean -C ./libft

re: fclean $(NAME)

.PHONY: all clean fclean re
