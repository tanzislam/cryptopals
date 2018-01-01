VPATH = \
../challenge_2_fixed_xor \
../challenge_1_convert_hex_to_base64

SRCS = \
test_implement_repeating_key_xor_cipher.cpp \
encode_hex.cpp \
xor.cpp \
repeating_stringstream.cpp

test_implement_repeating_key_xor_cipher : $(SRCS:.cpp=.o)

include $(dir $(lastword $(MAKEFILE_LIST)))../../common_settings.mk
