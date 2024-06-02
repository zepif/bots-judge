target := rsp_engine

CXXFLAGS := -Wall -Wextra -std=c++20 -Wshadow -Werror -O2

sources  := $(wildcard *.cpp)
includes := -I../../inc/
objects  := $(sources:.cpp=.o)
dep_file := Makefile.dep

.PHONY : all clean

all: $(target) noop scissors rock random

noop: botnoop/botnoop.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

rock: botrock/botrock.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

scissors: botscissors/botscissors.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

random: botrandom/botrandom.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

rsp_engine: $(objects) ../../build/libengine_main.a
	$(CXX) $(CXXFLAGS) -o $@ $^

clean :
	$(RM) $(target) $(dep_file) $(objects) noop rock scissors random

.cpp.o :
	$(CXX) $(CXXFLAGS) $(includes) -c $< -o $@

depend $(dep_file):
	@echo Makefile - creating dependencies for: $(sources)
	@$(RM) $(dep_file)
	@$(CXX) -E -MM $(CXXFLAGS) $(includes) $(sources) >> $(dep_file)

ifeq (,$(findstring clean,$(MAKECMDGOALS)))
-include $(dep_file)
endif
