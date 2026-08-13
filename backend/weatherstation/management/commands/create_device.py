from django.core.management.base import BaseCommand
from weatherstation.models import Device


class Command(BaseCommand):
    help = "Creates a new device"

    def handle(self, *args, **options):
        device, registration_secret = Device.create_new()

        self.stdout.write(
            self.style.SUCCESS("Device created successfully")
        )

        self.stdout.write(f"UUID: {device.id}")
        self.stdout.write(f"Registration Secret: {registration_secret}")