VPATH = \
../challenge_10_implement_cbc_mode \
../challenge_9_implement_pkcs7_padding \
../../set_1_basics/challenge_2_fixed_xor \
../../set_1_basics/challenge_1_convert_hex_to_base64

SRCS = \
test_encryption_oracle.cpp \
generate_random.cpp \
cat.cpp \
encryption_oracle.cpp \
detect_encryption_method.cpp \
aes_ecb_encrypt.cpp \
aes_cbc_encrypt.cpp \
pkcs7_pad.cpp \
xor.cpp \
tee.cpp

LIBS = cryptopp

test_encryption_oracle : $(SRCS:.cpp=.o)

include $(dir $(lastword $(MAKEFILE_LIST)))../../common_settings.mk
