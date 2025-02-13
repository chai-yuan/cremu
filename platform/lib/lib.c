#include "debug.h"
#include "machine/spike.h"
#include "string.h"

u8                  sram_data[128 * 1024 * 1024];
struct SpikeMachine machine;
bool                interrupt = false;
const unsigned char dtb[]     = {
    0xd0, 0x0d, 0xfe, 0xed, 0x00, 0x00, 0x06, 0x6a, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x05, 0x1c, 0x00, 0x00, 0x00,
    0x28, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x4e, 0x00, 0x00,
    0x04, 0xe4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x1b, 0x75, 0x63, 0x62, 0x62, 0x61, 0x72,
    0x2c, 0x73, 0x70, 0x69, 0x6b, 0x65, 0x2d, 0x62, 0x61, 0x72, 0x65, 0x2d, 0x64, 0x65, 0x76, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x26, 0x75, 0x63, 0x62, 0x62, 0x61, 0x72, 0x2c, 0x73,
    0x70, 0x69, 0x6b, 0x65, 0x2d, 0x62, 0x61, 0x72, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x63, 0x68, 0x6f,
    0x73, 0x65, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x2c, 0x2f, 0x73,
    0x6f, 0x63, 0x2f, 0x6e, 0x73, 0x31, 0x36, 0x35, 0x35, 0x30, 0x40, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x38, 0x63, 0x6f, 0x6e, 0x73,
    0x6f, 0x6c, 0x65, 0x3d, 0x74, 0x74, 0x79, 0x53, 0x30, 0x20, 0x65, 0x61, 0x72, 0x6c, 0x79, 0x63, 0x6f, 0x6e, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x63, 0x70, 0x75, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00,
    0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x00, 0x00, 0x41, 0x00, 0x98, 0x96, 0x80, 0x00, 0x00, 0x00, 0x01, 0x63, 0x70, 0x75, 0x40, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x54, 0x63, 0x70, 0x75, 0x00, 0x00, 0x00,
    0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00,
    0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x64, 0x6f, 0x6b, 0x61, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x1b, 0x72, 0x69, 0x73, 0x63, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x6b, 0x72, 0x76, 0x36, 0x34, 0x69, 0x6d, 0x61, 0x63, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x75, 0x72, 0x69, 0x73, 0x63, 0x76,
    0x2c, 0x73, 0x76, 0x35, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x7e,
    0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x8f, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xa4, 0x3b, 0x9a, 0xca, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x72, 0x75, 0x70, 0x74, 0x2d, 0x63, 0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c,
    0x6c, 0x65, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xb4, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc5, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x1b, 0x72, 0x69, 0x73, 0x63, 0x76, 0x2c, 0x63, 0x70, 0x75, 0x2d, 0x69, 0x6e, 0x74,
    0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xda, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x6d, 0x65, 0x6d,
    0x6f, 0x72, 0x79, 0x40, 0x38, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x07, 0x00, 0x00, 0x00, 0x54, 0x6d, 0x65, 0x6d, 0x6f, 0x72, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00,
    0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x73, 0x6f, 0x63, 0x00, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x04, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x21, 0x00,
    0x00, 0x00, 0x1b, 0x75, 0x63, 0x62, 0x62, 0x61, 0x72, 0x2c, 0x73, 0x70, 0x69, 0x6b, 0x65, 0x2d, 0x62, 0x61, 0x72,
    0x65, 0x2d, 0x73, 0x6f, 0x63, 0x00, 0x73, 0x69, 0x6d, 0x70, 0x6c, 0x65, 0x2d, 0x62, 0x75, 0x73, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe2, 0x00, 0x00, 0x00, 0x01, 0x63, 0x6c,
    0x69, 0x6e, 0x74, 0x40, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00,
    0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x1b, 0x72, 0x69, 0x73, 0x63, 0x76, 0x2c, 0x63, 0x6c, 0x69, 0x6e, 0x74, 0x30,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xe9, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x10, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x70, 0x6c, 0x69, 0x63, 0x40, 0x63, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
    0x1b, 0x72, 0x69, 0x73, 0x63, 0x76, 0x2c, 0x70, 0x6c, 0x69, 0x63, 0x30, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x10, 0x00,
    0x00, 0x00, 0xe9, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x09,
    0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
    0x00, 0xfd, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x01, 0x08, 0x00,
    0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xb4, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc5, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0xda, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x6e, 0x73,
    0x31, 0x36, 0x35, 0x35, 0x30, 0x40, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x1b, 0x6e, 0x73, 0x31, 0x36, 0x35, 0x35, 0x30, 0x61,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xa4, 0x00, 0x98, 0x96,
    0x80, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x01, 0x1b, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
    0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x01, 0x2c, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00,
    0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x01, 0x37, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x01, 0x41, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
    0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x68, 0x74, 0x69, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x1b, 0x75, 0x63, 0x62, 0x2c, 0x68, 0x74, 0x69, 0x66,
    0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x09, 0x23, 0x61, 0x64,
    0x64, 0x72, 0x65, 0x73, 0x73, 0x2d, 0x63, 0x65, 0x6c, 0x6c, 0x73, 0x00, 0x23, 0x73, 0x69, 0x7a, 0x65, 0x2d, 0x63,
    0x65, 0x6c, 0x6c, 0x73, 0x00, 0x63, 0x6f, 0x6d, 0x70, 0x61, 0x74, 0x69, 0x62, 0x6c, 0x65, 0x00, 0x6d, 0x6f, 0x64,
    0x65, 0x6c, 0x00, 0x73, 0x74, 0x64, 0x6f, 0x75, 0x74, 0x2d, 0x70, 0x61, 0x74, 0x68, 0x00, 0x62, 0x6f, 0x6f, 0x74,
    0x61, 0x72, 0x67, 0x73, 0x00, 0x74, 0x69, 0x6d, 0x65, 0x62, 0x61, 0x73, 0x65, 0x2d, 0x66, 0x72, 0x65, 0x71, 0x75,
    0x65, 0x6e, 0x63, 0x79, 0x00, 0x64, 0x65, 0x76, 0x69, 0x63, 0x65, 0x5f, 0x74, 0x79, 0x70, 0x65, 0x00, 0x72, 0x65,
    0x67, 0x00, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x00, 0x72, 0x69, 0x73, 0x63, 0x76, 0x2c, 0x69, 0x73, 0x61, 0x00,
    0x6d, 0x6d, 0x75, 0x2d, 0x74, 0x79, 0x70, 0x65, 0x00, 0x72, 0x69, 0x73, 0x63, 0x76, 0x2c, 0x70, 0x6d, 0x70, 0x72,
    0x65, 0x67, 0x69, 0x6f, 0x6e, 0x73, 0x00, 0x72, 0x69, 0x73, 0x63, 0x76, 0x2c, 0x70, 0x6d, 0x70, 0x67, 0x72, 0x61,
    0x6e, 0x75, 0x6c, 0x61, 0x72, 0x69, 0x74, 0x79, 0x00, 0x63, 0x6c, 0x6f, 0x63, 0x6b, 0x2d, 0x66, 0x72, 0x65, 0x71,
    0x75, 0x65, 0x6e, 0x63, 0x79, 0x00, 0x23, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x72, 0x75, 0x70, 0x74, 0x2d, 0x63, 0x65,
    0x6c, 0x6c, 0x73, 0x00, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x72, 0x75, 0x70, 0x74, 0x2d, 0x63, 0x6f, 0x6e, 0x74, 0x72,
    0x6f, 0x6c, 0x6c, 0x65, 0x72, 0x00, 0x70, 0x68, 0x61, 0x6e, 0x64, 0x6c, 0x65, 0x00, 0x72, 0x61, 0x6e, 0x67, 0x65,
    0x73, 0x00, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x72, 0x75, 0x70, 0x74, 0x73, 0x2d, 0x65, 0x78, 0x74, 0x65, 0x6e, 0x64,
    0x65, 0x64, 0x00, 0x72, 0x69, 0x73, 0x63, 0x76, 0x2c, 0x6e, 0x64, 0x65, 0x76, 0x00, 0x72, 0x69, 0x73, 0x63, 0x76,
    0x2c, 0x6d, 0x61, 0x78, 0x2d, 0x70, 0x72, 0x69, 0x6f, 0x72, 0x69, 0x74, 0x79, 0x00, 0x69, 0x6e, 0x74, 0x65, 0x72,
    0x72, 0x75, 0x70, 0x74, 0x2d, 0x70, 0x61, 0x72, 0x65, 0x6e, 0x74, 0x00, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x72, 0x75,
    0x70, 0x74, 0x73, 0x00, 0x72, 0x65, 0x67, 0x2d, 0x73, 0x68, 0x69, 0x66, 0x74, 0x00, 0x72, 0x65, 0x67, 0x2d, 0x69,
    0x6f, 0x2d, 0x77, 0x69, 0x64, 0x74, 0x68, 0x00};

struct RiscvCore *difftest_init(const u8 *data, u64 data_size) {
    INFO("difftest_init");
    memcpy(sram_data, data, data_size);
    INFO("load data size : %lld", data_size);
    spike_machine_init(&machine, (struct SpikePortableOperations){
                                     .sram_data = sram_data,
                                     .sram_size = 128 * 1024 * 1024,
                                     .dtb_data  = dtb,
                                     .dtb_size  = sizeof(dtb),
                                     .get_char  = NULL,
                                     .put_char  = NULL,
                                 });

    return &machine.core;
}

void difftest_step(void) {
    riscvcore_step(&machine.core, (struct RiscvEnvInfo){.eint = false});
    interrupt = false;
}
