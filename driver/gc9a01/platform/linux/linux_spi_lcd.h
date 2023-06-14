#ifndef LINUX_SPI_LCD_H_
#define LINUX_SPI_LCD_H_
#include "lcd_platform.h"

class LinuxSpiLcd : public LcdPlatform {
public:
  LinuxSpiLcd();
  virtual ~LinuxSpiLcd();

  void Initialize() override;
  void WritBus(uint8_t dat) override;
  void WrData8(uint8_t dat) override;
  void WrData16(uint16_t dat) override;
  void WrReg(uint8_t dat) override;
  void AddressSet(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) override;
};

#endif