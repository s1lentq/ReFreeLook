NAME = refreelook
#COMPILER = clang
COMPILER = /opt/intel/bin/icpc
CSSDK = include/cssdk

OBJECTS = src/main.cpp src/meta_api.cpp src/h_export.cpp src/amxxmodule.cpp src/mod_regamedll_api.cpp\
	src/sdk_util.cpp $(CSSDK)/public/interface.cpp\

LINK = -lm -ldl -static-libgcc

ifeq "$(COMPILER)" "clang"
	LINK += -Wno-return-type-c-linkage -fPIC
else
	LINK += -static-intel -no-intel-extensions
endif

METAMOD = include/metamod

OPT_FLAGS = -O3 -msse3 -funroll-loops -fomit-frame-pointer -fno-stack-protector -g
ifeq "$(COMPILER)" "clang"
	OPT_FLAGS += -fwritable-strings -Wreturn-type-c-linkage
else
	OPT_FLAGS += -no-prec-div
endif

INCLUDE = -I. -Isrc -I$(CSSDK)/common -I$(CSSDK)//dlls -I$(CSSDK)/engine -I$(CSSDK)/pm_shared -I$(CSSDK)/public -I$(METAMOD) -I$(CSSDK)/game_shared



BIN_DIR = Release
CFLAGS = $(OPT_FLAGS)

CFLAGS += -g0 -fvisibility=hidden -DNOMINMAX -fvisibility-inlines-hidden\
	-DNDEBUG -Dlinux -D__linux__ -std=c++11 -shared -fasm-blocks\
	-fno-rtti -D_vsnprintf=vsnprintf

ifeq "$(COMPILER)" "clang"
	CFLAGS += -D_bswap16=__builtin_bswap16 -D_bswap=bswap -D_bswap64=__builtin_bswap64
else
	CFLAGS += -Qoption,cpp,--treat_func_as_string_literal_cpp
endif

OBJ_LINUX := $(OBJECTS:%.c=$(BIN_DIR)/%.o)

$(BIN_DIR)/%.o: %.c
	$(COMPILER) $(INCLUDE) $(CFLAGS) -o $@ -c $<

all:
	mkdir -p $(BIN_DIR)
	mkdir -p $(BIN_DIR)/sdk

	$(MAKE) $(NAME) && strip -x $(BIN_DIR)/$(NAME)_amxx_i386.so

$(NAME): $(OBJ_LINUX)
	$(COMPILER) $(INCLUDE) $(CFLAGS) $(OBJ_LINUX) $(LINK) -o$(BIN_DIR)/$(NAME)_amxx_i386.so

check:
	cppcheck $(INCLUDE) --quiet --max-configs=100 -D__linux__ -DNDEBUG -DHAVE_STDINT_H .

debug:	
	$(MAKE) all DEBUG=false

default: all

clean:
	rm -rf Release/sdk/*.o
	rm -rf Release/*.o
	rm -rf Release/$(NAME)_amxx_i386.so

