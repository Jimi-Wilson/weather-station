from django.urls import path, include
from .views import GetLatestWeatherDataView, GetWeatherBetweenDates, GetRecentWeatherDataView, GetWeatherDataStatsView

urlpatterns = [
    path(
        'latest/',
        GetLatestWeatherDataView.as_view(),
        name='latest-weather-data'
    ),

    path(
        'range/',
        GetWeatherBetweenDates.as_view(),
        name='weather-data-between-datetime'
    ),

    path(
        'recent/',
        GetRecentWeatherDataView.as_view(),
        name='recent-weather-data'
    ),

    path(
        'stats/',
        GetWeatherDataStatsView.as_view(),
        name='stats-weather-data'
    )


]