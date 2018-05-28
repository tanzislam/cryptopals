VPATH = \
../challenge_10_implement_cbc_mode \
../../set_1_basics/challenge_1_convert_hex_to_base64

SRCS = \
test_pkcs7_padding_validation.cpp \
pkcs7_unpad_wrap.cpp \
pkcs7_unpad.cpp

test_pkcs7_padding_validation : $(SRCS:.cpp=.o)

include $(dir $(lastword $(MAKEFILE_LIST)))../../common_settings.mk
