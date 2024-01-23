package com.example.addlibs

import io.agora.rtc2.ChannelMediaOptions
import io.agora.rtc2.Constants
import io.agora.rtc2.IRtcEngineEventHandler
import io.agora.rtc2.RtcEngine
import kotlinx.coroutines.CancellationException
import kotlinx.coroutines.cancel
import kotlinx.coroutines.channels.awaitClose
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.callbackFlow
import java.lang.RuntimeException

class AgoraRtcAndroid : AgoraRTC {
  private lateinit var rtcEngine: RtcEngine
  private val rtcEventHandler = object : IRtcEngineEventHandler() {
    override fun onJoinChannelSuccess(channel: String?, uid: Int, elapsed: Int) {
      super.onJoinChannelSuccess(channel, uid, elapsed)
      joinChannelFlow.tryEmit(JoinChannelListenerEvent.OnJoinSuccess(channel, uid, elapsed))
    }

    override fun onLeaveChannel(stats: RtcStats?) {
      super.onLeaveChannel(stats)
      leaveChannelFlow.tryEmit(
        LeaveChannelListenerEvent.OnLeaveChannelSuccess(
          CallStats(
            stats?.totalDuration ?: 0
          )
        )
      )
    }
  }

  override fun initialize(appId: String): Flow<RTCEngineEvent> = callbackFlow {
    try {
      rtcEngine = RtcEngine.create(AppContext.get(), appId, rtcEventHandler)
      trySend(RTCEngineEvent.OnRTCEngineInitialized)
    } catch (exception: Exception) {
      cancel(CancellationException("Error creating rtcClient ${exception.message}"))
      throw RuntimeException("Error while initializing rtcEngine: ${exception.message}")
    }
    awaitClose {
      rtcEngine.removeHandler(rtcEventHandler)
    }
  }

  private lateinit var joinChannelFlow: MutableStateFlow<JoinChannelListenerEvent>
  private lateinit var leaveChannelFlow: MutableStateFlow<LeaveChannelListenerEvent>

  override fun joinChannel(
    token: String,
    channelId: String,
    agoraId: Int
  ): Flow<JoinChannelListenerEvent> {
    joinChannelFlow = MutableStateFlow(JoinChannelListenerEvent.OnJoinChannelCalled)
    val options = ChannelMediaOptions()
    options.autoSubscribeAudio = true
    options.clientRoleType = Constants.CLIENT_ROLE_BROADCASTER
    options.channelProfile = Constants.CHANNEL_PROFILE_LIVE_BROADCASTING
    val res = rtcEngine.joinChannel(token, channelId, agoraId, options)
    if (res < 0) {
      joinChannelFlow.tryEmit(JoinChannelListenerEvent.OnJoinChannelError(res, ""))
    }
    return joinChannelFlow
  }

  override fun leaveChannel(): Flow<LeaveChannelListenerEvent> {
    leaveChannelFlow = MutableStateFlow(LeaveChannelListenerEvent.OnLeaveChannelInitiated)
    val res = rtcEngine.leaveChannel()
    if (res < 0) {
      leaveChannelFlow.tryEmit(LeaveChannelListenerEvent.OnLeaveChannelError(res, ""))
    }
    return leaveChannelFlow
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

  override fun destroy() {
    RtcEngine.destroy()
  }
}

actual fun getAgoraRTC(): AgoraRTC = AgoraRtcAndroid()