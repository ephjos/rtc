#include "rtc.h"


canvas *canvas_alloc(u32 width, u32 height)
{
  canvas *c = malloc(sizeof(canvas));
  c->width = width;
  c->height = height;

  u32 pixels_bytes = sizeof(v3) * width * height;
  c->pixels = malloc(pixels_bytes);

  memset(c->pixels, 0, pixels_bytes);

  return c;
}

void canvas_free(canvas *c)
{
  free(c->pixels);
  free(c);
}

void canvas_write(canvas *c, u32 x, u32 y, v3 color)
{
  c->pixels[y * c->width + x][0] = color[0];
  c->pixels[y * c->width + x][1] = color[1];
  c->pixels[y * c->width + x][2] = color[2];
}

void canvas_safe_write(canvas *c, s32 x, s32 y, v3 color)
{
  x = CLAMP(x, 0, (s32)c->width-1);
  y = CLAMP(y, 0, (s32)c->height-1);
  canvas_write(c, x, y, color);
}

v3 *canvas_at(const canvas *c, u32 x, u32 y)
{
  return &c->pixels[y * c->width + x];
}

char *canvas_to_ppm(const canvas *c)
{
  // P3\n = 3
  // WWWWW HHHHH\n = 12
  // 255\n = 4
  // RGB per pixel, each component needs 4 bytes = 12
  // 12 * width * height
  u32 n = 24 + (14 * c->width * c->height);
  char *buf = malloc(n);
  memset(buf, 0, n);

  u32 buf_i = 0;
  buf_i += snprintf(buf+buf_i, 24, "P3\n%ld %ld\n255\n", c->width, c->height);

  for (u32 i = 0; i < c->height; i++) {
    for (u32 j = 0; j < c->width; j++) {
      f64 *p = c->pixels[i * c->width + j];

      s32 ir = (s32)(p[0] * 255.0);
      ir = CLAMP(ir, 0, 255);

      s32 ig = (s32)(p[1] * 255.0);
      ig = CLAMP(ig, 0, 255);

      s32 ib = (s32)(p[2] * 255.0);
      ib = CLAMP(ib, 0, 255);

      buf_i += snprintf(buf+buf_i, 24, "%ld %ld %ld ", ir, ig, ib);
    }

    buf_i += snprintf(buf+buf_i, 2, "\n");
  }

  return buf;
}

