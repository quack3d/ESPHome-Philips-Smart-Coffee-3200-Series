import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from esphome.const import CONF_ID
from ..philips_series_3200 import CONTROLLER_ID, PhilipsSeries3200

DEPENDENCIES = ['philips_series_3200']

CONF_ACTION = 'action'

philips_action_button_ns = cg.esphome_ns.namespace(
    'philips_series_3200').namespace('philips_action_button')
ActionButton = philips_action_button_ns.class_(
    "ActionButton", button.Button, cg.Component)

Action = philips_action_button_ns.enum("ActionButton")
ACTIONS = {
    "MAKE_COFFEE": Action.MAKE_COFFEE,
    "SELECT_COFFEE": Action.SELECT_COFFEE,
    "SELECT_ESPRESSO": Action.SELECT_ESPRESSO,
    "MAKE_ESPRESSO": Action.MAKE_ESPRESSO,
    "SELECT_HOT_WATER":   Action.SELECT_HOT_WATER,
    "MAKE_HOT_WATER": Action.MAKE_HOT_WATER,
    "SELECT_STEAM": Action.SELECT_STEAM,
    "MAKE_STEAM": Action.MAKE_STEAM,
    "BEAN": Action.SELECT_BEAN,
    "SIZE": Action.SELECT_SIZE,
    "AQUA_CLEAN": Action.SELECT_AQUA_CLEAN,
    "CALC_CLEAN": Action.SELECT_CALC_CLEAN,
    "PLAY_PAUSE": Action.PLAY_PAUSE,
    "SELECT_AMERICANO": Action.SELECT_AMERICANO,
    "SELECT_CAPPUCCINO": Action.SELECT_CAPPUCCINO,
    "MILK_LEVEL": Action.SELECT_MILK_LEVEL,
}

CONFIG_SCHEMA = button.BUTTON_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(ActionButton),
        cv.Required(CONTROLLER_ID): cv.use_id(PhilipsSeries3200),
        cv.Required(CONF_ACTION): cv.enum(
            ACTIONS, upper=True, space="_"
        )
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONTROLLER_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await button.register_button(var, config)

    cg.add(var.set_action(config[CONF_ACTION]))
    cg.add(parent.add_action_button(var))
