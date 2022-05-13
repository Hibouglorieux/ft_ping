NAME = ft_ping

FILES = main.c \
		parser.c \
		utilities.c\
		ft_ping.c

OBJ = $(addprefix obj/,$(FILES:.c=.o))

CXXFLAGS = -Wall -Wextra -g #-pg #-fsanitize=address
CXX = gcc

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $^ -o $@

obj/%.o:src/%.c includes/*.h
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@ -Iincludes -I$(HOME)/.brew/include

clean :
	rm -rf obj

fclean : clean
	rm -rf $(NAME)

re : fclean all

