SRCS			=	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
       			ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
       			ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
      			ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
      			ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c \
      			ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
      			ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
      			ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c ft_lstclear_bonus.c ft_lstdelone_bonus.c \
			ft_lstiter_bonus.c ft_lstlast_bonus.c ft_lstmap_bonus.c ft_lstnew_bonus.c ft_lstsize_bonus.c

PRINTF_SRC		= $(addprefix printf/, ft_printf_bonus.c ft_print_left_right_bonus.c \
			ft_handle_integer_bonus.c ft_parse_flags_bonus.c ft_handle_hex_bonus.c \
			ft_handle_hex_caps_bonus.c ft_handle_char_bonus.c \
			ft_handle_string_bonus.c ft_handle_udecimal_bonus.c ft_uitoa.c \
			ft_handle_pointer_bonus.c ft_hextoa_bonus.c ft_handle_percentage_bonus.c\
			ft_ptrtoa_bonus.c)
			
GNL_SRC			= $(addprefix gnl/, get_next_line_bonus.c get_next_line_utils_bonus.c)
			      			
OBJS			= $(SRCS:.c=.o)
PRINTF_OBJS		= $(PRINTF_SRC:.c=.o)
GNL_OBJS		= $(GNL_SRC:.c=.o)

CC				= cc
RM				= rm -rf 
CFLAGS			= -Wall -Wextra -Werror

NAME			= libft.a

all:			$(NAME)

$(NAME):		$(OBJS) $(PRINTF_OBJS) $(GNL_OBJS)
				ar rcs $(NAME) $(OBJS) $(PRINTF_OBJS) $(GNL_OBJS)

%.o: %.c
				$(CC) $(CFLAGS) -c $< -o $@
clean:
				$(RM) $(OBJS) $(PRINTF_OBJS) $(GNL_OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY:			all clean fclean re 
