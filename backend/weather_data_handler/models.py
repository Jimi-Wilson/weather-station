from django.db import models


class WeatherDataEntry(models.Model):
    temperature = models.FloatField()
    humidity = models.FloatField()
    pressure = models.FloatField()
    timestamp = models.DateTimeField(auto_now_add=True)

    class Meta:
        ordering = ['-timestamp']

    def __str__(self):
        return f"Weather Data for at {self.timestamp}"
