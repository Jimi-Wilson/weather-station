from django.db import models


class WeatherStation(models.Model):
    device_id = models.CharField(max_length=100, unique=True)
    name = models.CharField(max_length=200)
    rainfall_calibration_factor = models.FloatField()
    created_at = models.DateTimeField(auto_now_add=True)


class UploadBatch(models.Model):
    station = models.ForeignKey(WeatherStation, on_delete=models.CASCADE, related_name='batches')
    bucket_tips = models.PositiveIntegerField()
    rainfall_mm = models.FloatField(blank=True)  # rainfall per hour
    created_at = models.DateTimeField(auto_now_add=True)


class Reading(models.Model):
    batch = models.ForeignKey(UploadBatch, on_delete=models.CASCADE, related_name='readings')
    timestamp = models.DateTimeField(db_index=True, )
    temperature = models.FloatField()
    humidity = models.FloatField()
    pressure = models.FloatField()

    class Meta:
        ordering = ['-timestamp']
        indexes = [
            models.Index(fields=['timestamp']),
        ]

    def __str__(self):
        return f"Weather reading {self.batch.station.name} for at {self.timestamp}"
