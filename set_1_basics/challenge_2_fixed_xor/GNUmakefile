VPATH = ../challenge_1_convert_hex_to_base64

SRCS = \
test_xor_combine.cpp \
xor_combine.cpp \
decode_hex.cpp \
encode_hex.cpp \
xor.cpp

test_xor_combine : $(SRCS:.cpp=.o)

include $(dir $(lastword $(MAKEFILE_LIST)))../../common_settings.mk
