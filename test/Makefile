target := judgetest

CXXFLAGS := -Wall -Wextra -std=c++20 -Wshadow -Werror -g -fprofile-arcs -ftest-coverage

LDLIBS := -lgtest -lgtest_main -lpthread -lgcov

sources  := playerstream_test.cpp ../src/playerstream.cpp ../src/err.cpp
includes := -I../inc
objects  := $(sources:.cpp=.o)
dep_file := Makefile.dep

.PHONY : all clean

all: run

run : $(target)
	./$(target)

$(target) : $(objects)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean :
	$(RM) $(target) $(dep_file) $(objects)

.cpp.o :
	$(CXX) $(CXXFLAGS) $(includes) -c $< -o $@

depend $(dep_file):
	@echo Makefile - creating dependencies for: $(sources)
	@$(RM) $(dep_file)
	@$(CXX) -E -MM $(CXXFLAGS) $(includes) $(sources) >> $(dep_file)

ifeq (,$(findstring clean,$(MAKECMDGOALS)))
-include $(dep_file)
endif
