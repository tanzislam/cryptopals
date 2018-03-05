VPATH = \
../challenge_11_ecb_cbc_detection_oracle \
../challenge_9_implement_pkcs7_padding \
../challenge_10_implement_cbc_mode \
../../set_1_basics/challenge_7_aes_in_ecb_mode \
../../set_1_basics/challenge_1_convert_hex_to_base64

SRCS = \
key_value_parser.cpp \
test_ecb_cut_and_paste.cpp \
encode_user_profile.cpp \
user_profile_server.cpp \
generate_random.cpp \
pkcs7_pad.cpp \
aes_ecb_encrypt.cpp \
aes_ecb_decrypt.cpp \
forge_admin_profile.cpp

LIBS = cryptopp

test_ecb_cut_and_paste : $(SRCS:.cpp=.o)

include $(dir $(lastword $(MAKEFILE_LIST)))../../common_settings.mk
