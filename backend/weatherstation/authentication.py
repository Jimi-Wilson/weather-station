import secrets

from django.contrib.auth.hashers import check_password
from rest_framework.authentication import BaseAuthentication
from rest_framework.exceptions import AuthenticationFailed

from weatherstation.models import Device

PAIRING_CODE_ALPHABET = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789"
PAIRING_CODE_LENGTH = 8
PAIRING_CODE_EXPIRY_MINUTES = 15

def generate_pairing_code():
    # Generating code and checking for collisions
    while True:
        code = "".join(
            secrets.choice(PAIRING_CODE_ALPHABET)
            for _ in range(PAIRING_CODE_LENGTH)
        )

        if not Device.objects.filter(pairing_code=code).exists():
            break

    return code

API_KEY_PREFIX_LENGTH = 8
API_KEY_SECRET_LENGTH = 32

PREFIX_ALPHABET = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789"

def generate_api_key():
    while True:
        prefix = "".join(
            secrets.choice(PREFIX_ALPHABET)
            for _ in range(API_KEY_PREFIX_LENGTH)
        )

        if not Device.objects.filter(api_key_prefix=prefix).exists():
            break

    secret = secrets.token_urlsafe(API_KEY_SECRET_LENGTH)

    return f"{prefix}.{secret}", prefix


class DeviceAuthentication(BaseAuthentication):
    def authenticate(self, request):
        token = request.headers.get("X-Device-Key")

        if not token:
            return None

        prefix = token.split(".")[0]
        device = Device.objects.get(api_key_prefix=prefix)

        if not check_password(token, device.api_key_hash):
            raise AuthenticationFailed("Invalid API key")

        return (device, token)
