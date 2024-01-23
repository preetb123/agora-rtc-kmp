package com.example.addlibs

import kotlinx.coroutines.flow.Flow

sealed interface RTCEngineEvent {
    data object OnRTCEngineInitialized : RTCEngineEvent
    class OnUserOffline(val uid: Int, val reason: Int): RTCEngineEvent
}

sealed interface JoinChannelListenerEvent {
    data object OnJoinChannelCalled : JoinChannelListenerEvent
    data class OnJoinSuccess(val channel: String?, val uid: Int, val elapsed: Int): JoinChannelListenerEvent
    data class OnJoinChannelError(val errorCode: Int, val errorMessage: String): JoinChannelListenerEvent
}

data class CallStats(val totalDuration: Int)

sealed interface LeaveChannelListenerEvent {
    data object OnLeaveChannelInitiated : LeaveChannelListenerEvent
    data class OnLeaveChannelSuccess(val stats: CallStats): LeaveChannelListenerEvent
    data class OnLeaveChannelError(val errorCode: Int, val errorMessage: String): LeaveChannelListenerEvent
}

interface AgoraRTC {
    fun initialize(appId: String): Flow<RTCEngineEvent>
    fun joinChannel(token: String, channelId: String, agoraId: Int): Flow<JoinChannelListenerEvent>
    fun leaveChannel(): Flow<LeaveChannelListenerEvent>
    fun adjustPlaybackSignalVolume(volume: Int)
    fun adjustRecordingSignalVolume(volume: Int)
    fun setParameters(params: Map<String, String>)
    fun enableAudioVolumeIndication(interval: Int, smooth: Int, reportVad: Boolean)
    fun destroy()
}

expect fun getAgoraRTC(): AgoraRTC