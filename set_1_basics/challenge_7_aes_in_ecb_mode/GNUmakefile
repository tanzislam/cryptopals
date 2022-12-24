VPATH = \
../challenge_4_detect_single_byte_xor_cipher \
../challenge_6_break_repeating_key_xor \
../challenge_1_convert_hex_to_base64

SRCS = \
test_aes_in_ecb_mode.cpp \
download_file_over_https.cpp \
aes_ecb_decrypt.cpp \
decode_base64.cpp

BOOST_LIBS = system

LIBS = cryptopp ssl crypto $(if $(IS_WINDOWS_PLATFORM),crypt32 ws2_32)

test_aes_in_ecb_mode : $(SRCS:.cpp=.o)

include $(dir $(lastword $(MAKEFILE_LIST)))../../common_settings.mk
