ifeq "" "$(BOOST_DIR)"
$(error The BOOST_DIR variable must be set to where you installed Boost v1.61.0)
else ifeq "" "$(GTEST_DIR)"
$(error The GTEST_DIR variable must be set to where you downloaded or cloned\
        Google Test and Google Mock)
endif

CXX = g++
CPPFLAGS = -std=c++11 -Wc++11-compat -Wc++14-compat -pedantic -Wall -Wextra -O9
CXXFLAGS = -I$(BOOST_DIR) -I$(GTEST_DIR)/googletest/include
LDLIBS = $(GTEST_DIR)/googletest/make/gtest_main.a -lpthread

tests : CC=$(CXX)
tests : tests.o decode_hex.o encode_base64.o convert_hex_to_base64.o
tests.o : tests.cpp decode_hex.hpp encode_base64.hpp convert_hex_to_base64.hpp
decode_hex.o : decode_hex.cpp decode_hex.hpp
encode_base64.o : encode_base64.cpp encode_base64.hpp
convert_hex_to_base64.o : convert_hex_to_base64.hpp \
                          decode_hex.hpp encode_base64.hpp

.PHONY : clean
clean :
	rm -f *.o *.exe