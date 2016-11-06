VPATH = \
../challenge_6_break_repeating_key_xor \
../challenge_1_convert_hex_to_base64

SRCS = \
test_aes_in_ecb_mode.cpp \
aes_ecb_decrypt.cpp \
decode_base64.cpp

BOOST_LIBS = system

LIBS = cryptopp

test_aes_in_ecb_mode : $(SRCS:.cpp=.o)

include ../../common_settings.mk
