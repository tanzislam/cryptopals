VPATH = \
../challenge_1_convert_hex_to_base64 \
../challenge_2_fixed_xor

SRCS = \
test_break_single_byte_xor_cipher.cpp \
repeating_stringstream.cpp \
break_single_byte_xor_cipher.cpp \
break_single_byte_xor_cipher_char_frequency.cpp \
break_single_byte_xor_cipher_recognize_words.cpp \
hunspell_singleton.cpp \
xor_combine.cpp \
decode_hex.cpp \
encode_hex.cpp

LIBS = hunspell-1.5

test_break_single_byte_xor_cipher : $(SRCS:.cpp=.o)

include ../../common_settings.mk
