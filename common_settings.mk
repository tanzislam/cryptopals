ifeq "" "$(BOOST_DIR)"
$(error The BOOST_DIR variable must be set to where you installed Boost v1.61.0)
else ifeq "" "$(GTEST_DIR)"
$(error The GTEST_DIR variable must be set to where you downloaded or cloned\
        Google Test and Google Mock)
endif

CXX = g++
CPPFLAGS = -std=c++11 -Wc++11-compat -Wc++14-compat -pedantic -Wall -Wextra -O9
CXXFLAGS = -isystem $(BOOST_DIR) -isystem $(GTEST_DIR)/googletest/include
LDLIBS = $(GTEST_DIR)/googletest/make/gtest_main.a -lpthread

.PHONY : clean
clean :
	rm -f *.o *.exe

.DEFAULT_GOAL =
