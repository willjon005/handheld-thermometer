#include <Arduino.h>
#include <Wire.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

// put function declarations here:
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

double temp_C = 0, temp_F = 0;
int scaled_T = 0;
int graph_values[128] = {0};

void setup() {
  // put your setup code here, to run once:
  //begin OLED display
  Wire.begin(D2, D1);
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("SSD1306 allocation failed");
    while(1); // Don't proceed, loop forever
  }

  //begin MLX
  if(!mlx.begin(0x5A)){
    Serial.println("MLX90614 initialization failed");
    while(1);
  }

  //begin Serial
  Serial.begin(9600);
  while(!Serial);

  // Set text size and color
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
}

void loop() {
  // put your main code here, to run repeatedly:
  temp_C = mlx.readObjectTempC();
  temp_F = mlx.readObjectTempF();

  scaled_T = (int)(48 * ( ( temp_C + 40 ) / 200 )); //-40 degree minimum, 200 degree range (-40C -> 160C), 48 pixel height 

  display.clearDisplay();

  // Set cursor position (x, y)
  display.setCursor(0, 0);

  // Print text
  display.printf("Object Temperature:\n %.2lfC / %.2lfF", temp_C, temp_F);

  // real time graph
  // shift over all items in the graph array, and place the current scaled temperature into the 127th slot
  for(size_t i = 0; i < 127; i++){
    graph_values[i] = graph_values[i+1];
  }
  graph_values[127] = scaled_T;

  // plot each of the points in graph_values on the display
  for(size_t i = 0; i < 128; i++){
    display.drawPixel(i, 63 - graph_values[i], SSD1306_WHITE);
  }

  //draw line for zero degrees C
  display.drawLine(0, 16, 127, 16, SSD1306_WHITE);

  // Actually draw the text to the screen
  display.display();

  delay(500);
}

// put function definitions here:
