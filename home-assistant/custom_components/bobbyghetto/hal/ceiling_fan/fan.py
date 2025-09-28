import logging
from typing import Any

from homeassistant.helpers.device_registry import DeviceInfo
from ..client import HALClient
from .common import CHARACTERISTIC, FanCommand
from homeassistant.components.fan import DIRECTION_FORWARD, FanEntity, FanEntityFeature
from homeassistant.util.percentage import int_states_in_range

_LOGGER = logging.getLogger(__name__)

SUPPORTED_FEATURES = (
    FanEntityFeature.TURN_OFF   |
    FanEntityFeature.TURN_ON    |
    FanEntityFeature.SET_SPEED  |
    FanEntityFeature.DIRECTION
)

class CeilingFan(FanEntity):
    """A demonstration fan component that uses legacy fan speeds."""

    _attr_should_poll = False
    _attr_translation_key = "fan"

    def __init__(
        self,
        unique_id: str,
        device_name: str,
        domain: str,
        hal: HALClient,
    ) -> None:
        """Initialize the entity."""

        self._unique_id: str = unique_id
        self._attr_name: str = device_name
        self._hal: HALClient = hal

        self._attr_supported_features = SUPPORTED_FEATURES
        self._percentage: int = 0
        self._prev_percentage: int = 100

        self._preset_modes = None
        self._direction = DIRECTION_FORWARD
        
        self._attr_device_info = DeviceInfo(
            identifiers={
                # Serial numbers are unique identifiers within a specific domain
                (domain, self.unique_id)
            },
            name=device_name,
        )

    @property
    def unique_id(self) -> str:
        """Return the unique id."""
        return self._unique_id
    
    @property
    def percentage(self) -> int | None:
        """Return the current speed."""
        _LOGGER.debug("percentage=%d", self._percentage)
        return self._percentage

    @property
    def speed_count(self) -> int:
        """Return the number of speeds the fan supports."""
        return int_states_in_range(FanCommand.speed_range())
    
    @property
    def current_direction(self) -> str | None:
        """Fan direction."""
        return self._direction
    
    @property
    def is_on(self) -> bool:
        return self._percentage > 0
    
    async def async_set_percentage(self, percentage: int) -> None:
        """Set the speed of the fan, as a percentage."""
        _LOGGER.debug("called async_set_percentage percentage=%d", percentage)

        if percentage == self._percentage:
            return

        try:
            command: FanCommand
            if self.is_on and percentage == 0:
                command = FanCommand.FAN_TOGGLE
            else:
                command = FanCommand.from_speed(percentage)

            await self._hal.write_characteristic(CHARACTERISTIC, command)
        except:
            _LOGGER.error("Failed to set fan speed", exc_info=True)
            return

        if percentage > 0:
            self._prev_percentage = self._percentage

        self._percentage = percentage
        self.async_write_ha_state()

    async def async_turn_on(
        self,
        percentage: int | None = None,
        preset_mode: str | None = None,
        **kwargs: Any,
    ) -> None:
        """Turn on the entity."""
        _LOGGER.debug("called async_turn_on")
        await self.async_set_percentage(self._prev_percentage)

    async def async_turn_off(self, **kwargs: Any) -> None:
        """Turn off the entity."""
        _LOGGER.debug("called async_turn_off")
        await self.async_set_percentage(0)

    async def _async_toggle(self, **kwargs: Any) -> None:
        """Toggle the fan."""
        _LOGGER.debug("called async_toggle")

        if self.is_on:
            await self.async_turn_off()
        else:
            await self.async_turn_on()

    async def async_set_direction(self, direction: str) -> None:
        """Set the direction of the fan."""
        _LOGGER.debug("called async_set_direction")

        if self._direction == direction:
            return

        try:
            await self._hal.write_characteristic(CHARACTERISTIC, FanCommand.FAN_DIRECTION)
        except:
            _LOGGER.error("Failed to change direction to %s", direction)
            return

        self._direction = direction
        self.async_write_ha_state()
