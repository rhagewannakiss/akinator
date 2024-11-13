ifeq ($(origin CC),default)
  CC = g++
endif

CFLAGS =  -c -fdiagnostics-generate-patch -fdiagnostics-path-format=inline-events             \
          -Og -ggdb -std=c++20 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations         \
	      -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts    \
	      -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body             \
	      -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness             \
	      -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd                   \
	      -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls         \
	      -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel                      \
	      -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods            \
	      -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum             \
	      -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast                      \
	      -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers               \
	      -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector                   \
	      -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow                \
	      -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla               \
	      -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,$\
	       float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,$\
	       null,object-size,return,returns-nonnull-attribute,shift,$\
	       signed-integer-overflow,undefined,unreachable,vla-bound,vptr

OUT_O_DIR ?= build
COMMONINC = -I./include
SRC = ./source
ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

override CFLAGS += $(COMMONINC)

CSRC = main_akinator.cpp akinator.cpp game_modes.cpp str_cmp.cpp

# Reproducing source tree in object tree
COBJ := $(addprefix $(OUT_O_DIR)/,$(CSRC:.cpp=.o))
DEPS = $(COBJ:.o=.d)

.PHONY: all
all: $(OUT_O_DIR)/akinator.x

$(OUT_O_DIR)/akinator.x: $(COBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

# Static pattern rule to not redefine generic one
$(COBJ) : $(OUT_O_DIR)/%.o : %.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(DEPS) : $(OUT_O_DIR)/%.d : %.cpp
	@mkdir -p $(@D)
	$(CC) -E $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@

.PHONY: clean
clean:
	rm -rf $(COBJ) $(DEPS) $(OUT_O_DIR)/*.x $(OUT_O_DIR)/*.log

# Targets which we have no need to recollect deps
NODEPS = clean

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
include $(DEPS)
endif
