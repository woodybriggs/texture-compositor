#ifndef VECTOR_H
#define VECTOR_H

typedef union Vec2
{
  float e[2];
  struct
  {
    float x, y;
  };
} Vec2;

typedef union Vec3
{
  float e[3];
  struct
  {
    float x, y, z;
  };
  struct
  {
    float r, g, b;
  };
} Vec3;

typedef union Vec4
{
  float e[4];
  struct
  {
    float x, y, z, w;
  };
  struct
  {
    float r, g, b, a;
  };
} Vec4;

#endif