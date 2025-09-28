import logging
from typing import Any

from ..client import HALClient
from .common import CHARACTERISTIC

from homeassistant.components.light import ColorMode, LightEntity
from homeassistant.helpers.device_registry import DeviceInfo

_LOGGER = logging.getLogger(__name__)
_LIGHT_TOGGLE_COMMAND = 8

class CeilingFanLight(LightEntity):
    """Ceiling fan light."""

    _attr_has_entity_name = True
    _attr_name = None
    _attr_should_poll = False

    def __init__(
        self,
        unique_id: str,
        device_name: str,
        domain: str,
        hal: HALClient
    ) -> None:
        """Initialize the light."""
        self._available = True
        self._is_on = False
        self._unique_id = unique_id
        self._color_modes = {ColorMode.BRIGHTNESS}
        self._hal = hal

        self._attr_device_info = DeviceInfo(
            identifiers={
                # Serial numbers are unique identifiers within a specific domain
                (domain, self.unique_id)
            },
            name=device_name,
        )

    @property
    def unique_id(self) -> str:
        """Return unique ID for light."""
        return self._unique_id

    @property
    def available(self) -> bool:
        """Return availability."""
        return True

    @property
    def color_mode(self) -> str | None:
        """Return the color mode of the light."""
        return ColorMode.ONOFF

    @property
    def is_on(self) -> bool:
        """Return true if light is on."""
        return self._is_on

    @property
    def supported_color_modes(self) -> set[ColorMode]:
        """Flag supported color modes."""
        return {ColorMode.ONOFF}
    
    async def _async_toggle(self) -> None:
        try:
            await self._hal.write_characteristic(CHARACTERISTIC, _LIGHT_TOGGLE_COMMAND)
        except:
            _LOGGER.error("Failed to toggle light!", exc_info=True)

        self._is_on = not self._is_on

        # As we have disabled polling, we need to inform
        # Home Assistant about updates in our state ourselves.
        self.async_write_ha_state()

    async def async_turn_on(self, **kwargs: Any) -> None:
        """Turn the light on."""
        if self._is_on:
            return

        _LOGGER.debug("Turning on")
        await self._async_toggle()

    async def async_turn_off(self, **kwargs: Any) -> None:
        """Turn the light off."""
        if not self._is_on:
            return

        _LOGGER.debug("Turning off")
        await self._async_toggle()
