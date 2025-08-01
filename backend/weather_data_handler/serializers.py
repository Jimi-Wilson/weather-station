from rest_framework import serializers
from .models import WeatherDataEntry


class WeatherDataSerializer(serializers.ModelSerializer):
    class Meta:
        model = WeatherDataEntry
        fields = ['temperature', 'humidity', 'pressure']


class WeatherStatsDetailSerializer(serializers.Serializer):
    average = serializers.FloatField()
    maximum = serializers.FloatField()
    minimum = serializers.FloatField()


class WeatherStatsSerializer(serializers.Serializer):
    period_hours = serializers.IntegerField()
    count = serializers.IntegerField()

    temperature = WeatherStatsDetailSerializer()
    humidity = WeatherStatsDetailSerializer()
    pressure = WeatherStatsDetailSerializer()
