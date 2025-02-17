# ESPHome Smart Coffee (Philips Series 3200)
----------------------
----------------------
# DEPRECATED. Please use TillFleisch's repo which now supports 3200 series: https://github.com/TillFleisch/ESPHome-Philips-Smart-Coffee #
----------------------
----------------------
This project integrates a Philips Series 3200 Coffee Machine into [Home Assistant](https://home-assistant.io) through [ESPHome](https://esphome.io). It is a fork of TillFleich's repo for the 2200 Series.
This component has been developed on a Philips EP3243 and an ESP32 S2 Mini.

A really nice part of this project is that it can be done in a non-abrasive manner. Going back to factory settings is as simple as popping off the display, removing the ESP and connecting one cable back again. This is possible by purchasing an extra cable and connector instead of cutting the existing cable. See the wiring section for details and links to parts.

This component provides a `Power Switch`, a `Status sensor` and various `Buttons` which simulate user input.
The `Power Switch` can be used to turn on the coffee machine with and without a cleaning cycle during startup.

<p float="left">
  <img src="/esp32_wired.jpg" height="400" /> 
  <img src="/ha_entities.png" height="400" />
</p>

You might break/brick your coffee machine by modifying it in any way, shape or form. If you want to use this component, do so at your own risk.

# Configuration variables

A example configuration can be found [here](example.yaml)

## Philips Series 3200

- **id**(**Required**, string):Controller ID which will be used for entity configuration.
- **display_uart**(**Required**, string): ID of the UART-Component connected to the display unit
- **mainboard_uart**(**Required**, string): ID of the UART-Component connected to the mainboard
- **power_pin**(**Required**, [Pin](https://esphome.io/guides/configuration-types.html#config-pin)): Pin to which the MOSFET/Transistor is connected. This pin is used to temporarily turn of the display unit.
- **invert_power_pin**(**Optional**: boolean): If set to `true` the output of the power pin will be inverted. Defaults to `false`.
- **power_trip_delay**(**Optional**: Time): Determines the length of the power outage applied to the display unit, which is to trick it into turning on. Defaults to `500ms`.
- **model**(**Optional**: int): Different models or revisions may use different commands. This option can be used to specify the command set used by this component. Select one of `EP3243`. Defaults to `EP3243`.

## Philips Power switch

- **controller_id**(**Required**, string): The Philips Series 3200-Controller to which this entity belongs
- **clean**(**Optional**: boolean): If set to `true` the machine will perform a cleaning cycle during startup. Otherwise the machine will power on without cleaning. Defaults to `true`.
- All other options from [Switch](https://esphome.io/components/switch/index.html#config-switch)

## Action Button

- **controller_id**(**Required**, string): The Philips Series 3200-Controller to which this entity belongs
- **action**(**Required**, int): The action performed by this button. Select one of `MAKE_COFFEE`, `SELECT_COFFEE`, `SELECT_ESPRESSO`, `MAKE_ESPRESSO`, `SELECT_HOT_WATER`, `MAKE_HOT_WATER`, `SELECT_STEAM`, `MAKE_STEAM`, `SELECT_AMERICANO`, `SELECT_LATTE_MACCHIATO`, `SELECT_CAPPUCCINO`, `MILK`, `BEAN`, `SIZE`, `AQUA_CLEAN`, `CALC_CLEAN`, `PLAY_PAUSE`.
- **long_press**(**Optional**, boolean): If set to `true` this button will perform a long press. This option is only available for actions which don't include `MAKE`.
- All other options from [Button](https://esphome.io/components/button/index.html#config-button)

## Philips Status Sensor

- **controller_id**(**Required**, string): The Philips Series 3200-Controller to which this entity belongs
- All other options from [Text Sensor](https://esphome.io/components/text_sensor/index.html#config-text-sensor)
- **use_latte**(**Optional**, boolean): If set to `true`, `Latte Macchiato selected` will be reported instead of `Steam selected`. This option is intended for machines like the EP3243 that can make latte macchiato. Default to `false`.

## Bean and Size Settings

- **type**(**Required**, string): The type of this number component. One of `size`, `bean` and `milk`. If `size` is selected, this component will report/manipulate the beverage size. If `bean` is used, this component will report/manipulate the beverage strength. If `milk` is used, this component will report/manipulate the milk amount.
- **controller_id**(**Required**, string): The Philips Series 3200-Controller to which this entity belongs
- **status_sensor_id**(**Required**, string): Id of a status sensor which is also connected to the controller.
- **source**(**Required**, int): The source of this sensor. Select one of `COFFEE`, `ESPRESSO`, `LATTE MACCHIATO`. When selecting `LATTE MACCHIATO` the related status sensor must use `use_latte = true`.
- All other options from [Number](https://esphome.io/components/number/index.html#config-number)

## Size Settings

- **controller_id**(**Required**, string): The Philips Series 3200-Controller to which this entity belongs
- **status_sensor_id**(**Required**, string): Id of a status sensor which is also connected to the controller.
- **source**(**Required**, int): The source of this sensor. Select one of `COFFEE`, `ESPRESSO`, `LATTE MACCHIATO`, `CAPPUCCINO`, `AMERICANO`, `HOT_WATER`. When selecting `LATTE MACCHIATO` the related status sensor must use `use_latte = true`.
- All other options from [Number](https://esphome.io/components/number/index.html#config-number)

## Milk Settings

- **controller_id**(**Required**, string): The Philips Series 3200-Controller to which this entity belongs
- **status_sensor_id**(**Required**, string): Id of a status sensor which is also connected to the controller.
- **source**(**Required**, int): The source of this sensor. Select one of `CAPPUCCINO`, `LATTE MACCHIATO`. When selecting `LATTE MACCHIATO` the related status sensor must use `use_latte = true`.
- All other options from [Number](https://esphome.io/components/number/index.html#config-number)


# Wiring

The coffee machines display unit is connected to the mainboard via a 8-pin ribbon cable with Picoflex connectors.
The display is powered by the mainboard and the two units communicate using a serial bus.
The ESP is placed in between this bus to perform a man-in-the-middle attack.
The RX/TX lines are piped through the ESP such that messages can be read, intercepted and injected.

When injecting a 'turn coffee machine on' command, the coffee machine does turn on, but the display unit does not. To circumvent this behavior we can re-boot the display unit by temporarily removing it's power. Thus the display will power on and operate normally. To perform this operation a transistor or MOSFET can be used. I used the IRLZ44N.

I got the 8P connector from here: https://www.aliexpress.com/item/1005005142277556.html </br>
And the 8P 'Same direction' from here: https://www.aliexpress.com/item/1005005026658462.html </br>
And the ESP32 S2 Mini with PSRAM from here: https://www.aliexpress.com/item/1005004971974530.html </br>

The following wiring guideline can be used to add a ESP32 S2 Mini to the coffee machine. **The unlabeled wires should be connected without changes.**
<p float="left">
  <img src="/wiring.png" height="400" />
</p>

# Troubleshooting

- Make sure your wiring is correct
- The UART debug function can be used to analyze communication and verify correct wiring
- The commands used by the display unit may be different between different revisions/models
