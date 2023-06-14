#ifndef LCD_PLATFORM_H_
#define LCD_PLATFORM_H_

#include <stdint.h>
#include <memory>

class LcdPlatform {
public:
  static std::shared_ptr<LcdPlatform> builder();

  virtual void Initialize() = 0;
  virtual void WritBus(uint8_t dat) = 0;
  virtual void WrData8(uint8_t dat) = 0;
  virtual void WrData16(uint16_t dat) = 0;
  virtual void WrReg(uint8_t dat) = 0;
  virtual void AddressSet(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) = 0;
};

#endif