from django.contrib import admin
from weatherstation.models import Device, WeatherStation
# Register your models here.


admin.site.register(Device)
admin.site.register(WeatherStation)
