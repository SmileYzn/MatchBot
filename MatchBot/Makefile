CSSDK 	= include/cssdk
METAMOD = include/metamod

NAME = matchbot

COMPILER = c++

OBJECTS = *.cpp

LINK =

OPT_FLAGS = -std=c++11 -Wall -pipe -g0 -O3 -s -flto -funroll-loops -fomit-frame-pointer -fno-stack-protector -fPIC

INCLUDE = -I. -I$(CSSDK)/common -I$(CSSDK)/dlls -I$(CSSDK)/engine -I$(CSSDK)/game_shared -I$(CSSDK)/pm_shared -I$(CSSDK)/public -I$(METAMOD)

CFLAGS = $(OPT_FLAGS) -g -DNDEBUG -Dlinux -D__linux__ -D_GLIBCXX_USE_CXX11_ABI=0 -U_FORTIFY_SOURCE -m32 -msse3 -msse4.1 -shared

BIN_DIR = Release

OBJ_LINUX := $(OBJECTS:%.c=$(BIN_DIR)/%.o)

$(BIN_DIR)/%.o: %.c

	$(COMPILER) $(INCLUDE) $(CFLAGS) -o $@ -c $<

all:
	mkdir -p $(BIN_DIR)

	$(MAKE) $(NAME) && strip -x $(BIN_DIR)/$(NAME)_mm.so

$(NAME): $(OBJ_LINUX)

	$(COMPILER) $(INCLUDE) $(CFLAGS) $(OBJ_LINUX) $(LINK) -o$(BIN_DIR)/$(NAME)_mm.so

check:
	cppcheck $(INCLUDE) --quiet --max-configs=100 --std=c++11 -DNDEBUG -Dlinux -D__linux__ -U_FORTIFY_SOURCE .

default: all

clean:
	rm -rf Release/*.o
	rm -rf Release/*.so
