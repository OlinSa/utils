#include <stdint.h>

#include "font_color.h"
#include "lcd.h"
#include "pic.h"
#include <iostream>
#include <unistd.h>

int main(void) {
  uint8_t i, j;
  float t = 0;
  Lcd lcd;

  lcd.Initialize();
  lcd.Fill(0, 0, LCD_W, LCD_H, RED);
  printf("Fill down\n");
  while (1) {
    lcd.ShowString(32, 80, (const uint8_t *)"LCD_Diameter:", RED, WHITE, 16, 1);
    // lcd.ShowIntNum(134, 80, LCD_W, 3, RED, WHITE, 16);
    // lcd.ShowString(32, 100, (const uint8_t*)"Increaseing Nun:", RED, WHITE,
    // 16,
    //                0);
    // lcd.ShowFloatNum1(160, 100, t, 4, RED, WHITE, 16);
    // t += 0.11;
    // for (j = 0; j < 3; j++) {
    //   for (i = 0; i < 6; i++) {
    //     lcd.ShowPicture(40 * i, 120 + j * 40, 40, 40, gImage_1);
    //   }
    // }
    sleep(10);
  }
}
