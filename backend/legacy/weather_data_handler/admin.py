from django.contrib import admin
from rest_framework_api_key.admin import APIKeyModelAdmin

from .models import Reading, UploadBatch, WeatherStation, WeatherStationAPIKey


@admin.register(Reading)
class ReadingAdmin(admin.ModelAdmin):
    pass


@admin.register(UploadBatch)
class UploadBatchAdmin(admin.ModelAdmin):
    pass


@admin.register(WeatherStation)
class WeatherStationAdmin(admin.ModelAdmin):
    pass


@admin.register(WeatherStationAPIKey)
class WeatherStationAPIKeyAdmin(APIKeyModelAdmin):
    pass
