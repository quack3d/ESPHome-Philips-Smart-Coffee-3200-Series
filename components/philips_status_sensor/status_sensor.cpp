#include "esphome/core/log.h"
#include "status_sensor.h"

namespace esphome
{
    namespace philips_series_3200
    {
        namespace philips_status_sensor
        {
            static const char *TAG = "philips_status_sensor";

            void StatusSensor::setup()
            {
            }

            void StatusSensor::dump_config()
            {
                ESP_LOGCONFIG(TAG, "Philips Status Text Sensor");
            }

            void StatusSensor::update_status(uint8_t *data, size_t len)
            {
                // reject invalid messages
                if (len < 19 && data[0] != 0xD5 && data[1] != 0x55)
                    return;

                // TODO: figure out how the checksum is calculated and only parse valid messages

                // Check if the play/pause button is on/off/blinking
                if ((data[16] == 0x07) != play_pause_led_)
                {
                    play_pause_last_change_ = millis();
                }
                play_pause_led_ = data[16] == 0x07;

                // Check for idle state (selection led on)
                if (data[3] == 0x07 && data[4] == 0x07 && data[5] == 0x07 && data[13] == 0x00 && data[14] == 0x00 && data[15] == 0x00)
                {
                    // selecting a beverage can result in a short "busy" period since the play/pause button has not been blinking
                    // This can be circumvented: if the user is on the selection screen/idle we can reset the timer
                    play_pause_last_change_ = millis();

                    update_state("Idle");
                    return;
                }

                // Check for rotating icons - pre heating
                if (data[3] == 0x03 || data[4] == 0x03 || data[5] == 0x03 || data[6] == 0x03)
                {
                    if (play_pause_led_)
                        update_state("Cleaning");
                    else
                        update_state("Preparing");
                    return;
                }

                // Water empty led
                if (data[14] == 0x38)
                {
                    update_state("Water empty");
                    return;
                }

                // Waste container led
                if (data[15] == 0x07)
                {
                    update_state("Waste container warning");
                    return;
                }

                // Warning/Error led
                if (data[15] == 0x38)
                {
                    update_state("Error");
                    return;
                }

                // Coffee selected
                if (data[3] == 0x00 && data[4] == 0x00 && (data[5] == 0x07 || data[5] == 0x38) && data[6] == 0x00)
                {
                    if (millis() - play_pause_last_change_ < BLINK_THRESHOLD)
                        update_state((data[5] == 0x07) ? "Coffee selected" : "2x Coffee selected");
                    else
                        //update_state("Busy");
						update_state("Brewing Coffee");
                    return;
                }

                // Steam selected - Latte Macchiato
                if (data[3] == 0x00 && data[4] == 0x00 && data[5] == 0x00 && data[6] == 0x07)
                {
                    if (millis() - play_pause_last_change_ < BLINK_THRESHOLD)
                        update_state(use_latte_?"Latte Macchiato selected":"Steam selected");
                    else
                        //update_state("Busy");
						update_state("Brewing Latte Macchiato");
                    return;
                }

                // Cappuccino selected
                if (data[3] == 0x00 && data[4] == 0x07 && data[5] == 0x00 && data[6] == 0x00)
                {
                    if (millis() - play_pause_last_change_ < BLINK_THRESHOLD)
                        update_state("Cappuccino selected");
                    else
                        //update_state("Busy");
						update_state("Brewing Cappuccino");
                    return;
                }
                
                // Hot water selected
                if (data[3] == 0x00 && data[4] == 0x00 && data[5] == 0x00 && data[6] == 0x00 && data[7] == 0x38)
                {
                    if (millis() - play_pause_last_change_ < BLINK_THRESHOLD)
                        update_state("Hot water selected");
                    else
                        //update_state("Busy");
						update_state("Brewing Hot water");
                    return;
                }    				

                // Espresso selected
                if ((data[3] == 0x07 || data[3] == 0x38) && data[4] == 0x00 && data[5] == 0x00 && data[6] == 0x00)
                {
                    if (millis() - play_pause_last_change_ < BLINK_THRESHOLD)
                        update_state((data[3] == 0x07) ? "Espresso selected" : "2x Espresso selected");
                    else
                        //update_state("Busy");
						update_state("Brewing Espresso");
                    return;
                }
                
                // Americano selected
                if (data[3] == 0x00 && data[4] == 0x00 && data[5] == 0x00 && (data[6] == 0x38 || data[7] == 0x7))
                {
                    if (millis() - play_pause_last_change_ < BLINK_THRESHOLD)
                        update_state((data[6] == 0x38) ? "Americano selected" : "2x Americano selected");
                    else
                        //update_state("Busy");
						update_state("Brewing Americano");
                    return;
                }
            }

        } // namespace philips_status_sensor
    }     // namespace philips_series_3200
} // namespace esphome