# Include CSSDK and MetaMod SDK
CSSDK 		= include/cssdk
METAMOD 	= include/metamod
LIBCURL 	= include/curl

# Release file output name
NAME 		= matchbot

# Release directory
BIN_DIR 	= Release

# G++
COMPILER 	= g++
OBJECTS 	= *.cpp

#  Linker
LINK 		= -static-libstdc++ -lcurl -lssl -lcrypto -ldl -lm -lz

# Optional flags
OPT_FLAGS 	= -std=gnu++14 -pipe -g0 -O3 -s -flto -funroll-loops -fno-stack-protector

# C flags
CFLAGS 		= $(OPT_FLAGS) -g -DNDEBUG -Dlinux -D__linux__ -D__USE_GNU -DCURL_STATICLIB -U_FORTIFY_SOURCE -m32 -msse3 -msse4.1 -shared

# Include directory
INCLUDE 	= -I. -I$(CSSDK)/common -I$(CSSDK)/dlls -I$(CSSDK)/engine -I$(CSSDK)/game_shared -I$(CSSDK)/pm_shared -I$(CSSDK)/public -I$(METAMOD) -I$(LIBCURL)/include

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

	$(COMPILER) $(INCLUDE) $(CFLAGS) $(OBJ_LINUX) $(LINK) -o$(BIN_DIR)/$(NAME)_mm.so

check:
	cppcheck $(INCLUDE) --quiet --max-configs=100 --std=gnu++14 -DNDEBUG -Dlinux -D__linux__ -U_FORTIFY_SOURCE .

default: all

clean:
	rm -rf Release/*.o
	rm -rf Release/*.so
