package com.example.addlibs

import cocoapods.AgoraRtcEngine_iOS.AgoraChannelStats
import cocoapods.AgoraRtcEngine_iOS.AgoraRtcChannelMediaOptions
import cocoapods.AgoraRtcEngine_iOS.AgoraRtcEngineDelegateProtocol
import cocoapods.AgoraRtcEngine_iOS.AgoraRtcEngineKit
import kotlinx.coroutines.channels.awaitClose
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.callbackFlow
import platform.darwin.NSInteger
import platform.darwin.NSObject
import platform.darwin.NSUInteger

class AgoraRtcIOS: AgoraRTC {

    private lateinit var rtcEngineKit: AgoraRtcEngineKit
    override fun initialize(appId: String): Flow<RTCEngineEvent> = callbackFlow {
        val delegate : AgoraRtcEngineDelegateProtocol = object : AgoraRtcEngineDelegateProtocol, NSObject() {
            override fun rtcEngine(
                engine: AgoraRtcEngineKit,
                didJoinChannel: String,
                withUid: NSUInteger,
                elapsed: NSInteger
            ) {
                joinChannelFlow.tryEmit(JoinChannelListenerEvent.OnJoinSuccess(didJoinChannel, withUid.toInt(), elapsed.toInt()))
            }

            override fun rtcEngine(
                engine: AgoraRtcEngineKit,
                didLeaveChannelWithStats: AgoraChannelStats
            ) {
                leaveChannelFlow.tryEmit(LeaveChannelListenerEvent.OnLeaveChannelSuccess(CallStats(didLeaveChannelWithStats.duration.toInt())))
            }
        }
        rtcEngineKit = AgoraRtcEngineKit.sharedEngineWithAppId(appId, delegate)

        awaitClose {  }
    }

    private lateinit var joinChannelFlow: MutableStateFlow<JoinChannelListenerEvent>
    private lateinit var leaveChannelFlow: MutableStateFlow<LeaveChannelListenerEvent>

    override fun joinChannel(token: String, channelId: String, agoraId: Int): Flow<JoinChannelListenerEvent> {
        joinChannelFlow = MutableStateFlow(JoinChannelListenerEvent.OnJoinChannelCalled)
        val options = AgoraRtcChannelMediaOptions()
        val res = rtcEngineKit.joinChannelByToken(token, channelId, "", null)
        if(res < 0) {
            joinChannelFlow.tryEmit(JoinChannelListenerEvent.OnJoinChannelError(res, ""))
        }
        return joinChannelFlow
    }

    override fun leaveChannel(): Flow<LeaveChannelListenerEvent> {
        leaveChannelFlow = MutableStateFlow(LeaveChannelListenerEvent.OnLeaveChannelInitiated)
        val res = rtcEngineKit.leaveChannel(null)
        if(res < 0){
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
        AgoraRtcEngineKit.destroy()
    }
}

actual fun getAgoraRTC(): AgoraRTC = AgoraRtcIOS()