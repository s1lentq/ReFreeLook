NAME = refreelook
COMPILER = /opt/intel/bin/icpc
CSSDK = include/cssdk

OBJECTS = src/main.cpp src/meta_api.cpp src/h_export.cpp src/amxxmodule.cpp src/mod_regamedll_api.cpp\
	src/sdk_util.cpp $(CSSDK)/public/interface.cpp\

LINK = -lm -ldl -static-intel -static-libgcc -no-intel-extensions

METAMOD = include/metamod

OPT_FLAGS = -O3 -msse3 -no-prec-div -funroll-loops -fomit-frame-pointer -fno-stack-protector -g

INCLUDE = -I. -Isrc -I$(CSSDK)/common -I$(CSSDK)//dlls -I$(CSSDK)/engine -I$(CSSDK)/pm_shared -I$(CSSDK)/public -I$(METAMOD)

BIN_DIR = Release
CFLAGS = $(OPT_FLAGS)

CFLAGS += -g0 -fvisibility=hidden -DNOMINMAX -fvisibility-inlines-hidden\
	-DNDEBUG -Dlinux -D__linux__ -std=c++11 -shared -wd147,274 -fasm-blocks\
	-Qoption,cpp,--treat_func_as_string_literal_cpp -fno-rtti

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


