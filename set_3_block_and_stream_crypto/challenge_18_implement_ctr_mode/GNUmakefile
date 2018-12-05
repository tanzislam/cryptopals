VPATH = \
../../set_2_block_crypto/challenge_10_implement_cbc_mode \
../../set_1_basics/challenge_6_break_repeating_key_xor \
../../set_1_basics/challenge_1_convert_hex_to_base64 \
../../set_2_block_crypto/challenge_11_ecb_cbc_detection_oracle

SRCS = \
test_ctr_mode.cpp \
aes_ctr.cpp \
aes_ecb_encrypt.cpp \
decode_base64.cpp \
generate_random.cpp

LIBS = cryptopp

test_ctr_mode : $(SRCS:.cpp=.o)

include $(dir $(lastword $(MAKEFILE_LIST)))../../common_settings.mk
