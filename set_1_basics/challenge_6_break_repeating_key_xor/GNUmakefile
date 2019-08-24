VPATH = \
../challenge_1_convert_hex_to_base64 \
../challenge_2_fixed_xor \
../challenge_3_single_byte_xor_cipher \
../challenge_4_detect_single_byte_xor_cipher \
../challenge_5_implement_repeating_key_xor_cipher

SRCS = \
test_break_repeating_key_xor.cpp \
hamming_distance.cpp \
decode_base64.cpp \
download_file_over_https.cpp \
break_repeating_key_xor_cipher.cpp \
skip_chars.cpp \
break_single_byte_xor_cipher.cpp \
repeating_stringstream.cpp \
xor.cpp \
break_single_byte_xor_cipher_char_frequency.cpp \
levenshtein_distance.cpp

BOOST_LIBS = system

LIBS = ssl crypto

test_break_repeating_key_xor : $(SRCS:.cpp=.o)

include $(dir $(lastword $(MAKEFILE_LIST)))../../common_settings.mk
