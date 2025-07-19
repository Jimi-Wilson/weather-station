from django.db import models


class WeatherStation(models.Model):
    location = models.CharField(max_length=100)


class WeatherData(models.Model):
    station = models.ForeignKey(WeatherStation, on_delete=models.CASCADE, related_name='weather_data')
    temperature = models.FloatField()
    humidity = models.FloatField()
    pressure = models.FloatField()
    timestamp = models.DateTimeField(auto_now_add=True)


    def __str__(self):
        return f"Weather Data"

    @property
    def location(self):
        return self.station.location
