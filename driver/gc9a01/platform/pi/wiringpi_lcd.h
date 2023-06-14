#ifndef WIRING_PI_PLATFORM_
#define WIRING_PI_PLATFORM_
#include "lcd_platform.h"
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define USE_HORIZONTAL 0 // 0或1为竖屏 2或3为横屏

class WiringPiLcd : public LcdPlatform {
public:
  WiringPiLcd();
  virtual ~WiringPiLcd();

  void Initialize() override;
  void WritBus(uint8_t dat) override;
  void WrData8(uint8_t dat) override;
  void WrData16(uint16_t dat) override;
  void WrReg(uint8_t dat) override;
  void AddressSet(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) override;

protected:
  void DelayMs(uint32_t ms);
  void DeviceGpioInit();
  void Reset();
  void SetAttributes(uint8_t direct);
  void InitReg();
  void WrRst(uint8_t dat);
  void WrBlk(uint8_t dat);

private:
  int32_t channel_; // IO 23
  int32_t dc_;      // P25: Data or Command
  int32_t din_;     // IO 25
  int32_t clk_;     // IO 23
  int32_t rst_;     // P13
  int32_t ce_;      // IO 24
  int32_t blk_;     // P18
};

#endif
