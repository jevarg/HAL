from .hal import HALClientConfigEntry, CeilingFanLight
from .const import DOMAIN

from homeassistant.core import HomeAssistant
from homeassistant.helpers.entity_platform import AddConfigEntryEntitiesCallback

async def async_setup_entry(
    hass: HomeAssistant,
    config_entry: HALClientConfigEntry,
    async_add_entities: AddConfigEntryEntitiesCallback,
) -> None:
    """Set up the demo light platform."""
    async_add_entities(
        [
            CeilingFanLight(
                unique_id="ceiling_fan_light",
                device_name="Ceiling fan light",
                domain=DOMAIN,
                hal=config_entry.runtime_data,
            ),
        ]
    )
