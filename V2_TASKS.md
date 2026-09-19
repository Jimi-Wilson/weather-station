# Weather Station v2 — Minimum Scope

## Goal

Turn the personal weather station into a multi-user platform where people can create an account, connect and manage their own weather stations, receive readings over MQTT, and optionally share a read-only station page.

## Definition of done

V2 is complete when this journey works end to end:

> Alice creates an account, configures a new ESP32, pairs it using the code shown by the device, names the station, sees its MQTT readings on her dashboard, makes it public, and Bob opens its read-only page without logging in.

If work does not directly support this journey, it is not required for v2.

---

## Task 1: Complete account authentication

Implement the minimum account flow needed to own and manage stations.

### Requirements

- [ ] Add account registration with email and password.
- [ ] Add login, token refresh, and logout flows.
- [ ] Store passwords through Django's authentication system.
- [ ] Add frontend forms and authenticated-session handling.
- [ ] Protect station-management pages from unauthenticated users.
- [ ] Return clear validation and authentication errors.
- [ ] Add backend tests for registration, login, and protected endpoints.

### Acceptance criteria

- A new user can register, log in, refresh their session, and log out.
- An unauthenticated visitor cannot access station-management endpoints or pages.
- One user cannot access another user's private station data.

### Not included

Email verification, password reset, social login, profiles, avatars, and roles.

---

## Task 2: Complete secure device registration

Allow a known physical device to exchange its factory credentials for operational credentials.

### Requirements

- [ ] Create devices through an admin/management command with a UUID and one-time registration secret.
- [ ] Allow the ESP32 to exchange those credentials for device credentials.
- [ ] Store only hashed secrets in the backend.
- [ ] Store the returned credentials persistently on the ESP32.
- [ ] Make registration retries safe and define what happens when a device registers twice.
- [ ] Reject unknown, disabled, or incorrectly authenticated devices.
- [ ] Add tests for successful, repeated, and rejected registration.

### Acceptance criteria

- A provisioned ESP32 can register without manual database editing.
- A device with invalid credentials cannot register.
- A registered device can restart without losing its operational credentials.

### Not included

Public device creation, automated manufacturing workflows, credential rotation UI, and fleet provisioning.

---

## Task 3: Complete station pairing and ownership

Let a logged-in user claim a physical station using a short-lived code displayed by the device.

### Requirements

- [ ] Allow an authenticated device to request a short, expiring pairing code.
- [ ] Display the pairing code on the ESP32 screen.
- [ ] Add a frontend form for a logged-in user to enter the code.
- [ ] Assign the device to the user after a valid claim.
- [ ] Invalidate the code immediately after use.
- [ ] Let the ESP32 detect when pairing succeeds.
- [ ] Prevent a device from being claimed by more than one user.
- [ ] Rate-limit claim attempts and test expired, invalid, and reused codes.

### Acceptance criteria

- A logged-in user can claim an unowned station using the displayed code.
- Invalid, expired, or already-used codes are rejected.
- A claimed station appears in the owner's station list.

### Not included

Shared ownership, invitations, organizations, and ownership transfers.

---

## Task 4: Add authenticated MQTT ingestion

Use MQTT only for device-to-server weather telemetry while keeping HTTP for account, provisioning, pairing, and dashboard requests.

### Requirements

- [ ] Define one topic pattern: `stations/{device_id}/readings`.
- [ ] Define and document a versioned reading payload.
- [ ] Include a unique reading or batch ID and measurement timestamp.
- [ ] Publish temperature, humidity, and pressure with QoS 1.
- [ ] Disable anonymous broker access.
- [ ] Give each device unique broker credentials.
- [ ] Restrict each device to publishing only to its own topic.
- [ ] Subscribe from the backend and validate every message.
- [ ] Verify that the publishing device exists and is active.
- [ ] Store readings idempotently so duplicate delivery is safe.
- [ ] Update the device's `last_seen` after a valid message.
- [ ] Log malformed, unauthorized, and rejected messages.
- [ ] Add integration tests covering valid, duplicate, malformed, and unauthorized messages.

### Acceptance criteria

- An authenticated station can publish a reading that appears once in the database.
- Re-delivering the same QoS 1 message does not create a duplicate.
- A device cannot publish data for another station.
- Anonymous clients cannot publish.

### Not included

Server-to-device commands, configuration topics, retained readings, MQTT over WebSockets, QoS 2, broker clustering, and dead-letter infrastructure.

---

## Task 5: Preserve readings through connection failures

Make telemetry delivery resilient without attempting exactly-once transport.

### Requirements

- [ ] Store unsent readings in the ESP32's local filesystem.
- [ ] Retry publishing after the MQTT connection returns.
- [ ] Keep a reading until its QoS 1 publish is acknowledged.
- [ ] Preserve original measurement timestamps during retries.
- [ ] Place a documented bound on local storage and define overflow behavior.
- [ ] Test restart, Wi-Fi loss, broker loss, and duplicate-delivery scenarios.

### Acceptance criteria

- Readings captured during a temporary outage are uploaded after reconnection.
- Device restarts do not silently discard buffered readings.
- Retries do not create duplicate database records.

---

## Task 6: Add the “My Stations” management page

Give each user a minimal place to manage the stations they own.

### Requirements

- [ ] List all stations owned by the current user.
- [ ] Show each station's name, last reading time, and online/stale state.
- [ ] Allow the owner to rename a station.
- [ ] Allow the owner to unpair/remove a station with confirmation.
- [ ] Scope every endpoint and database query to the authenticated owner.
- [ ] Add authorization tests using at least two different users.

### Acceptance criteria

- A user can list, rename, view, and unpair their own stations.
- A user cannot see or modify another user's private stations.
- Stale stations are visibly distinguishable from recently active stations.

### Scope definition

For v2, “manage” means exactly: **list, rename, view, and unpair**.

### Not included

Remote settings, calibration, remote commands, OTA updates, health telemetry, and ownership transfers.

---

## Task 7: Build a per-station weather dashboard

Adapt the existing dashboard so it displays data for a selected station instead of global data.

### Requirements

- [ ] Add owner-authorized latest-reading and recent-reading endpoints per station.
- [ ] Show the latest temperature, humidity, and pressure.
- [ ] Show recent charts using a fixed 24-hour range.
- [ ] Show the time of the last reading.
- [ ] Show clear empty, loading, error, and stale/offline states.
- [ ] Ensure readings from different stations can never be mixed.
- [ ] Add API authorization and data-isolation tests.

### Acceptance criteria

- Selecting a station displays only that station's readings.
- A station with no readings has a useful empty state.
- A station that has stopped reporting shows a stale/offline state.

### Not included

Custom date ranges, advanced smoothing controls, comparison views, forecasts, and live WebSocket updates.

---

## Task 8: Add optional public station pages

Provide the smallest credible community feature: an owner can publish a read-only station page.

### Requirements

- [ ] Add a private/public visibility setting to each station.
- [ ] Default new stations to private.
- [ ] Let only the owner change visibility.
- [ ] Give public stations a stable, shareable URL.
- [ ] Show station name, approximate location, latest readings, last-updated time, and a 24-hour chart.
- [ ] Never expose device credentials, owner email, or an exact private address.
- [ ] Return not-found for private stations requested through the public endpoint.
- [ ] Add public/private authorization tests.

### Acceptance criteria

- A visitor without an account can view a public station URL.
- That visitor cannot view a private station or management controls.
- Switching a public station back to private immediately removes public access.

### Not included

Maps, nearby-station search, a public directory, comments, follows, likes, messaging, and social profiles.

---

## Task 9: Integrate and document the complete v2 deployment

Make the full system reproducible and demonstrate the architectural trade-offs.

### Requirements

- [ ] Run the database, Django API, MQTT subscriber, and Mosquitto together in the deployment configuration.
- [ ] Move credentials and environment-specific settings out of source code.
- [ ] Add health/restart behavior for long-running services.
- [ ] Document local setup, device provisioning, pairing, and the MQTT topic/payload contract.
- [ ] Document why MQTT is used for telemetry and HTTP for request-response workflows.
- [ ] Document QoS 1 duplicate handling, offline buffering, and security boundaries.
- [ ] Add one end-to-end smoke test or repeatable manual test checklist for the definition-of-done journey.

### Acceptance criteria

- A fresh environment can be started from the documented instructions.
- The complete Alice-and-Bob definition-of-done journey succeeds.
- The README accurately describes the implemented system rather than planned features.

---

## Suggested implementation order

1. Complete account authentication.
2. Complete secure device registration.
3. Complete pairing and ownership.
4. Define the reading model and MQTT contract.
5. Implement authenticated MQTT ingestion.
6. Implement device-side MQTT publishing and offline buffering.
7. Build My Stations.
8. Convert the dashboard to per-station data.
9. Add public station pages.
10. Complete deployment, tests, and documentation.

Public pages can be postponed until the rest of the journey works without changing the core architecture.

---

## V2 scope guardrail

Before adding work to v2, ask:

1. Is it required for the definition-of-done journey?
2. Does leaving it out make the system insecure, unreliable, or impossible to demonstrate?
3. Can it be added after v2 without changing the core data model or protocols?

If the answers are **no**, **no**, and **yes**, put it in v2.1 instead.

## V2.1 parking lot

- Interactive public station map and directory
- Nearby-station discovery
- Password reset and email verification
- Shared ownership and invitations
- Ownership transfer
- Alerts and notifications
- Rainfall support
- Data export
- User-facing API access
- Remote station configuration and commands
- OTA firmware updates
- Calibration interface
- Detailed device-health telemetry
- Forecasts and third-party weather data
- Live dashboard updates
- Broker clustering and advanced monitoring
