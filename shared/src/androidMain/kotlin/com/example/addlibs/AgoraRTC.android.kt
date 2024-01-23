package com.example.addlibs

class AgoraRtcAndroid : AgoraRTC {
    override fun initialize() {
        TODO("Not yet implemented")
    }

    override fun joinChannel(token: String, channelId: String, agoraId: Int) {
        TODO("Not yet implemented")
    }

    override fun adjustPlaybackSignalVolume(volume: Int) {
        TODO("Not yet implemented")
    }

    override fun adjustRecordingSignalVolume(volume: Int) {
        TODO("Not yet implemented")
    }

    override fun setParameters(params: Map<String, String>) {
        TODO("Not yet implemented")
    }

    override fun enableAudioVolumeIndication(interval: Int, smooth: Int, reportVad: Boolean) {
        TODO("Not yet implemented")
    }
}

actual fun getAgoraRTC(): AgoraRTC = AgoraRtcAndroid()