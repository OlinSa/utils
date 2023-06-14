#ifndef LCD_H_
#define LCD_H_

#include <memory>
#include <stdint.h>

#include "lcd_platform.h"

#define LCD_W 240
#define LCD_H 240

class Lcd {
public:
  Lcd();
  virtual ~Lcd();
  void Initialize();
  void Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend,
            uint16_t color);
  void DrawPoint(uint16_t x, uint16_t y, uint16_t color);
  void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                uint16_t color);
  void DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                     uint16_t color);
  void DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);

  void ShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc,
                uint8_t sizey, uint8_t mode);
  void ShowString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc,
                  uint16_t bc, uint8_t sizey, uint8_t mode);
  void ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len,
                  uint16_t fc, uint16_t bc, uint8_t sizey);
  void ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len,
                     uint16_t fc, uint16_t bc, uint8_t sizey);

  void ShowPicture(uint16_t x, uint16_t y, uint16_t length, uint16_t width,
                   const uint8_t pic[]);

private:
  std::shared_ptr<LcdPlatform> plat_;
};
#endif