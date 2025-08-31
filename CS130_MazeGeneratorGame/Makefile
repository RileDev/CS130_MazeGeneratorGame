# ---- Config ----
TARGET := game
SRC_DIR := .
INC_DIRS := . include

CXX := g++                # force g++
CXXSTD := -std=c++20      # << C++20 here
WARN := -Wall -Wextra -Wpedantic
OPT := -O2
DEFS :=

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

PKG ?= pkg-config
RL_CFLAGS := $(shell $(PKG) --cflags raylib)
RL_LDFLAGS := $(shell $(PKG) --libs raylib)

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(RL_LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXSTD) $(WARN) $(OPT) $(DEFS) $(INC_FLAGS) $(RL_CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
