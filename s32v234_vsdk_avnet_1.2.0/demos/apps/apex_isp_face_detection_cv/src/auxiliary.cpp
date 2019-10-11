#include <auxiliary.h>

void DrawRectangle(unsigned char* dst,
                   int            x,
                   int            y,
                   int            width,
                   int            height,
                   unsigned char  cR,
                   unsigned char  cG,
                   unsigned char  cB)
{
  int offx11 = x * 3;
  int offx12 = (x + 1) * 3;
  int offx13 = (x - 1) * 3;
  int offx21 = (x + width) * 3;
  int offx22 = (x + 1 + width) * 3;
  int offx23 = (x - 1 + width) * 3;
  for(int cy = y; cy < y + height; ++cy)
  {
    int offy = cy * 1280 * 3;
    if(cy > 1 && cy < 719)
    {
      if(x > 1 && x < 1279)
      {
        dst[offy + offx13]     = cR;
        dst[offy + offx13 + 1] = cG;
        dst[offy + offx13 + 2] = cB;
        dst[offy + offx11]     = cR;
        dst[offy + offx11 + 1] = cG;
        dst[offy + offx11 + 2] = cB;
        dst[offy + offx12]     = cR;
        dst[offy + offx12 + 1] = cG;
        dst[offy + offx12 + 2] = cB;
      }
      if((x + width) > 0 && (x + width) < 1280)
      {
        dst[offy + offx23]     = cR;
        dst[offy + offx23 + 1] = cG;
        dst[offy + offx23 + 2] = cB;
        dst[offy + offx21]     = cR;
        dst[offy + offx21 + 1] = cG;
        dst[offy + offx21 + 2] = cB;
        dst[offy + offx22]     = cR;
        dst[offy + offx22 + 1] = cG;
        dst[offy + offx22 + 2] = cB;
      }
    }
  }
  int offy11 = y * 1280 * 3;
  int offy12 = (y + 1) * 1280 * 3;
  int offy13 = (y - 1) * 1280 * 3;
  int offy21 = (y + height) * 1280 * 3;
  int offy22 = (y + height + 1) * 1280 * 3;
  int offy23 = (y + height - 1) * 1280 * 3;

  for(int cx = x; cx < x + width; ++cx)
  {
    int offx = cx * 3;
    if(cx > 1 && cx < 1279)
    {
      if(y > 1 && y < 719)
      {
        dst[offy13 + offx]     = cR;
        dst[offy13 + offx + 1] = cG;
        dst[offy13 + offx + 2] = cB;
        dst[offy11 + offx]     = cR;
        dst[offy11 + offx + 1] = cG;
        dst[offy11 + offx + 2] = cB;
        dst[offy12 + offx]     = cR;
        dst[offy12 + offx + 1] = cG;
        dst[offy12 + offx + 2] = cB;
      }
      if((y + height) > 1 && (y + height) < 719)
      {
        dst[offy23 + offx]     = cR;
        dst[offy23 + offx + 1] = cG;
        dst[offy23 + offx + 2] = cB;
        dst[offy21 + offx]     = cR;
        dst[offy21 + offx + 1] = cG;
        dst[offy21 + offx + 2] = cB;
        dst[offy22 + offx]     = cR;
        dst[offy22 + offx + 1] = cG;
        dst[offy22 + offx + 2] = cB;
      }
    }
  }
}

void Resample(char* dst, char* src, int newWidth, int newHeight, int width, int height)
{
  double scaleWidth  = (double)newWidth / (double)width;
  double scaleHeight = (double)newHeight / (double)height;

  for(int cy = 0; cy < newHeight; cy++)
  {
    for(int cx = 0; cx < newWidth; cx++)
    {
      int pixel        = (cy * (newWidth * 3)) + (cx * 3);
      int nearestMatch = (((int)(cy / scaleHeight) * (width * 3)) + ((int)(cx / scaleWidth) * 3));

      dst[pixel]     = src[nearestMatch];
      dst[pixel + 1] = src[nearestMatch + 1];
      dst[pixel + 2] = src[nearestMatch + 2];
    }
  }
}

void ResampleG(char* dst, char* src, int newWidth, int newHeight, int width, int height)
{
  double scaleWidth  = (double)newWidth / (double)width;
  double scaleHeight = (double)newHeight / (double)height;

  for(int cy = 0; cy < newHeight; cy++)
  {
    for(int cx = 0; cx < newWidth; cx++)
    {
      int pixel        = (cy * (newWidth)) + (cx);
      int nearestMatch = (((int)(cy / scaleHeight) * (width)) + ((int)(cx / scaleWidth)));

      dst[pixel] = src[nearestMatch];
    }
  }
}

void GrayScale(unsigned char* dst, unsigned char* src, int width, int height)
{
  for(int cy = 0; cy < height; cy++)
  {
    for(int cx = 0; cx < width; cx++)
    {
      int pixel                  = (cy * (width * 3)) + (cx * 3);
      dst[(cy * (width)) + (cx)] = 0.1140 * src[pixel] + 0.5870 * src[pixel + 1] + 0.2989 * src[pixel + 2];
    }
  }
}

//***************************************************************************

void neon_memcpy_1280_to_hd(char* dst, char* src)
{
  char* dst_local         = dst;
  char* src_local         = src;
  long  line_size         = (1280 * 3) / 24;
  long  line_length       = (1280 * 3);
  long  line_iterations   = line_size;
  long  height_iterations = 360;
  long  stride_dst        = 640 * 3;

  __asm volatile(
      "1: \n\t"
      "PRFM PLDL1KEEP, [%[src_local], #-24] \n\t"
      "LD3 {V0.8B, V1.8B, V2.8B}, [%[src_local]], #24   \n\t"
      "ST3 {V0.B, V1.B, V2.B}[0], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[1], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[1], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[2], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[3], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[3], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[4], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[5], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[5], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[6], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[7], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[7], [%[dst_local]], #3                \n\t"

      //"DSB SY\n\t"
      "subs %[line_iterations],%[line_iterations],#1 \n\t"
      "bne 1b \n\t"

      "subs %[src_local],%[src_local], %[line_length] \n\t"
      "mov %[line_iterations], %[line_size] \n\t"

      "2: \n\t"
      "PRFM PLDL1KEEP, [%[src_local], #-24] \n\t"
      "LD3 {V0.8B, V1.8B, V2.8B}, [%[src_local]], #24   \n\t"
      "ST3 {V0.B, V1.B, V2.B}[0], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[1], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[1], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[2], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[3], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[3], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[4], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[5], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[5], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[6], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[7], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[7], [%[dst_local]], #3                \n\t"

      //"DSB SY\n\t"
      "subs %[line_iterations],%[line_iterations],#1 \n\t"
      "bne 2b \n\t"

      "mov %[line_iterations], %[line_size] \n\t"
      "3: \n\t"
      "PRFM PLDL1KEEP, [%[src_local], #-24] \n\t"
      "LD3 {V0.8B, V1.8B, V2.8B}, [%[src_local]], #24   \n\t"
      "ST3 {V0.B, V1.B, V2.B}[0], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[1], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[1], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[2], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[3], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[3], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[4], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[5], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[5], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[6], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[7], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[7], [%[dst_local]], #3                \n\t"

      //"DSB SY\n\t"
      "subs %[line_iterations],%[line_iterations],#1 \n\t"
      "bne 3b \n\t"

      "mov %[line_iterations], %[line_size] \n\t"
      /*"add %[dst_local],%[dst_local],%[stride_dst] \n\t"*/

      "subs %[height_iterations],%[height_iterations],#1 \n\t"
      "bne 1b \n\t"

      : [src_local] "+r"(src_local), [dst_local] "+r"(dst_local), [line_iterations] "+r"(line_iterations),
        [line_length] "+r"(line_length), [height_iterations] "+r"(height_iterations), [line_size] "+r"(line_size),
        [stride_dst] "+r"(stride_dst)
      :);
}

void neon_memcpy_1280(char* dst, char* src)
{
  char* dst_local         = dst;
  char* src_local         = src;
  long  line_size         = (1280 * 3) / 24;
  long  line_length       = (1280 * 3);
  long  line_iterations   = line_size;
  long  height_iterations = 720;
  long  stride_dst        = 640 * 3;

  __asm volatile(
      "1: \n\t"
      "PRFM PLDL1KEEP, [%[src_local], #-24] \n\t"
      "LD3 {V0.8B, V1.8B, V2.8B}, [%[src_local]], #24   \n\t"
      "ST3 {V0.B, V1.B, V2.B}[0], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[1], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[2], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[3], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[4], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[5], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[6], [%[dst_local]], #3                \n\t"
      "ST3 {V0.B, V1.B, V2.B}[7], [%[dst_local]], #3                \n\t"

      //"DSB SY\n\t"
      "subs %[line_iterations],%[line_iterations],#1 \n\t"
      "bne 1b \n\t"

      "mov %[line_iterations], %[line_size] \n\t"
      /*"add %[dst_local],%[dst_local],%[stride_dst] \n\t"*/

      "subs %[height_iterations],%[height_iterations],#1 \n\t"
      "bne 1b \n\t"

      : [src_local] "+r"(src_local), [dst_local] "+r"(dst_local), [line_iterations] "+r"(line_iterations),
        [line_length] "+r"(line_length), [height_iterations] "+r"(height_iterations), [line_size] "+r"(line_size),
        [stride_dst] "+r"(stride_dst)
      :);
}
