from rest_framework import serializers
from .models import WeatherDataEntry


class WeatherDataSerializer(serializers.ModelSerializer):
    class Meta:
        model = WeatherDataEntry
        fields = '__all__'
        read_only_fields = ('timestamp',)
