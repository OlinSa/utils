#include "linux_spi_lcd.h"

LinuxSpiLcd::LinuxSpiLcd() {}
LinuxSpiLcd::~LinuxSpiLcd() {}

void LinuxSpiLcd::Initialize(){}
void LinuxSpiLcd::WritBus(uint8_t dat) {}
void LinuxSpiLcd::WrData8(uint8_t dat) {}
void LinuxSpiLcd::WrData16(uint16_t dat){}
void LinuxSpiLcd::WrReg(uint8_t dat) {}
void LinuxSpiLcd::AddressSet(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){}

static std::shared_ptr<LcdPlatform> g_impl;
std::shared_ptr<LcdPlatform> LcdPlatform::builder() {
  if (g_impl == nullptr) {
    g_impl = std::make_shared<LinuxSpiLcd>();
  }
  return g_impl;
}