CC = cc
RM = rm -rf
CFLAGS = -L./libft -lft -lreadline -g3
LIB = -L./libft -lft

SRC = chicken.c \
	  lexicaltokenizer.c \
	  parser.c \
	  cd.c \
	  setenv.c

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
