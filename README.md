# Camera Capture Module — Missing Persons Detection

Raspberry Pi camera client for [Missing_Persons_Detection_And_Automated_Reporting_System](https://github.com/Leooruko/Missing_Persons_Detection_And_Automated_Reporting_System). Captures frames from a Pi camera and streams them to the Django backend's frame-ingestion API for face-match processing.

- `camera_agent` — polls the backend readiness endpoint and coordinates capture
- `camera_streamer` — captures and sends frames over HTTP

Built and tested as the hardware-side component of a full-stack academic project combining Django, OpenCV face detection, and embedded C on Raspberry Pi. See the main repo for architecture and setup.
