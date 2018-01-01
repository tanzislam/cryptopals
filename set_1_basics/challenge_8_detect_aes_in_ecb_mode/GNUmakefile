VPATH = \
../challenge_4_detect_single_byte_xor_cipher \
../challenge_1_convert_hex_to_base64

SRCS = \
test_detect_aes_in_ecb_mode.cpp \
detect_aes_in_ecb_mode.cpp \
detect_repeated_block.cpp \
line_extract_streambuf.cpp \
decode_hex.cpp

BOOST_LIBS = system

test_detect_aes_in_ecb_mode : $(SRCS:.cpp=.o)

include $(dir $(lastword $(MAKEFILE_LIST)))../../common_settings.mk
