class Config(object):
    # Labs settings
    labs_config = {
        'duration': 20, # minutes
        'labs': [{
            'lab_name' : 'in4labs_cybersecurity',
            'html_name' : 'Laboratory of Cybersecurity',
            'description' : 'This lab performs cybersecurity experiments on Arduino devices.',
            'host_port' : 8002,
            'nat_port' : 8120,
            'cam_url': '',
        }],
    }