from rest_framework import viewsets, generics
from rest_framework.exceptions import ValidationError
from rest_framework.response import Response
from rest_framework.views import APIView
from rest_framework import status

from .models import WeatherDataEntry
from .pagination import StandardResultsSetPagination
from .serializers import WeatherDataSerializer

from datetime import datetime


class WeatherDataViewSet(viewsets.ModelViewSet):
    queryset = WeatherDataEntry.objects.all()
    serializer_class = WeatherDataSerializer


class GetLatestWeatherDataView(generics.ListAPIView):
    queryset = WeatherDataEntry.objects.all()
    serializer_class = WeatherDataSerializer

    def get_queryset(self):
        return WeatherDataEntry.objects.all().latest('timestamp')


class GetWeatherBetweenDates(generics.ListAPIView):
    serializer_class = WeatherDataSerializer
    pagination_class = StandardResultsSetPagination

    def get_queryset(self):
        queryset = WeatherDataEntry.objects.all()

        start_datetime_str = self.request.query_params.get("start_datetime")
        end_datetime_str = self.request.query_params.get("end_datetime")

        if not start_datetime_str or not end_datetime_str:
            raise ValidationError(
                {"detail": "start_datetime and end_datetime are required"}
            )

        try:
            start_datetime = datetime.fromisoformat(start_datetime_str)
            end_datetime = datetime.fromisoformat(end_datetime_str)
            return queryset.filter(timestamp__range=(start_datetime, end_datetime))
        except ValueError:
            raise ValidationError({
                {"detail": "Invalid datetime format. Please use ISO 8601 format."}
            })

