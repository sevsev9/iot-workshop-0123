#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// Sensor Settings
#define BMP280_ADDRESS 0x76
#define BMP280_SENSID 0x58
#define SEA_LEVEL 1013.25

// Delays
#define DATA_DELAY 500  //ms
#define RENDER_DELAY 100 //ms

// Last updates
long last_data_update = 0;
long last_display_update = 0;


// Screen Settings
#define SCREEN_ADDRESS 0x3C
#define SCREEN_HEIGHT 64  // px
#define SCREEN_WIDTH 128  // px
#define TEXT_SIZE 1
#define LINE_WIDTH 21 // chars

// Render Task Handle
TaskHandle_t RenderTask;

// char pointers (Strings) that will be displayed on the display
char* tmp_str;
char* pressure_str;
char* alt_str;

// Starting pixel of every line on the display @ Text Size 1
const int16_t LINES[7] = { 0, 8, 16, 24, 32, 40, 48 };

// draw and read function definitions
void read_data(void* param);
void draw_info(void* param);

// Sensor References
Adafruit_BMP280 bmp;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup() {
  // Initialize the Serial Communication with 115200 bps
  Serial.begin(115200);
  Serial.println("Starting up...");


  // Initialize Display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1) delay(100);
  }

  // Display Setup
  display.clearDisplay();
  display.setTextSize(TEXT_SIZE);
  display.setTextColor(WHITE);
  display.setCursor(0, LINES[0]);
  display.print(F("Initialized Display!"));
  display.display();

  // BMP280 Sensor Setup
  display.setCursor(0, LINES[1]);
  display.print(F("Initializing BMP280..."));
  if (!bmp.begin(BMP280_ADDRESS, BMP280_SENSID)) {
    display.setCursor(0, LINES[1]);
    Serial.println("BMP280 init failed. Check wiring.");
    display.print(F("BMP280 init failed.   "));
    display.display();
    while (1) delay(100);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  // initialize the strings to be displayed
  tmp_str = (char*)calloc(24, sizeof(char));
  pressure_str = (char*)calloc(24, sizeof(char));
  alt_str = (char*)calloc(24, sizeof(char));

  // initialize the render task
  xTaskCreatePinnedToCore(
    draw_info,     /* Function to implement the task */
    "RenderTask",  /* Name of the task */
    10000,         /* Stack size in words */
    NULL,          /* Task input parameter */
    0,             /* Priority of the task */
    &RenderTask,   /* Task handle. */
    0);            /* Core where the task should run */
}

void loop() {
  read_data((void *) 0);
}

void draw_info(void* param) {
  while (1) {
    // re-draw information on the i2c display
    if (millis() > last_display_update + RENDER_DELAY) {
      display.clearDisplay();
      display.setCursor(0, LINES[0]);
      display.print(tmp_str);
      display.setCursor(0, LINES[1]);
      display.print(pressure_str);
      display.setCursor(0, LINES[2]);
      display.print(alt_str);
      display.display();

      last_display_update = millis();
    }

    // to conserve energy (~98.5% power usage reduction)
    delay(10);
  }
}

void read_data(void* param) {
  while (1) {
    
    if (millis() - last_data_update > DATA_DELAY) {
      float temp = bmp.readTemperature();
      float pre = bmp.readPressure();
      float alt = bmp.readAltitude(SEA_LEVEL);  // sea level pressure as parameter
      sprintf(tmp_str, "Temperature: %.2f%cC", temp, 167);
      sprintf(pressure_str, "Pressure: %.2f kPa", pre / 1000);
      sprintf(alt_str, "Altitude: %.2fm", alt);

      last_data_update = millis();
    }

    // to conserve energy (~98.5% power usage reduction)
    delay(10);
  }
}
