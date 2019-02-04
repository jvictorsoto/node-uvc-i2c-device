{
  'targets': [
    {
      'target_name': 'uvcI2cDeviceBinding',
      'defines': [ 'V8_DEPRECATION_WARNINGS=1' ],
      'sources': [ 'uvcI2cDevice.cc', 'UVCLinux.h' ],
      'libraries': [ '-ldl', '-luvclinuxPD570' ]
    }
  ]
}
