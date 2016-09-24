VPATH = \
../challenge_3_single_byte_xor_cipher \
../challenge_2_fixed_xor \
../challenge_1_convert_hex_to_base64

SRCS = \
test_detect_single_byte_xor_cipher.cpp \
line_extract_streambuf.cpp \
detect_single_byte_xor_cipher.cpp \
break_single_byte_xor_cipher.cpp \
break_single_byte_xor_cipher_recognize_words.cpp \
hunspell_singleton.cpp \
decode_hex.cpp \
repeating_stringstream.cpp \
encode_hex.cpp \
xor_combine.cpp

BOOST_LIBS = system

LIBS = hunspell-1.5

test_detect_single_byte_xor_cipher : $(SRCS:.cpp=.o)

include ../../common_settings.mk
