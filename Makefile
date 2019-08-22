APP = attack_logger

# dirs:
BUILD_DIR ?= ./build
OBJ_DIR    = $(BUILD_DIR)/obj
BIN_DIR    = $(BUILD_DIR)/bin
SRC_DIR    = ./src
INC_DIR    = ./include

CXX        = g++
C_FLAGS   := -std=gnu99   -Wall -Werror -Wmissing-declarations -D_GNU_SOURCE
CXX_FLAGS := -std=gnu++0x -Wall -Werror -fno-strict-aliasing -Wno-deprecated-declarations -D_GNU_SOURCE
# CXX_FLAGS += -ggdb
CXX_FLAGS += -g3
LD_FLAGS  :=

LIBS      :=
LIBS      += -pthread

SRC_CXX   := $(wildcard $(SRC_DIR)/*.cxx)
OBJ       := $(foreach odir, $(notdir $(patsubst %.cxx,%.ocxx,$(SRC_CXX))), $(OBJ_DIR)/$(odir))

INCLUDES  :=  $(foreach odir, $(INC_DIR), $(addprefix -I, $(odir)))

TARGETS = clean checkdirs $(APP)

.PHONY: all-parallel all checkdirs clean

all: $(TARGETS)

$(OBJ_DIR)/%.ocxx: $(SRC_DIR)/%.cxx
	@printf "Build:      $@\n"
#@printf "$(CXX) -c $< $(INCLUDES) $(CXX_FLAGS) -o $@\n"
	@$(CXX) -c $< $(INCLUDES) $(CXX_FLAGS) -o $@

$(APP): $(OBJ)
	@printf "Build:      $(APP)\n"
#@printf "@$(CXX) $(INCLUDES) $(OBJ) $(CXX_FLAGS) $(LIBS) -o $(BIN_DIR)/$(APP)\n"
	@$(CXX) $(INCLUDES) $(OBJ) $(CXX_FLAGS) $(LIBS) -o $(BIN_DIR)/$(APP)

checkdirs:
	@printf "Create dir: $(OBJ_DIR)\n"
	@printf "Create dir: $(BIN_DIR)\n"
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

clean:
	@printf "Remove dir: $(OBJ_DIR)\n"
	@printf "Remove dir: $(BIN_DIR)\n"
	-@rm -rf $(OBJ_DIR)
	-@rm -rf $(BIN_DIR)