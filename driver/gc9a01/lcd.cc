#include "lcd.h"

#include "font.h"
#include <math.h>
#include <unistd.h>

Lcd::Lcd() : plat_(std::make_shared<LcdInit>()) {}
Lcd::~Lcd() {}

void Lcd::Initialize() { plat_->Initialize(); }
void Lcd::Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend,
               uint16_t color) {
  plat_->AddressSet(xsta, ysta, xend, yend);
  for (int i = ysta; i < yend; i++) {
    for (int j = xsta; j < xend; j++) {
      plat_->WrData16(color);
    }
  }
}
void Lcd::DrawPoint(uint16_t x, uint16_t y, uint16_t color) {
  printf("DrawPoint=(%d,%d,c=%d)\n", x, y, color);
  plat_->AddressSet(x, y, x, y);
  plat_->WrData16(color);
}
void Lcd::DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                   uint16_t color) {
  uint16_t t;
  int32_t xerr = 0, yerr = 0, delta_x, delta_y, distance;
  int32_t incx, incy, uRow, uCol;
  delta_x = x2 - x1;
  delta_y = y2 - y1;
  uRow = x1;
  uCol = y1;
  if (delta_x > 0)
    incx = 1;
  else if (delta_x == 0)
    incx = 0;
  else {
    incx = -1;
    delta_x = -delta_x;
  }
  if (delta_y > 0)
    incy = 1;
  else if (delta_y == 0)
    incy = 0;
  else {
    incy = -1;
    delta_y = -delta_y;
  }
  if (delta_x > delta_y)
    distance = delta_x;
  else
    distance = delta_y;
  for (t = 0; t < distance + 1; t++) {
    DrawPoint(uRow, uCol, color);
    xerr += delta_x;
    yerr += delta_y;
    if (xerr > distance) {
      xerr -= distance;
      uRow += incx;
    }
    if (yerr > distance) {
      yerr -= distance;
      uCol += incy;
    }
  }
}
void Lcd::DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                        uint16_t color) {
  DrawLine(x1, y1, x2, y1, color);
  DrawLine(x1, y1, x1, y2, color);
  DrawLine(x1, y2, x2, y2, color);
  DrawLine(x2, y1, x2, y2, color);
}
void Lcd::DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color) {
  int32_t a = 0, b = r;
  while (a <= b) {
    DrawPoint(x0 - b, y0 - a, color); // 3
    DrawPoint(x0 + b, y0 - a, color); // 0
    DrawPoint(x0 - a, y0 + b, color); // 1
    DrawPoint(x0 - a, y0 - b, color); // 2
    DrawPoint(x0 + b, y0 + a, color); // 4
    DrawPoint(x0 + a, y0 - b, color); // 5
    DrawPoint(x0 + a, y0 + b, color); // 6
    DrawPoint(x0 - b, y0 + a, color); // 7
    a++;
    if ((a * a + b * b) > (r * r)) {
      b--;
    }
  }
}

void Lcd::ShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc,
                   uint16_t bc, uint8_t sizey, uint8_t mode) {
  printf("ShowChar, (%d,%d) p=%c\n", x, y, num);
  uint8_t temp, sizex, t, m = 0;
  uint16_t i, TypefaceNum;
  uint16_t x0 = x;
  sizex = sizey / 2;
  TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
  num = num - ' ';
  plat_->AddressSet(x, y, x + sizex - 1, y + sizey - 1);
  printf("AddressSet, (%d,%d)->(%d,%d)\n", x, y, x + sizex - 1, y + sizey - 1);
  for (i = 0; i < TypefaceNum; i++) {
    // sleep(1);
    if (sizey == 12)
      temp = ascii_1206[num][i];
    else if (sizey == 16)
      temp = ascii_1608[num][i];
    else if (sizey == 24)
      temp = ascii_2412[num][i];
    else if (sizey == 32)
      temp = ascii_3216[num][i];
    else
      return;
    for (t = 0; t < 8; t++) {
      // printf("temp=%d sizey=%d num=%d i=%d\n", temp, sizey, num, i);
      if (!mode) {
        if (temp & (0x01 << t))
          plat_->WrData16(fc);
        else
          plat_->WrData16(bc);
        m++;
        if (m % sizex == 0) {
          m = 0;
          break;
        }
      } else {
        if (temp & (0x01 << t))
          DrawPoint(x, y, fc);
        x++;
        if ((x - x0) == sizex) {
          x = x0;
          y++;
          break;
        }
      }
    }
  }
}
void Lcd::ShowString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc,
                     uint16_t bc, uint8_t sizey, uint8_t mode) {
  while (p != nullptr && *p != '\0') {
    printf("ShowString, p=%c", *p);
    ShowChar(x, y, *p, fc, bc, sizey, mode);
    x += sizey / 2;
    p++;
  }
}
void Lcd::ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len,
                     uint16_t fc, uint16_t bc, uint8_t sizey) {
  int32_t t, temp;
  uint8_t enshow = 0;
  uint8_t sizex = sizey / 2;
  for (t = 0; t < len; t++) {
    temp = (int32_t(num / pow(10, len - t - 1))) % 10;
    if (enshow == 0 && t < (len - 1)) {
      if (temp == 0) {
        ShowChar(x + t * sizex, y, ' ', fc, bc, sizey, 0);
        continue;
      } else
        enshow = 1;
    }
    ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
  }
}
void Lcd::ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len,
                        uint16_t fc, uint16_t bc, uint8_t sizey) {
  uint8_t t, temp, sizex;
  uint16_t num1;
  sizex = sizey / 2;
  num1 = num * 100;
  for (t = 0; t < len; t++) {
    temp = (int32_t(num1 / pow(10, len - t - 1))) % 10;
    if (t == (len - 2)) {
      ShowChar(x + (len - 2) * sizex, y, '.', fc, bc, sizey, 0);
      t++;
      len += 1;
    }
    ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
  }
}

void Lcd::ShowPicture(uint16_t x, uint16_t y, uint16_t length, uint16_t width,
                      const uint8_t pic[]) {
  uint16_t i, j;
  uint32_t k = 0;
  plat_->AddressSet(x, y, x + length - 1, y + width - 1);
  for (i = 0; i < length; i++) {
    for (j = 0; j < width; j++) {
      plat_->WrData8(pic[k * 2]);
      plat_->WrData8(pic[k * 2 + 1]);
      k++;
    }
  }
}