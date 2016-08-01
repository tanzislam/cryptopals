include ../../common_settings.mk

test_hex2base64 : CC = $(CXX)
test_hex2base64 : test_hex2base64.o \
                  decode_hex.o encode_base64.o convert_hex_to_base64.o

test_hex2base64.o : test_hex2base64.cpp \
                    decode_hex.hpp encode_base64.hpp convert_hex_to_base64.hpp

decode_hex.o : decode_hex.cpp decode_hex.hpp

encode_base64.o : encode_base64.cpp encode_base64.hpp

convert_hex_to_base64.o : convert_hex_to_base64.hpp \
                          decode_hex.hpp encode_base64.hpp
