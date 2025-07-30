from django.contrib import admin

from .models import WeatherDataEntry


@admin.register(WeatherDataEntry)
class WeatherStationAdmin(admin.ModelAdmin):
    pass
