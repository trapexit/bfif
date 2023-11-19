COMPILER_PREFIX =
PLATFORM := $(shell uname -s | tr A-Z a-z)_$(shell arch)
EXE = bfif

JOBS := $(shell nproc)
PUID := $(shell id -u)
PGID := $(shell id -g)

OUTPUT = build/$(EXE)

CC    = $(COMPILER_PREFIX)-gcc
CXX   = $(COMPILER_PREFIX)-g++
STRIP = $(COMPILER_PREFIX)-strip

ifeq ($(DEBUG),1)
OPT := -O0 -ggdb
else
OPT := -O3 -flto -static
endif

ifeq ($(SANITIZE),1)
OPT += -fsanitize=undefined
endif

CFLAGS = $(OPT) -Wall
CXXFLAGS = $(OPT) -Wall -std=c++17
CPPFLAGS ?= -MMD -MP

SRCS_C   := $(wildcard src/*.c)
SRCS_CXX := $(wildcard src/*.cpp)

BUILDDIR = build/$(PLATFORM)
OBJS := $(SRCS_C:src/%.c=$(BUILDDIR)/%.c.o)
OBJS += $(SRCS_CXX:src/%.cpp=$(BUILDDIR)/%.cpp.o)
DEPS  = $(OBJS:.o=.d)


all: $(OUTPUT)

$(OUTPUT): builddir $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(OUTPUT) $(OBJS) $(LDFLAGS)

strip: $(OUTPUT)
	$(STRIP) --strip-all $(OUTPUT)

$(BUILDDIR)/%.c.o: src/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.cpp.o: src/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rfv build/

builddir:
	mkdir -p $(BUILDDIR)

linux-release:
	$(MAKE) -j$(JOBS) EXE=bfif_$(PLATFORM) strip

win-i686-release:
	$(MAKE) -j$(JOBS) COMPILER_PREFIX=i686-w64-mingw32 PLATFORM=win_i686 EXE=bfif_win_i686.exe strip

win-x86_64-release:
	$(MAKE) -j$(JOBS) COMPILER_PREFIX=x86_64-w64-mingw32 PLATFORM=win_x86_64 EXE=bfif_win_x86_64.exe strip

docker-release:
	docker run --rm -it -e PUID=$(PUID) -e PGID=$(PGID) -v ${PWD}:/src alpine:edge "/src/tools/docker-make-release"


.PHONY: clean builddir release docker-release

-include $(DEPS)
