VPATH = ../challenge_1_convert_hex_to_base64

SRCS = \
test_break_single_byte_xor_cipher.cpp \
break_single_byte_xor_cipher.cpp \
break_single_byte_xor_cipher_char_frequency.cpp \
break_single_byte_xor_cipher_recognize_words.cpp \
hunspell_singleton.cpp \
decode_hex.cpp \
levenshtein_distance.cpp

LIBS = hunspell-1.6

test_break_single_byte_xor_cipher : $(SRCS:.cpp=.o)

include ../../common_settings.mk
