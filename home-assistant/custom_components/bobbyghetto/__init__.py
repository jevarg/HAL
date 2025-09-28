"""The hal integration."""

import logging

from .hal import HALClient, HALClientConfigEntry

from homeassistant.components import bluetooth
from homeassistant.const import CONF_ADDRESS, Platform
from homeassistant.core import HomeAssistant
from homeassistant.exceptions import ConfigEntryNotReady

_PLATFORMS: list[Platform] = [Platform.FAN, Platform.LIGHT]
_LOGGER = logging.getLogger(__name__)

async def async_setup_entry(hass: HomeAssistant, entry: HALClientConfigEntry) -> bool:
    """Set up hal from a config entry."""

    address: str = entry.data[CONF_ADDRESS]
    ble_device = bluetooth.async_ble_device_from_address(hass, address.upper(), True)
    if not ble_device:
        raise ConfigEntryNotReady(
            f"Could not find HAL Device with address {address}"
        )

    _LOGGER.info("Found HAL device at %s", address)
    entry.runtime_data = HALClient(ble_device)
    await hass.config_entries.async_forward_entry_setups(entry, _PLATFORMS)

    return True


# TODO Update entry annotation
async def async_unload_entry(hass: HomeAssistant, entry: HALClientConfigEntry) -> bool:
    """Unload a config entry."""

    return await hass.config_entries.async_unload_platforms(entry, _PLATFORMS)
