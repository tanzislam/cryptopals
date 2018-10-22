VPATH = \
../../set_2_block_crypto/challenge_11_ecb_cbc_detection_oracle \
../../set_2_block_crypto/challenge_10_implement_cbc_mode \
../../set_2_block_crypto/challenge_9_implement_pkcs7_padding \
../../set_1_basics/challenge_7_aes_in_ecb_mode \
../../set_1_basics/challenge_2_fixed_xor \
../../set_1_basics/challenge_1_convert_hex_to_base64 \
../../set_2_block_crypto/challenge_15_pkcs7_padding_validation \
../../set_1_basics/challenge_6_break_repeating_key_xor

SRCS = \
test_cbc_padding_oracle.cpp \
cbc_padding_oracle.cpp \
generate_random.cpp \
aes_cbc_encrypt.cpp \
aes_ecb_encrypt.cpp \
pkcs7_pad.cpp \
tee.cpp \
xor.cpp \
aes_cbc_decrypt.cpp \
aes_ecb_decrypt.cpp \
pkcs7_unpad.cpp \
attack_cbc_padding_oracle.cpp \
pkcs7_unpad_wrap.cpp \
decode_base64.cpp

LIBS = cryptopp

test_cbc_padding_oracle : $(SRCS:.cpp=.o)

include $(dir $(lastword $(MAKEFILE_LIST)))../../common_settings.mk
