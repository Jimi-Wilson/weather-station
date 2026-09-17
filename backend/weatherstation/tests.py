from django.test import TestCase

from weatherstation.models import Device


class DeviceCreationTests(TestCase):
    def test_multiple_unregistered_devices_have_unique_ids_and_null_prefixes(self):
        first_device, _ = Device.create_new()
        second_device, _ = Device.create_new()

        self.assertNotEqual(first_device.id, second_device.id)
        self.assertIsNone(first_device.api_key_prefix)
        self.assertIsNone(second_device.api_key_prefix)
        self.assertEqual(first_device.status, Device.Status.INACTIVE)
        self.assertEqual(second_device.status, Device.Status.INACTIVE)
