from datetime import timedelta

from django.contrib.auth.hashers import make_password, check_password
from django.shortcuts import get_object_or_404
from django.utils import timezone
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from rest_framework.views import APIView
from rest_framework_simplejwt.authentication import JWTAuthentication

from weatherstation.authentication import generate_api_key, generate_pairing_code
from weatherstation.models import Device, WeatherStation
from weatherstation.serializers import RegistrationSerializer, ClaimStationSerializer
from weatherstation.throttles import PairingThrottle


class RegisterDevice(APIView):
    def post(self, request):
        serializer = RegistrationSerializer(data=request.data)
        serializer.is_valid(raise_exception=True)

        device = get_object_or_404(Device, id=serializer.validated_data.get("device_id"))

        if device.status == Device.Status.ACTIVE:
            return Response(
                {"error": "Device already registered"},
                status=400
            )

        if not check_password(serializer.validated_data["registration_secret"], device.registration_secret_hash):
            return Response(
                {"error": "Invalid registration secret"},
                status=401
            )

        api_token, prefix = generate_api_key()
        device.api_key_hash = make_password(api_token)
        device.api_key_prefix = prefix
        device.status = Device.Status.ACTIVE
        device.registered_at = timezone.now()

        device.pairing_code_expires_at = (
                timezone.now() + timedelta(minutes=30)
        )

        device.pairing_code = generate_pairing_code()

        device.save()


        return Response({
            "api_key": api_token,
            "pairing_code": device.pairing_code
        })


class ClaimStation(APIView):
    authentication_classes = [JWTAuthentication]
    permission_classes = [IsAuthenticated]
    throttle_classes = [PairingThrottle]

    def post(self, request):
        serializer = ClaimStationSerializer(data=request.data)
        serializer.is_valid(raise_exception=True)

        device = get_object_or_404(Device, pairing_code=serializer.validated_data.get("pairing_code"))

        if device.pairing_code_expires_at is None or device.pairing_code_expires_at < timezone.now():
            return Response(
                {"error": "Pairing code expired"},
                status=400
            )

        weather_station, created = WeatherStation.objects.get_or_create(device=device, defaults={
            "user": request.user
        })

        weather_station.save()

        # Invalidating pairing codes, after station creation
        device.pairing_code = None
        device.pairing_code_expires_at = None
        device.save(update_fields=[
            "pairing_code",
            "pairing_code_expires_at"
        ])


        return Response(status=200)
