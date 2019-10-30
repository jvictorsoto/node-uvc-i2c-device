{
  'targets': [
    {
      'target_name': 'uvcI2cDeviceBinding',
      'defines': [ 'V8_DEPRECATION_WARNINGS=1' ],
      'sources': [ 'UVCLinuxControl.cpp', 'UVCLinux.cpp', 'uvcI2cDevice.cpp', 'UVCLinuxControl.h', 'UVCLinux.h' ],
      'libraries': [ '-ldl' ]
    }
  ]
}
