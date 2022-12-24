VPATH = \
../../set_1_basics/challenge_7_aes_in_ecb_mode \
../../set_1_basics/challenge_2_fixed_xor \
../../set_1_basics/challenge_6_break_repeating_key_xor \
../../set_1_basics/challenge_4_detect_single_byte_xor_cipher \
../../set_1_basics/challenge_1_convert_hex_to_base64 \
../challenge_9_implement_pkcs7_padding

SRCS = \
test_implement_cbc_mode.cpp \
aes_ecb_encrypt.cpp \
aes_ecb_decrypt.cpp \
aes_cbc_decrypt.cpp \
tee.cpp \
xor.cpp \
decode_base64.cpp \
pkcs7_unpad.cpp \
aes_cbc_encrypt.cpp \
pkcs7_pad.cpp \
download_file_over_https.cpp

BOOST_LIBS = system

LIBS = cryptopp ssl crypto $(if $(IS_WINDOWS_PLATFORM),crypt32 ws2_32)

test_implement_cbc_mode : $(SRCS:.cpp=.o)

include $(dir $(lastword $(MAKEFILE_LIST)))../../common_settings.mk
