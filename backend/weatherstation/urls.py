from django.urls import path
from weatherstation.views import RegisterDevice, ClaimStation, DevicePairingStatus, PairingCodeView

urlpatterns = [
    path("device/register", RegisterDevice.as_view()),
    path("device/pairing-code", PairingCodeView.as_view()),
    path("device/pairing-code/status", DevicePairingStatus.as_view()),

    path("station/claim", ClaimStation.as_view()),
]
