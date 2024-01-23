package com.example.addlibs

import io.agora.rtc2.RtcEngine

class AndroidPlatform : Platform {
  override val name: String =
    "Android ${android.os.Build.VERSION.SDK_INT}, sdkVersion: ${RtcEngine.getSdkVersion()}"
}

actual fun getPlatform(): Platform = AndroidPlatform()