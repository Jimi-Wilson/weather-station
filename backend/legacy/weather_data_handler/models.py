from django.db import models
from rest_framework_api_key.models import AbstractAPIKey


class WeatherStation(models.Model):
    device_id = models.CharField(max_length=100, unique=True)
    name = models.CharField(max_length=200)
    rainfall_calibration_factor = models.FloatField()
    created_at = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return f"{self.name} ({self.device_id})"


class WeatherStationAPIKey(AbstractAPIKey):
    station = models.ForeignKey(WeatherStation, on_delete=models.CASCADE, related_name='api_keys')

    class Meta(AbstractAPIKey.Meta):
        verbose_name = "Weather Station API Key"
        verbose_name_plural = "Weather Station API Keys"


class UploadBatch(models.Model):
    station = models.ForeignKey(WeatherStation, on_delete=models.CASCADE, related_name='batches')
    bucket_tips = models.PositiveIntegerField()
    rainfall_mm = models.FloatField(blank=True)  # rainfall per hour
    created_at = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return f"Upload batch for {self.station.name} at {self.created_at}"

    def difference_to_previous_batch(self):
        previous_batch = UploadBatch.objects.filter(
            station=self.station,
            created_at__lt=self.created_at
        ).order_by('-created_at').first()

        if not previous_batch:
            return None

        try:
            latest_reading = self.readings.latest('timestamp')
            previous_reading = previous_batch.readings.latest('timestamp')
        except Reading.DoesNotExist:
            return None

        diffs = {
            "temperature": None,
            "humidity": None,
            "pressure": None,
        }

        if latest_reading.temperature is not None and previous_reading.temperature is not None:
            diffs['temperature'] = latest_reading.temperature - previous_reading.temperature

        if latest_reading.humidity is not None and previous_reading.humidity is not None:
            diffs['humidity'] = latest_reading.humidity - previous_reading.humidity

        if latest_reading.pressure is not None and previous_reading.pressure is not None:
            diffs['pressure'] = latest_reading.pressure - previous_reading.pressure

        return diffs

class Reading(models.Model):
    batch = models.ForeignKey(UploadBatch, on_delete=models.CASCADE, related_name='readings')
    timestamp = models.DateTimeField(db_index=True)
    temperature = models.FloatField(null=True, blank=True)
    humidity = models.FloatField(null=True, blank=True)
    pressure = models.FloatField(null=True, blank=True)

    class Meta:
        ordering = ['-timestamp']
        indexes = [
            models.Index(fields=['timestamp']),
        ]

    def __str__(self):
        return f"Weather reading {self.batch.station.name} for at {self.timestamp}"
