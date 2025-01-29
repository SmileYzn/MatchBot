# Include CS SDK
CSSDK 		= include/cssdk

# Include Metamod SDK
METAMOD 	= include/metamod

# Release file output name
NAME 		= matchbot

# Release directory
BIN_DIR 	= Release

# G++
COMPILER 	= g++
OBJECTS 	= *.cpp

# Linker Libs
LINKER 		= 

# C flags
CFLAGS 		= -std=gnu++11 -Wall -Wno-switch -Wno-unknown-pragmas -Wno-strict-aliasing -shared -m32 -mtune=generic -msse3 -msse4.1 -pipe -g0 -O3 -s -funroll-loops -fno-stack-protector -flto -fPIC -g -DNDEBUG -Dlinux -D__linux__ -DGLIBCXX_USE_CXX11_ABI=0 -U_FORTIFY_SOURCE

# Include directory
INCLUDE 	= -I. -I$(CSSDK)/common -I$(CSSDK)/dlls -I$(CSSDK)/engine -I$(CSSDK)/game_shared -I$(CSSDK)/pm_shared -I$(CSSDK)/public -I$(METAMOD)

# Linux objects to build
OBJ_LINUX 	:= $(OBJECTS:%.c=$(BIN_DIR)/%.o)

# Objects output path
$(BIN_DIR)/%.o: %.c

# Compile command
	$(COMPILER) $(INCLUDE) $(CFLAGS) -o $@ -c $<

all:
	mkdir -p $(BIN_DIR)
	$(MAKE) $(NAME)

$(NAME): $(OBJ_LINUX)

	$(COMPILER) $(INCLUDE) $(CFLAGS) $(OBJ_LINUX) $(LINKER) -o$(BIN_DIR)/$(NAME)_mm.so

check:
	cppcheck $(INCLUDE) --quiet --max-configs=100 -DNDEBUG -Dlinux -D__linux__ -DGLIBCXX_USE_CXX11_ABI=1 -U_FORTIFY_SOURCE .

default: all

clean:
	rm -rf Release/*.o
	rm -rf Release/*.so
