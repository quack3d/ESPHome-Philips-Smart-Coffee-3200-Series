import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components.uart import UARTComponent
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]

DISPLAY_UART_ID = "display_uart"
MAINBOARD_UART_ID = "mainboard_uart"
POWER_PIN = "power_pin"
CONTROLLER_ID = "controller_id"
INVERT_POWER_PIN = "invert_power_pin"
POWER_TRIP_DELAY = "power_trip_delay"

CONF_COMMAND_SET = "model"
COMMAND_SETS = {"EP_3243": "PHILIPS_EP3243"}

philips_series_3200_ns = cg.esphome_ns.namespace("philips_series_3200")
PhilipsSeries3200 = philips_series_3200_ns.class_("PhilipsSeries3200", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(PhilipsSeries3200),
        cv.Required(DISPLAY_UART_ID): cv.use_id(UARTComponent),
        cv.Required(MAINBOARD_UART_ID): cv.use_id(UARTComponent),
        cv.Required(POWER_PIN): pins.gpio_output_pin_schema,
        cv.Optional(INVERT_POWER_PIN, default=False): cv.boolean,
        cv.Optional(POWER_TRIP_DELAY, default="500ms"): cv.All(
            cv.positive_time_period_milliseconds,
            cv.Range(
                min=cv.TimePeriod(milliseconds=0),
                max_included=cv.TimePeriod(milliseconds=10000),
            ),
        ),
        cv.Optional(CONF_COMMAND_SET, default="EP_3243"): cv.enum(
            COMMAND_SETS, upper=True, space="_"
        ),
    }
).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    # Use user-specified command set, default to EP_3243
    cg.add_define(config[CONF_COMMAND_SET])

    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

    display = yield cg.get_variable(config[DISPLAY_UART_ID])
    mainboard = yield cg.get_variable(config[MAINBOARD_UART_ID])
    pin = yield cg.gpio_pin_expression(config[POWER_PIN])

    cg.add(var.register_display_uart(display))
    cg.add(var.register_mainboard_uart(mainboard))
    cg.add(var.set_power_pin(pin))
    cg.add(var.set_invert_power_pin(config[INVERT_POWER_PIN]))
    cg.add(var.set_power_trip_delay(config[POWER_TRIP_DELAY]))