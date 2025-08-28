from .models import WeatherStationAPIKey


def get_weather_station_from_request(request):
    auth = request.META.get("HTTP_AUTHORIZATION", "")
    if not auth.startswith("Api-Key "):
        return None

    raw_key = auth.split(" ")[1]

    try:
        api_key = WeatherStationAPIKey.objects.get_from_key(raw_key)
        return api_key.station
    except WeatherStationAPIKey.DoesNotExist:
        return None
