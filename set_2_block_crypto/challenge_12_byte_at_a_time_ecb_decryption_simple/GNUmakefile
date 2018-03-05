VPATH = \
../challenge_11_ecb_cbc_detection_oracle \
../challenge_10_implement_cbc_mode \
../../set_1_basics/challenge_6_break_repeating_key_xor \
../../set_1_basics/challenge_1_convert_hex_to_base64

SRCS = \
ecb_encryption_oracle.cpp \
generate_random.cpp \
decode_base64.cpp \
cat.cpp \
aes_ecb_encrypt.cpp \
test_byte_at_a_time_ecb_decryption_simple.cpp \
detect_encryption_method.cpp \
byte_at_a_time_ecb_decryption_simple.cpp

LIBS = cryptopp

test_byte_at_a_time_ecb_decryption_simple : $(SRCS:.cpp=.o)

include $(dir $(lastword $(MAKEFILE_LIST)))../../common_settings.mk
