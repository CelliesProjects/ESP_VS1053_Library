/* This is an example that shows how to use the VS1053 library together with M5Stack displays.
   In this example a M5Stack Grey is used.

   The SPI bus is shared between the M5Stack display and the VS1053 codec board.
   This code is not thread safe.  
   
   Let the display and VS1053 take turns to use the SPI bus.
 */
#include <M5GFX.h> /* https://github.com/m5stack/M5GFX */

// This ESP_VS1053_Library
#include <VS1053.h>

#include "SampleMp3.h"

#define VS1053_CS 13
#define VS1053_DCS 26
#define VS1053_DREQ 36
#define SPI_CLK_PIN 18
#define SPI_MISO_PIN 19
#define SPI_MOSI_PIN 23

#define VOLUME 100 // volume level 0-100

M5GFX display;
VS1053 player(VS1053_CS, VS1053_DCS, VS1053_DREQ);

void setup()
{
    Serial.begin(115200);
    Serial.println("\n\nM5Stack Grey + VS1053 Demo");

    // Initialize SPI with the M5Stack SPI pins
    SPI.begin(SPI_CLK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN);

    /* Start the M5 display */
    display.init();
    display.setTextSize((float)display.width() / 160);
    display.println("VS1053 + M5Stack display");

    Serial.println("Disabling M5Stack DAC and internal speaker");
    const auto DAC_PIN = 25; // on Grey and Fire
    pinMode(DAC_PIN, OUTPUT); 
    digitalWrite(DAC_PIN, 0);

    Serial.println("Hello VS1053!");
    // initialize a player
    player.begin();
    if (player.getChipVersion() == 4)
    { // Only perform an update if we really are using a VS1053, not. eg. VS1003
        player.loadDefaultVs1053Patches();
    }
    player.switchToMp3Mode(); // optional, some boards require this
    player.setVolume(VOLUME);
}

void loop()
{
    Serial.println("Playing sound... ");

    // play mp3 flow each second
    static auto cnt = 0;
    player.playChunk(sampleMp3, sizeof(sampleMp3));
    display.printf("Played %i times\n", ++cnt);
    delay(1000);
}