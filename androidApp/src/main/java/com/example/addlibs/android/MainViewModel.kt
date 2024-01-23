package com.example.addlibs.android

import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.addlibs.AgoraRTC
import com.example.addlibs.AgoraRTCClient
import com.example.addlibs.JoinChannelListenerEvent
import com.example.addlibs.LeaveChannelListenerEvent
import com.example.addlibs.RTCEngineEvent
import kotlinx.coroutines.flow.collect
import kotlinx.coroutines.launch

class MainViewModel : ViewModel() {
  private var agoraRTCClient: AgoraRTC
  val someLiveData = MutableLiveData<String>()
  private val token =
    "007eJxTYIgT5j+7aIXDU0v1nJTQO3IKfyMWlqnKajGyPct22lp784kCg2WShVGiSaK5oWWqkUlaqpGFQVqqmalRWqqBWZpRmlnS0UXrUxsCGRkW85cwMTIwMrAAMYjPBCaZwSQLlCxJLS5hYAAA300giQ=="

  init {
    agoraRTCClient = AgoraRTCClient().getRTCClient()
    viewModelScope.launch {
      agoraRTCClient.initialize("9b82a4a719e24fe280fe652fe06f2f6b").collect {
        when (it) {
          RTCEngineEvent.OnRTCEngineInitialized -> {
            someLiveData.postValue("RTC client initialized")
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
          JoinChannelListenerEvent.OnJoinChannelCalled -> someLiveData.postValue("joining call")
          is JoinChannelListenerEvent.OnJoinChannelError -> someLiveData.postValue("Error joining call ${it.errorCode}")
          is JoinChannelListenerEvent.OnJoinSuccess -> someLiveData.postValue("Joined channel sucessfully ${it.channel}")
        }
      }
    }
  }

  fun endCall() {
    viewModelScope.launch {
      agoraRTCClient.leaveChannel().collect {
        when (it) {
          is LeaveChannelListenerEvent.OnLeaveChannelError -> someLiveData.postValue("error ending call ${it.errorCode}")
          LeaveChannelListenerEvent.OnLeaveChannelInitiated -> someLiveData.postValue("ending call")
          is LeaveChannelListenerEvent.OnLeaveChannelSuccess -> someLiveData.postValue("call ended successfully. call duration ${it.stats.totalDuration}")
        }
      }
    }
  }
}