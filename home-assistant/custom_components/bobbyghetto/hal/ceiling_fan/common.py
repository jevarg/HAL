import math

from uuid import UUID
from enum import IntEnum, auto
from homeassistant.config_entries import ConfigEntry
from homeassistant.util.percentage import percentage_to_ranged_value

from ..client import HALClient
type HALClientConfigEntry = ConfigEntry[HALClient]

CHARACTERISTIC = UUID("0ffd20c8-7ab7-49a3-aa50-1608ab03bc06")

class FanCommand(IntEnum):
    FAN_TOGGLE = 0
    FAN_DIRECTION = auto()
    FAN_SPEED_1 = auto()
    FAN_SPEED_2 = auto()
    FAN_SPEED_3 = auto()
    FAN_SPEED_4 = auto()
    FAN_SPEED_5 = auto()
    FAN_SPEED_6 = auto()
    LIGHT_TOGGLE = auto()
    LIGHT_INTENSITY = auto()
    
    @staticmethod
    def speed_range():
        return (FanCommand.FAN_SPEED_1, FanCommand.FAN_SPEED_6)

    @staticmethod
    def from_speed(percentage: int):
        value = math.ceil(percentage_to_ranged_value(FanCommand.speed_range(), percentage))
        
        return FanCommand(value)
