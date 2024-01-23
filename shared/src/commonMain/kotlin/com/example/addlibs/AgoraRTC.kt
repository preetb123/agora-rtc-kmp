package com.example.addlibs

interface AgoraRTC {
    fun initialize()
    fun joinChannel(token: String, channelId: String, agoraId: Int)
    fun adjustPlaybackSignalVolume(volume: Int)
    fun adjustRecordingSignalVolume(volume: Int)
    fun setParameters(params: Map<String, String>)
    fun enableAudioVolumeIndication(interval: Int, smooth: Int, reportVad: Boolean)
}

expect fun getAgoraRTC(): AgoraRTC