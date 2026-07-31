from django.urls import path
from weatherstation.views import RegisterDevice, ClaimStation


urlpatterns = [
    path("device/register", RegisterDevice.as_view()),
    path("station/claim", ClaimStation.as_view()),
]
