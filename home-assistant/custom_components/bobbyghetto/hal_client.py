from asyncio import Lock
from enum import IntEnum
import logging
from uuid import UUID

from bleak import BleakClient
from bleak.backends.device import BLEDevice
from bleak.exc import BleakError

from homeassistant.components import bluetooth

_LOGGER = logging.getLogger(__name__)

class HALClient:
    """HAL Bluetooth low-energy client."""

    def __init__(self, ble_device: BLEDevice) -> None:
        """Initialize the HAL client with the specified MAC address."""

        self._lock = Lock()
        self._device = ble_device
        self._client: BleakClient | None = None

    async def update(self) -> None:
        _LOGGER.debug("update")
        pass

    async def stop(self) -> None:
        _LOGGER.debug("Stop called")
        if not self._client or not self._client.is_connected:
            _LOGGER.debug("Client disconnected or null")
            return

        _LOGGER.info("Disconnecting")
        await self._client.disconnect()
    
    async def write_characteristic(self, characteristic: UUID, value: int):
        _LOGGER.debug("Writing characteristic=%s value=%02x", characteristic, value)
        await self._refresh_client()
        
        if not self._client:
            _LOGGER.warning("Client undefined")
            return
        
        try:
            await self._client.write_gatt_char(characteristic, value.to_bytes())
        except:
            _LOGGER.error("Write failed!", exc_info=True)

    async def _refresh_client(self) -> None:
        async with self._lock:
            if self._client:
                _LOGGER.debug("Reusing BLE client")
                return

            _LOGGER.info("Connecting to %s", self._device.address)
            try:
                self._client = BleakClient(self._device, pair=True)
                await self._client.connect()
            except:
                _LOGGER.error("Error on connect", exc_info=True)

            _LOGGER.debug("Connected!")
