package com.example.addlibs

import platform.UIKit.UIDevice
import cocoapods.AgoraRtcEngine_iOS.AgoraRtcEngineKit
import platform.Foundation.version

class IOSPlatform : Platform {
  override val name: String = UIDevice.currentDevice.systemName() +
      " " + UIDevice.currentDevice.systemVersion +
      " sdk version: " + AgoraRtcEngineKit.description()
}

actual fun getPlatform(): Platform = IOSPlatform()