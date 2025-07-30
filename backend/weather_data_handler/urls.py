from django.urls import path, include
from .views import GetLatestWeatherDataView, GetWeatherBetweenDates

urlpatterns = [
    path(
        'latest-weather-data/',
        GetLatestWeatherDataView.as_view(),
        name='latest-weather-data'
    ),

    path(
        'weather-data-between-datetime/',
        GetWeatherBetweenDates.as_view(),
        name='weather-data-between-datetime'
    )


]