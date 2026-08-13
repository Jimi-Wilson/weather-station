from rest_framework_api_key.permissions import BaseHasAPIKey
from .models import WeatherStationAPIKey


class HasWeatherStationAPIKey(BaseHasAPIKey):
    model = WeatherStationAPIKey
