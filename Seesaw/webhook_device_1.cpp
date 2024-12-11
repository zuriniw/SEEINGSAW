{
    "name": "doPair",
    "event": "doBlue_1",
    "template": "webhook",
    "url": "https://api.particle.io/v1/devices/events",
    "requestType": "POST",
    "noDefaults": true,
    "rejectUnauthorized": true,
    "unchunked": false,
    "data_url_response_event": false,
    "form": {
        "name": "doBlue_1",
        "private": "false",
        "access_token": "-----device_2's token-----",       // replace it into device_2's Particle token
        "data": "{{{PARTICLE_EVENT_VALUE}}}"
    }
}