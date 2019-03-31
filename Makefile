NAME =	Particle_System
CC =	clang++
FLAGS =	-Wall -Wextra -std=c++11

SRC_DIR =	src
OBJ_DIR =	.obj
INC_DIR =	include

CLASS =	Window Log gl_Shader gl_Program cl_Program cl_Kernel Camera
SRC =	$(sort \
		$(patsubst %, %.cpp, $(CLASS)) \
		main.cpp events.cpp draw.cpp \
		)
INC =	$(sort \
		$(patsubst %, %.h, $(CLASS)) \
		particle_system.h call.h Buffer.h \
		)
OBJ =	$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))
DEPS =	$(patsubst %.h, $(INC_DIR)/%.h, $(INC))

OS =	$(shell uname -s)

ifeq ($(OS), Linux)
LIBS =	-lOpenCL -lGLEW -lGL -lSDL2 -lm
else
ifeq ($(OS), Darwin)
LIBS =	-framework OpenCL -framework OpenGL -lSDL2 -lm
endif
endif

JOBS =	6

.PHONY: all clean fclean re

all:
	make $(NAME) -j$(JOBS)

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS) | $(OBJ_DIR)
	$(CC) $(FLAGS) -I $(INC_DIR) -o $@ -c $<

$(NAME): $(OBJ)
	$(CC) $(FLAGS) -o $@ $(OBJ) $(LIBS)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME) log

re:
	make fclean
	make all
