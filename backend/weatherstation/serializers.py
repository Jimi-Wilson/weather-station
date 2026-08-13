from rest_framework import serializers



class RegistrationSerializer(serializers.Serializer):
    device_id = serializers.UUIDField()
    registration_secret = serializers.CharField()


class ClaimStationSerializer(serializers.Serializer):
    pairing_code = serializers.CharField(max_length=8)