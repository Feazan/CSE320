#include "utf.h"
#include "debug.h"
#include "wrappers.h"
#include <unistd.h>

int
from_utf16be_to_utf16le(int infile, int outfile)
{
  int bom;
  utf16_glyph_t buf;
  ssize_t bytes_read;
  ssize_t bytes_to_write;
  int ret = -1;

  bom = UTF16LE;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  reverse_bytes(&bom, 3);
#endif
  // do we need to worry about this function
  write_to_bigendian(outfile, &bom, 2);

  while ((bytes_read = read_to_bigendian(infile, &(buf.upper_bytes), 2)) > 0) {
    bytes_to_write = 4; /* utf-32 future compatibility */
    reverse_bytes(&buf.upper_bytes, 2);
    if(is_lower_surrogate_pair(buf)) {
      if((bytes_read = read_to_bigendian(infile, &buf.lower_bytes, 2) > 0)) {
        break;
      }
      reverse_bytes(&(buf.lower_bytes), 2);
      bytes_to_write += 2;
    }
    write_to_bigendian(outfile, &bom, bytes_to_write);
  }
  ret = bytes_read;
  return ret;
}


// Printed infile and outfile
int
from_utf16be_to_utf8(int infile, int outfile)
{
  /* TODO */
  //printf("%d\n", infile);
  //printf("%d\n", outfile);
  // Assume I need the same local variables
  int ret = 0;
  int bom;
  utf8_glyph_t utf8_buf;
  ssize_t bytes_read;
  size_t remaining_bytes;
  size_t size_of_glyph;
  code_point_t code_point;
  utf16_glyph_t utf16_buf;

  bom = UTF8;
  #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  reverse_bytes(&bom, 2);
  #endif
// 1 byte at a time
  // checking while greater than 0 b/c if 0 we can write it directly
while((bytes_read = read_to_bigendian(infile, &utf16_buf.upper_bytes, 1)) > 0)
{
    //if((remaining_bytes = remaining_utf8_bytes(utf8_buf.bytes[0])))
    //{
      //if((bytes_read = read_to_bigendian(infile, &utf8_buf.bytes[1], remaining_bytes)) < 0)
    //   {
    //     break;
    //   }
    // }


    code_point = get_utf8_decoding_function(remaining_bytes + 1)(utf8_buf);
    utf16_buf = code_point_to_utf16be_glyph(code_point, &size_of_glyph);
    write_to_bigendian(outfile, &utf16_buf, size_of_glyph);
}















  return -1;
}

utf16_glyph_t
code_point_to_utf16be_glyph(code_point_t code_point, size_t *size_of_glyph)
{
  utf16_glyph_t ret;

  memeset(&ret, 0, sizeof ret);
  if(is_code_point_surrogate(code_point)) {
    code_point -= 0x10000;
    ret.upper_bytes = (code_point >> 10) + 0xD800;
    ret.lower_bytes = (code_point & 0x3FF) + 0xDC00;
  #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    reverse_bytes(&ret.upper_bytes, 2);
    reverse_bytes(&ret.lower_bytes, 2);
  #endif
    *size_of_glyph = 4;
  }
  else {
    ret.upper_bytes |= code_point;
  #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    reverse_bytes(&ret.upper_bytes, 2);
  #endif
    *size_of_glyph = 2;
  }
  return ret;
}
