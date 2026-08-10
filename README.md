# Camera Capture Module — Missing Persons Detection

**Academic project — hardware component.** Raspberry Pi camera client built
as the embedded-hardware half of a full-stack academic project (Django +
OpenCV face detection + embedded C), demonstrating real-time frame capture
and streaming to a backend matching pipeline.

Captures frames from a Pi camera and streams them to the Django backend's
frame-ingestion API for face-match processing:

- `camera_agent` — polls the backend readiness endpoint and coordinates capture
- `camera_streamer` — captures and sends frames over HTTP

See the main repo, [Missing_Persons_Detection_And_Automated_Reporting_System](https://github.com/Leooruko/Missing_Persons_Detection_And_Automated_Reporting_System), for architecture and setup.
