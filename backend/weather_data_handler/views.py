from rest_framework import viewsets
from .models import WeatherDataEntry
from .serializers import WeatherDataSerializer

class WeatherDataViewSet(viewsets.ModelViewSet):
    queryset = WeatherDataEntry.objects.all()
    serializer_class = WeatherDataSerializer