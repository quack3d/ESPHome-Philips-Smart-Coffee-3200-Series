#include "esphome/core/log.h"
#include "action_button.h"

namespace esphome
{
    namespace philips_series_3200
    {
        namespace philips_action_button
        {

            static const char *const TAG = "philips-action-button";

            void ActionButton::dump_config()
            {
                LOG_BUTTON("", "Philips Action Button", this);
            }

            void ActionButton::loop()
            {
                // Repeated message sending for long presses
                if (millis() - press_start_ <= LONG_PRESS_DURATION)
                {
                    if (millis() - last_message_sent_ > LONG_PRESS_REPETITION_DELAY)
                    {
                        last_message_sent_ = millis();
                        perform_action();
                    }
                    is_long_pressing_ = true;
                }
                else
                {
                    is_long_pressing_ = false;
                }
            }

            void ActionButton::write_array(const std::vector<uint8_t> &data)
            {
                for (unsigned int i = 0; i <= MESSAGE_REPETITIONS; i++)
                    mainboard_uart_->write_array(data);
                mainboard_uart_->flush();
            }

            void ActionButton::press_action()
            {
                if (should_long_press_)
                {
                    // Reset button press start time
                    press_start_ = millis();
                    last_message_sent_ = 0;
                }
                else
                {
                    // Perform a single button press
                    perform_action();
                }
            }

            void ActionButton::perform_action()
            {
                auto action = action_;
                // Coffee
                if (action == SELECT_COFFEE || action == MAKE_COFFEE)
                {
                    write_array({0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x08, 0x00, 0x00, 0x1D, 0x1E});
					if (action == SELECT_COFFEE)
                        return;
                    delay(BUTTON_SEQUENCE_DELAY);
                    action = PLAY_PAUSE;
                }

                // Espresso
                if (action == SELECT_ESPRESSO || action == MAKE_ESPRESSO)
                {
                    write_array({0xD5, 0x55, 0x00, 0x01, 0x02, 0x00, 0x02, 0x02, 0x00, 0x00, 0x09, 0x2D});
                    if (action == SELECT_ESPRESSO)
                        return;
                    delay(BUTTON_SEQUENCE_DELAY);
                    action = PLAY_PAUSE;
                }

                // Hot water
                if (action == SELECT_HOT_WATER || action == MAKE_HOT_WATER)
                {
                    write_array({0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x00, 0x01, 0x00, 0x39, 0x38});
                    if (action == SELECT_HOT_WATER)
                        return;
                    delay(BUTTON_SEQUENCE_DELAY);
                    action = PLAY_PAUSE;
                }
				
                // Cappuccino
                if (action == SELECT_CAPPUCCINO || action == MAKE_CAPPUCCINO)
                {
                    write_array({0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x04, 0x00, 0x00, 0x05, 0x03});
                    if (action == SELECT_CAPPUCCINO)
                        return;
                    delay(BUTTON_SEQUENCE_DELAY);
                    action = PLAY_PAUSE;
                }

                // Americano
                if (action == SELECT_AMERICANO || action == MAKE_AMERICANO)
                {
                    write_array({0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x20, 0x00, 0x00, 0x04, 0x15});
                    if (action == SELECT_AMERICANO)
                        return;
                    delay(BUTTON_SEQUENCE_DELAY);
                    action = PLAY_PAUSE;
                }				

                // Steam (Latte Macchiato)
                if (action == SELECT_STEAM || action == MAKE_STEAM)
                {
                    write_array({0xD5, 0x55, 0x00, 0x01, 0x02, 0x00, 0x02, 0x10, 0x00, 0x00, 0x09, 0x26});
                    if (action == SELECT_STEAM)
                        return;
                    delay(BUTTON_SEQUENCE_DELAY);
                    action = PLAY_PAUSE;
                }

                // press/play or subsequent press/play
                if (action == PLAY_PAUSE)
                    write_array({0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x00, 0x00, 0x01, 0x3D, 0x30});
                else if (action == SELECT_BEAN)
                    // bean button
                    write_array({0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x00, 0x02, 0x00, 0x2D, 0x2D});
                else if (action == SELECT_SIZE)
                    // size button
                     write_array({0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x00, 0x04, 0x00, 0x04, 0x07});
                else if (action == SELECT_MILK_LEVEL)
                    // milk button
                     write_array({0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x00, 0x08, 0x00, 0x1F, 0x16});
				else if (action == SELECT_AQUA_CLEAN)
                    // aqua clean button
                    write_array({0xD5, 0x55, 0x00, 0x01, 0x02, 0x00, 0x02, 0x00, 0x10, 0x00, 0x0D, 0x36});
                else if (action == SELECT_CALC_CLEAN)
                    // calc clean button
                    write_array({0xD5, 0x55, 0x00, 0x01, 0x02, 0x00, 0x02, 0x00, 0x20, 0x00, 0x28, 0x37});
                else
                    ESP_LOGE(TAG, "Invalid Action provided!");
            }
        } // namespace philips_action_button
    }     // namespace philips_series_3200
} // namespace esphome