from rest_framework.throttling import SimpleRateThrottle


class PairingThrottle(SimpleRateThrottle):
    scope = "pairing"

    def get_cache_key(self, request, view):
        return self.get_ident(request)