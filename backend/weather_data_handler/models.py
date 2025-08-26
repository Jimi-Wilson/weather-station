from django.db import models


class WeatherStation(models.Model):
    device_id = models.CharField(max_length=100, unique=True)
    name = models.CharField(max_length=200)
    rainfall_calibration_factor = models.FloatField()
    created_at = models.DateTimeField(auto_now_add=True)
class WeatherDataEntry(models.Model):
    temperature = models.FloatField()
    humidity = models.FloatField()
    pressure = models.FloatField()
    timestamp = models.DateTimeField(auto_now_add=True)

    class Meta:
        ordering = ['-timestamp']
        indexes = [
            models.Index(fields=['timestamp']),
        ]

    def __str__(self):
        return f"Weather Data for at {self.timestamp}"
