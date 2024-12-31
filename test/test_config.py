class Config(object):
    # Labs settings
    labs_config = {
        'duration': 15, # minutes
        'labs': [{
            'lab_name' : 'in4labs_perception',
            'html_name' : 'Perception Laboratory',
            'description' : 'This lab performs sensors experiments on Arduino devices',
            'host_port' : 8001,
            'nat_port' : 8120,
            'cam_url': 'http://62.204.201.51:8100/Mjpeg/1?authToken=2454ef16-84cf-4184-a748-8bddd993c078',
        }],
    }