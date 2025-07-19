from django.contrib import admin

from api.models import WeatherData, WeatherStation


# Register your models here.
@admin.register(WeatherData)
class WeatherStationAdmin(admin.ModelAdmin):
    pass


@admin.register(WeatherStation)
class WeatherStationAdmin(admin.ModelAdmin):
    pass