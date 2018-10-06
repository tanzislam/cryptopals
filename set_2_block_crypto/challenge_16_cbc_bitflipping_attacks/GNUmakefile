VPATH = \
../challenge_14_byte_at_a_time_ecb_decryption_harder \
../challenge_13_ecb_cut_and_paste \
../challenge_11_ecb_cbc_detection_oracle \
../challenge_10_implement_cbc_mode \
../challenge_9_implement_pkcs7_padding \
../../set_1_basics/challenge_7_aes_in_ecb_mode \
../../set_1_basics/challenge_2_fixed_xor \
../../set_1_basics/challenge_1_convert_hex_to_base64

SRCS = \
test_cbc_bitflipping_attacks.cpp \
server.cpp \
generate_random.cpp \
aes_cbc_encrypt.cpp \
aes_cbc_decrypt.cpp \
key_value_parser.cpp \
pkcs7_pad.cpp \
pkcs7_unpad.cpp \
xor.cpp \
tee.cpp \
aes_ecb_encrypt.cpp \
aes_ecb_decrypt.cpp \
forge_admin_profile_cbc.cpp \
byte_at_a_time_ecb_decryption_with_prefix.cpp

LIBS = cryptopp

test_cbc_bitflipping_attacks : $(SRCS:.cpp=.o)

include $(dir $(lastword $(MAKEFILE_LIST)))../../common_settings.mk
