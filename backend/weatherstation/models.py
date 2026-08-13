import secrets
import uuid
from users.models import User
from django.contrib.auth.hashers import make_password
from django.db import models

class Device(models.Model):
    class Status(models.TextChoices):
        INACTIVE = "INACTIVE", "Inactive"
        PENDING = "PENDING", "Pending"
        ACTIVE = "ACTIVE", "Active"
        DISABLED = "DISABLED", "Disabled"

    id = models.UUIDField(
        primary_key=True,
        default=uuid.uuid4(),
        editable=False,
    )

    registration_secret_hash = models.CharField(max_length=128)

    api_key_prefix = models.CharField(max_length=8, unique=True)
    api_key_hash = models.CharField(max_length=128)

    pairing_code = models.CharField(max_length=8, unique=True, null=True)

    pairing_code_expires_at = models.DateTimeField(
        null=True,
        blank=True,
    )

    status = models.CharField(
        max_length=10,
        choices=Status.choices,
        default=Status.INACTIVE,
    )

    firmware_version = models.CharField(max_length=20)

    registered_at = models.DateTimeField(null=True, blank=True)
    last_seen = models.DateTimeField(null=True, blank=True)

    created_at = models.DateTimeField(auto_now_add=True)

    @classmethod
    def create_new(cls):
        registration_secret = secrets.token_urlsafe(32)

        device = cls(
            registration_secret_hash=make_password(registration_secret),
            status=cls.Status.PENDING,
        )

        device.save()

        return device, registration_secret


class WeatherStation(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE, null=True)

    device = models.OneToOneField(
        Device,
        on_delete=models.CASCADE
    )

    name = models.CharField(max_length=100)
