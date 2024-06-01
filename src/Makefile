CXXFLAGS := -Wall -Wextra -std=c++20 -Wshadow -Werror -O2
target := libengine_main.a

sources  := $(wildcard *.cpp)
headers := $(wildcard *.h)
includes := -I../inc/
objects  := $(sources:.cpp=.o)
dep_file := Makefile.dep

.PHONY : all clean

all: $(target)

$(target): $(objects)
	mkdir -p ../build/
	ar rcs ../build/$@ $^

clean :
	$(RM) $(dep_file) $(objects)

.cpp.o :
	$(CXX) $(CXXFLAGS) $(includes) -c $< -o $@

depend $(dep_file):
	@echo Makefile - creating dependencies for: $(sources)
	@$(RM) $(dep_file)
	@$(CXX) -E -MM $(CXXFLAGS) $(includes) $(sources) >> $(dep_file)

ifeq (,$(findstring clean,$(MAKECMDGOALS)))
-include $(dep_file)
endif

clang-tidy:
	clang-tidy $(sources) $(headers) -header-filter='.*' --export-fixes=clang-tidy.yaml > clang-tidy.log 2>&1 || true
