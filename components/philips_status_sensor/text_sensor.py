import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import CONF_ID
from ..philips_series_3200 import CONTROLLER_ID, PhilipsSeries3200

USE_LATTE = "use_latte"
STATUS_SENSOR_ID = "status_sensor_id"

philips_status_sensor_ns = cg.esphome_ns.namespace("philips_series_3200").namespace(
    "philips_status_sensor"
)
StatusSensor = philips_status_sensor_ns.class_(
    "StatusSensor", text_sensor.TextSensor, cg.Component
)

CONFIG_SCHEMA = text_sensor.TEXT_SENSOR_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(StatusSensor),
        cv.Required(CONTROLLER_ID): cv.use_id(PhilipsSeries3200),
        cv.Optional(USE_LATTE, default=False): cv.boolean,
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONTROLLER_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await text_sensor.register_text_sensor(var, config)

    cg.add(var.set_use_latte(config[USE_LATTE]))
    cg.add(parent.add_status_sensor(var))
