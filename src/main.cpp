#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Sipeed_OV2640.h>

#define TFT_DC            8
#define TFT_CS            7
#define TFT_RST           9
#define TFT_MOSI          11
#define TFT_CLK           13
#define TFT_MISO          12

#define SPI_FREQUENCY     67000000

#define COLOR_WHITE       0xFFFF

//////////////////////////////////////////////////////////

Sipeed_OV2640 camera(FRAMESIZE_QVGA, PIXFORMAT_RGB565);

SPIClass spiClass(SPI0, 3, 10, 11, 12); // k210 remaping - sck, miso, mosi, ss
Adafruit_ILI9341 tft = Adafruit_ILI9341(&spiClass, TFT_DC, TFT_CS, TFT_RST); // hardware spi

void setup() {
  Serial.begin(115200);
  Serial.println("ILI9341 Test!"); 

  sysctl_set_spi0_dvp_data(1);

  uint32_t spiFreq = SPI_FREQUENCY;
  spiClass.setFrequency(spiFreq);
  tft.begin(spiFreq);
  tft.setRotation(1);

  if(!camera.begin())
    Serial.printf("camera init fail\n");
  else
    Serial.printf("camera init success\n");
  camera.run(true);
  camera.setInvert(true);
  camera.setRotation(1);

  Serial.print("camera width: ");
  Serial.print(camera.width());
  Serial.print(", height: ");
  Serial.println(camera.height());
}

void loop(void) {  
  camera.snapshot();
  uint16_t *img = camera.getRGB565();
  
  if(img == nullptr || img==0)
    Serial.printf("snap fail\n");
  else {
    // tft.drawRect(60, 120, 80, 140, COLOR_WHITE);
    tft.drawRGBBitmap(0, 0, img, camera.width(), camera.height());
    // tft.drawBitmap(0, 0, (uint8_t *)img, camera.width(), camera.height(), 1, 2);
  }
}