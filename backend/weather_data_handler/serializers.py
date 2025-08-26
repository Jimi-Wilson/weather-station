from rest_framework import serializers
from .models import Reading


class _UploadReadingSerializer(serializers.Serializer):
    timestamp = serializers.IntegerField()
    temperature = serializers.FloatField()
    humidity = serializers.FloatField()
    pressure = serializers.FloatField()


class WeatherUploadSerializer(serializers.Serializer):
    device_id = serializers.CharField(max_length=100, unique=True)
    bucket_tips = serializers.IntegerField(min_value=0)

    readings = _UploadReadingSerializer(many=True)


class ReadingSerializer(serializers.ModelSerializer):
    class Meta:
        model = Reading
        fields = ['temperature', 'humidity', 'pressure', 'timestamp']


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
