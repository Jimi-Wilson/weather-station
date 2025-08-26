from django.contrib import admin

from .models import Reading


@admin.register(Reading)
class WeatherStationAdmin(admin.ModelAdmin):
    pass
