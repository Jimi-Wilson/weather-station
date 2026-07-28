# run_mqtt.py

from django.core.management.base import BaseCommand
from django.conf import settings
import paho.mqtt.client as mqtt

class Command(BaseCommand):
    def handle(self, *args, **options):
        def on_connect(client, userdata, flags, rc):
            if rc == 0:
                self.stdout.write(self.style.SUCCESS("Connected to MQTT Broker!"))
                client.subscribe("test")
            else:
                self.stdout.write(self.style.ERROR(f"Failed to connect. Return code: {rc}"))

        def on_message(client, userdata, msg):
            payload = msg.payload.decode("utf-8")
            self.stdout.write(self.style.SUCCESS(f"Topic: {msg.topic} | Message: {payload}"))

        client = mqtt.Client()
        client.on_connect = on_connect
        client.on_message = on_message

        client.connect(settings.MQTT_BROKER, settings.MQTT_PORT, settings.MQTT_KEEPALIVE)

        self.stdout.write(self.style.WARNING("Starting up MQTT network loop..."))
        client.loop_forever()