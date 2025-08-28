from django.http import Http404
from rest_framework import generics, status
from rest_framework.exceptions import ValidationError
from django.db.models import Avg, Max, Min, Count
from rest_framework.response import Response
from rest_framework.views import APIView

from .models import Reading, UploadBatch
from .pagination import StandardResultsSetPagination
from .permissions import HasWeatherStationAPIKey
from .serializers import ReadingSerializer, WeatherStatsSerializer, WeatherUploadSerializer
from .utils import get_weather_station_from_request

from datetime import datetime, timedelta, timezone


class AddWeatherDataView(APIView):
    permission_classes = [HasWeatherStationAPIKey]

    def post(self, request):
        serializer = WeatherUploadSerializer(data=request.data)
        if not serializer.is_valid():
            return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)

        authenticated_station = get_weather_station_from_request(request)
        validated_data = serializer.validated_data

        if authenticated_station.device_id != validated_data['device_id']:
            return Response({"error": "Payload device_id does not match authenticated station."},
                            status=status.HTTP_400_BAD_REQUEST)

        batch = UploadBatch.objects.create(
            station=authenticated_station,
            bucket_tips=validated_data['bucket_tips'],
            rainfall_mm=validated_data['bucket_tips'] * authenticated_station.rainfall_calibration_factor,
        )

        readings_to_create = []
        for reading_data in validated_data['readings']:
            timestamp = datetime.fromtimestamp(reading_data['timestamp'], tz=timezone.utc)
            readings_to_create.append(Reading(
                batch=batch,
                timestamp=timestamp,
                temperature=reading_data['temperature'],
                humidity=reading_data['humidity'],
                pressure=reading_data['pressure'],
            ))

        if readings_to_create:
            Reading.objects.bulk_create(readings_to_create)

        return Response({"status": "success"}, status=status.HTTP_201_CREATED)


class GetLatestWeatherDataView(generics.RetrieveAPIView):
    serializer_class = ReadingSerializer

    def get_object(self):
        try:
            return Reading.objects.latest("timestamp")
        except Reading.DoesNotExist:
            raise Http404


class GetWeatherBetweenDates(generics.ListAPIView):
    serializer_class = ReadingSerializer
    pagination_class = StandardResultsSetPagination

    def get_queryset(self):
        queryset = Reading.objects.all()

        start_datetime_str = self.request.query_params.get("start_datetime")
        end_datetime_str = self.request.query_params.get("end_datetime")

        if not start_datetime_str or not end_datetime_str:
            raise ValidationError(
                {"detail": "start_datetime and end_datetime are required"}
            )

        try:
            start_datetime = datetime.fromisoformat(start_datetime_str)
            end_datetime = datetime.fromisoformat(end_datetime_str)
            return queryset.filter(timestamp__range=(start_datetime, end_datetime)).order_by("-timestamp")
        except ValueError:
            raise ValidationError({
                "detail": "Invalid datetime format. Please use ISO 8601 format."
            })


class GetRecentWeatherDataView(generics.ListAPIView):
    serializer_class = ReadingSerializer
    pagination_class = StandardResultsSetPagination

    def get_queryset(self):
        hours_str = self.request.query_params.get("hours", 24)
        try:
            hours = int(hours_str)

        except ValueError:
            raise ValidationError(
                {"detail": "Query parameter 'hours must be an integer."}
            )

        if hours > 7 * 24:
            raise ValidationError({"detail": "hours cannot be greater than 7 days"})

        since = timezone.now() - timedelta(hours=hours)
        return Reading.objects.filter(timestamp__gte=since).order_by("timestamp")


class GetWeatherDataStatsView(generics.GenericAPIView):

    def get(self, request):
        hours_str = request.query_params.get("hours", 24)

        try:
            hours = int(hours_str)
        except ValueError:
            return Response(
                {"detail": "Query parameter 'hours must be an integer."},
                status=status.HTTP_400_BAD_REQUEST
            )

        since = timezone.now() - timedelta(hours=hours)

        queryset = Reading.objects.filter(timestamp__gte=since)

        if not queryset.exists():
            return Response({
                "detail": "There is no data available for the specified period."
            })

        stats = queryset.aggregate(
            count=Count("id"),
            avg_temperature=Avg("temperature"),
            max_temperature=Max("temperature"),
            min_temperature=Min("temperature"),

            avg_humidity=Avg("humidity"),
            max_humidity=Max("humidity"),
            min_humidity=Min("humidity"),

            avg_pressure=Avg("pressure"),
            max_pressure=Max("pressure"),
            min_pressure=Min("pressure"),
        )

        response_data = {
            "period_hours": hours,
            "count": stats["count"],

            "temperature": {
                "average": stats["avg_temperature"],
                "maximum": stats["max_temperature"],
                "minimum": stats["min_temperature"],
            },

            "humidity": {
                "average": stats["avg_humidity"],
                "maximum": stats["max_humidity"],
                "minimum": stats["min_humidity"],
            },

            "pressure": {
                "average": stats["avg_pressure"],
                "maximum": stats["max_pressure"],
                "minimum": stats["min_pressure"],
            },
        }

        serializer = WeatherStatsSerializer(instance=response_data)
        return Response(serializer.data)
