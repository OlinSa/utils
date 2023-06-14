#include "wiringpi_lcd.h"

#include <unistd.h>

#include <iostream>

WiringPiLcd::WiringPiLcd() {
  if (wiringPiSetupGpio() < 0) {
    std::cout << "wiringPiSetupGpio failed" << std::endl;
  }

  channel_ = 0;
  dc_ = 25;
  rst_ = 27;
  ce_ = 24;
  blk_ = 18;

  DeviceGpioInit();

  if (-1 == wiringPiSPISetup(channel_, 40000000)) {
    std::cout << "set spi fail" << std::endl;
  } else {
    std::cout << "set spi channel=" << channel_ << std::endl;
  }
  pinMode(blk_, PWM_OUTPUT);
  pwmWrite(blk_, 512);
}
WiringPiLcd::~WiringPiLcd() {}

void WiringPiLcd::DeviceGpioInit() {
  pinMode(ce_, OUTPUT);
  pinMode(rst_, OUTPUT);
  pinMode(dc_, OUTPUT);
  pinMode(blk_, OUTPUT);
  digitalWrite(ce_, 1);
  digitalWrite(blk_, 1);
}

void WiringPiLcd::SetAttributes(uint8_t direct) {
  WrReg(0x36);
  if (direct == 0)
    WrData8(0x08);
  else if (direct == 1)
    WrData8(0xC8);
  else if (direct == 2)
    WrData8(0x68);
  else
    WrData8(0xA8);
}

void WiringPiLcd::InitReg() {
  WrReg(0xEF);
  WrReg(0xEB);
  WrData8(0x14);

  WrReg(0xFE);
  WrReg(0xEF);

  WrReg(0xEB);
  WrData8(0x14);

  WrReg(0x84);
  WrData8(0x40);

  WrReg(0x85);
  WrData8(0xFF);

  WrReg(0x86);
  WrData8(0xFF);

  WrReg(0x87);
  WrData8(0xFF);

  WrReg(0x88);
  WrData8(0x0A);

  WrReg(0x89);
  WrData8(0x21);

  WrReg(0x8A);
  WrData8(0x00);

  WrReg(0x8B);
  WrData8(0x80);

  WrReg(0x8C);
  WrData8(0x01);

  WrReg(0x8D);
  WrData8(0x01);

  WrReg(0x8E);
  WrData8(0xFF);

  WrReg(0x8F);
  WrData8(0xFF);

  WrReg(0xB6);
  WrData8(0x00);
  WrData8(0x20);

  WrReg(0x36);
  WrData8(0x08); // Set as vertical screen

  WrReg(0x3A);
  WrData8(0x05);

  WrReg(0x90);
  WrData8(0x08);
  WrData8(0x08);
  WrData8(0x08);
  WrData8(0x08);

  WrReg(0xBD);
  WrData8(0x06);

  WrReg(0xBC);
  WrData8(0x00);

  WrReg(0xFF);
  WrData8(0x60);
  WrData8(0x01);
  WrData8(0x04);

  WrReg(0xC3);
  WrData8(0x13);
  WrReg(0xC4);
  WrData8(0x13);

  WrReg(0xC9);
  WrData8(0x22);

  WrReg(0xBE);
  WrData8(0x11);

  WrReg(0xE1);
  WrData8(0x10);
  WrData8(0x0E);

  WrReg(0xDF);
  WrData8(0x21);
  WrData8(0x0c);
  WrData8(0x02);

  WrReg(0xF0);
  WrData8(0x45);
  WrData8(0x09);
  WrData8(0x08);
  WrData8(0x08);
  WrData8(0x26);
  WrData8(0x2A);

  WrReg(0xF1);
  WrData8(0x43);
  WrData8(0x70);
  WrData8(0x72);
  WrData8(0x36);
  WrData8(0x37);
  WrData8(0x6F);

  WrReg(0xF2);
  WrData8(0x45);
  WrData8(0x09);
  WrData8(0x08);
  WrData8(0x08);
  WrData8(0x26);
  WrData8(0x2A);

  WrReg(0xF3);
  WrData8(0x43);
  WrData8(0x70);
  WrData8(0x72);
  WrData8(0x36);
  WrData8(0x37);
  WrData8(0x6F);

  WrReg(0xED);
  WrData8(0x1B);
  WrData8(0x0B);

  WrReg(0xAE);
  WrData8(0x77);

  WrReg(0xCD);
  WrData8(0x63);

  WrReg(0x70);
  WrData8(0x07);
  WrData8(0x07);
  WrData8(0x04);
  WrData8(0x0E);
  WrData8(0x0F);
  WrData8(0x09);
  WrData8(0x07);
  WrData8(0x08);
  WrData8(0x03);

  WrReg(0xE8);
  WrData8(0x34);

  WrReg(0x62);
  WrData8(0x18);
  WrData8(0x0D);
  WrData8(0x71);
  WrData8(0xED);
  WrData8(0x70);
  WrData8(0x70);
  WrData8(0x18);
  WrData8(0x0F);
  WrData8(0x71);
  WrData8(0xEF);
  WrData8(0x70);
  WrData8(0x70);

  WrReg(0x63);
  WrData8(0x18);
  WrData8(0x11);
  WrData8(0x71);
  WrData8(0xF1);
  WrData8(0x70);
  WrData8(0x70);
  WrData8(0x18);
  WrData8(0x13);
  WrData8(0x71);
  WrData8(0xF3);
  WrData8(0x70);
  WrData8(0x70);

  WrReg(0x64);
  WrData8(0x28);
  WrData8(0x29);
  WrData8(0xF1);
  WrData8(0x01);
  WrData8(0xF1);
  WrData8(0x00);
  WrData8(0x07);

  WrReg(0x66);
  WrData8(0x3C);
  WrData8(0x00);
  WrData8(0xCD);
  WrData8(0x67);
  WrData8(0x45);
  WrData8(0x45);
  WrData8(0x10);
  WrData8(0x00);
  WrData8(0x00);
  WrData8(0x00);

  WrReg(0x67);
  WrData8(0x00);
  WrData8(0x3C);
  WrData8(0x00);
  WrData8(0x00);
  WrData8(0x00);
  WrData8(0x01);
  WrData8(0x54);
  WrData8(0x10);
  WrData8(0x32);
  WrData8(0x98);

  WrReg(0x74);
  WrData8(0x10);
  WrData8(0x85);
  WrData8(0x80);
  WrData8(0x00);
  WrData8(0x00);
  WrData8(0x4E);
  WrData8(0x00);

  WrReg(0x98);
  WrData8(0x3e);
  WrData8(0x07);

  WrReg(0x35);
  WrReg(0x21);

  WrReg(0x11);
  DelayMs(120);
  WrReg(0x29);
  DelayMs(20);
}

void WiringPiLcd::Reset() {
  digitalWrite(rst_, 1);
  delay(100);
  digitalWrite(rst_, 0);
  delay(100);
  digitalWrite(rst_, 1);
  delay(100);
}

void WiringPiLcd::Initialize() {
  // Turn on the backlight
  digitalWrite(blk_, 1);

  // Hardware reset
  Reset();

  // Set the resolution and scanning method of the screen
  SetAttributes(USE_HORIZONTAL);

  // Set the initialization register
  InitReg();
}
void WiringPiLcd::DelayMs(uint32_t ms) { usleep(ms * 1000); }

void WiringPiLcd::WritBus(uint8_t dat) { wiringPiSPIDataRW(channel_, &dat, 1); }
void WiringPiLcd::WrData8(uint8_t dat) {
  digitalWrite(dc_, HIGH);
  WritBus(dat);
}
void WiringPiLcd::WrData16(uint16_t dat) {
  digitalWrite(dc_, HIGH);
  WritBus(dat >> 8);
  WritBus(dat);
}
void WiringPiLcd::WrReg(uint8_t dat) {
  digitalWrite(dc_, LOW);
  WritBus(dat);
}
void WiringPiLcd::AddressSet(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  // set the X coordinates
  WrReg(0x2A);
  WrData8(0x00);
  WrData8(x1);
  WrData8(0x00);
  WrData8(x2 - 1);

  // set the Y coordinates
  WrReg(0x2B);
  WrData8(0x00);
  WrData8(y1);
  WrData8(0x00);
  WrData8((y2 - 1));

  WrReg(0x2C);
}

void WiringPiLcd::WrRst(uint8_t dat) {
  printf("WrRst=%u\n", dat);
  digitalWrite(rst_, dat == 0 ? LOW : HIGH);
}
void WiringPiLcd::WrBlk(uint8_t dat) {
  printf("WrBlk=%u\n", dat);
  digitalWrite(blk_, dat == 0 ? LOW : HIGH);
}

static std::shared_ptr<LcdPlatform> g_impl;
std::shared_ptr<LcdPlatform> LcdPlatform::builder() {
  if (g_impl == nullptr) {
    g_impl = std::make_shared<WiringPiLcd>();
  }
  return g_impl;

}