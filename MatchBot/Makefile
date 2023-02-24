CSSDK 	= include/cssdk
METAMOD = include/metamod

NAME = matchbot

COMPILER = g++

OBJECTS = *.cpp

LINK = -static-libstdc++

OPT_FLAGS = -std=c++17 -g0 -O3 -s -flto -funroll-loops -fno-stack-protector

INCLUDE = -I. -I$(CSSDK)/common -I$(CSSDK)/dlls -I$(CSSDK)/engine -I$(CSSDK)/game_shared -I$(CSSDK)/pm_shared -I$(CSSDK)/public -I$(METAMOD)

CFLAGS = $(OPT_FLAGS) -g -DNDEBUG -Dlinux -D__linux__ -D__USE_GNU -U_FORTIFY_SOURCE -m32 -msse -msse2 -msse3 -mssse3 -mmmx -shared

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
	cppcheck $(INCLUDE) --quiet --max-configs=100 --std=c++17 -DNDEBUG -Dlinux -D__linux__ -D__USE_GNU -U_FORTIFY_SOURCE .

default: all

clean:
	rm -rf Release/*.o
	rm -rf Release/*.so
