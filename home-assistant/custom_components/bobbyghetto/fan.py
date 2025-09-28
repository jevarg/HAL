"""Ceiling fan using HAL protocol."""

from .hal import HALClientConfigEntry, CeilingFan
from .const import DOMAIN

from homeassistant.core import HomeAssistant
from homeassistant.helpers.entity_platform import AddConfigEntryEntitiesCallback

async def async_setup_entry(
    hass: HomeAssistant,
    config_entry: HALClientConfigEntry,
    async_add_entities: AddConfigEntryEntitiesCallback,
) -> None:
    """Set up the Demo config entry."""
    async_add_entities([
        CeilingFan(
            unique_id="ceiling_fan",
            device_name="Ceiling Fan",
            domain=DOMAIN,
            hal=config_entry.runtime_data,
        )
    ])
