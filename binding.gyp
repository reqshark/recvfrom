{'targets': [
    {
        'target_name': 'recvfrom',
        'include_dirs': [
            "<!(node -e \"require('nan')\")",
        ],
        'sources': [ 'binding.cc' ],
    }
]}
