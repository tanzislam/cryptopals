include ../../common_settings.mk
CXXFLAGS += -I../challenge_1_convert_hex_to_base64
VPATH = ../challenge_1_convert_hex_to_base64

test_xor_combine : CC = $(CXX)
test_xor_combine : test_xor_combine.o xor_combine.o decode_hex.o

test_xor_combine.o : test_xor_combine.cpp xor_combine.hpp

xor_combine.o : xor_combine.cpp xor_combine.hpp decode_hex.hpp

decode_hex.o : decode_hex.cpp decode_hex.hpp