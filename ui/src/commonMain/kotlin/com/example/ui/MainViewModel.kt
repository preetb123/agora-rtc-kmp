package com.example.ui

import com.example.addlibs.AgoraRTC
import com.example.addlibs.AgoraRTCClient
import com.example.addlibs.JoinChannelListenerEvent
import com.example.addlibs.LeaveChannelListenerEvent
import com.example.addlibs.RTCEngineEvent
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch
import moe.tlaster.precompose.viewmodel.ViewModel
import moe.tlaster.precompose.viewmodel.viewModelScope

class MainViewModel : ViewModel() {
  private var agoraRTCClient: AgoraRTC
  private val _viewState = MutableStateFlow("")
  val someLiveData: StateFlow<String> = _viewState
  private val token = ""

  init {
    agoraRTCClient = AgoraRTCClient().getRTCClient()
     viewModelScope.launch {
      agoraRTCClient.initialize("").collect {
        when (it) {
          RTCEngineEvent.OnRTCEngineInitialized -> {
            _viewState.emit("RTC client initialized")
          }

          is RTCEngineEvent.OnUserOffline -> {

          }
        }
      }
    }
  }

  fun joinCall(channelName: String) {
    viewModelScope.launch {
      agoraRTCClient.joinChannel(token, channelName, 1).collect {
        when (it) {
          JoinChannelListenerEvent.OnJoinChannelCalled -> _viewState.emit("joining call")
          is JoinChannelListenerEvent.OnJoinChannelError -> _viewState.emit("Error joining call ${it.errorCode}")
          is JoinChannelListenerEvent.OnJoinSuccess -> _viewState.emit("Joined channel sucessfully ${it.channel}")
        }
      }
    }
  }

  fun endCall() {
    viewModelScope.launch {
      agoraRTCClient.leaveChannel().collect {
        when (it) {
          is LeaveChannelListenerEvent.OnLeaveChannelError -> _viewState.emit("error ending call ${it.errorCode}")
          LeaveChannelListenerEvent.OnLeaveChannelInitiated -> _viewState.emit("ending call")
          is LeaveChannelListenerEvent.OnLeaveChannelSuccess -> _viewState.emit("call ended successfully. call duration ${it.stats.totalDuration}")
        }
      }
    }
  }
}