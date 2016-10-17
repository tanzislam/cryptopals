VPATH = \
../challenge_3_single_byte_xor_cipher \
../challenge_1_convert_hex_to_base64

SRCS = \
test_detect_single_byte_xor_cipher.cpp \
line_extract_streambuf.cpp \
decode_hex.cpp \
detect_single_byte_xor_cipher.cpp \
break_single_byte_xor_cipher.cpp \
break_single_byte_xor_cipher_recognize_words.cpp \
hunspell_singleton.cpp

BOOST_LIBS = system

LIBS = hunspell-1.5

test_detect_single_byte_xor_cipher : $(SRCS:.cpp=.o)

include ../../common_settings.mk
