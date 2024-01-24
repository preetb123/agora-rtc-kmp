package com.example.ui

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.PaddingValues
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.material.Button
import androidx.compose.material.MaterialTheme
import androidx.compose.material.Text
import androidx.compose.material.TextField
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.input.TextFieldValue
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import co.touchlab.kermit.Logger
import com.dokar.sonner.ToastType
import com.dokar.sonner.rememberToasterState
import moe.tlaster.precompose.PreComposeApp
import moe.tlaster.precompose.viewmodel.viewModel
import org.jetbrains.compose.resources.ExperimentalResourceApi

@OptIn(ExperimentalResourceApi::class)
@Composable
fun App() {
  PreComposeApp {
    MaterialTheme {
      LoginPage()
    }
  }
}

@Composable
fun LoginPage() {
  Column(
    modifier = Modifier
      .fillMaxSize()

      .verticalScroll(state = rememberScrollState())
      .background(Color.White),
    verticalArrangement = Arrangement.Center,
    horizontalAlignment = Alignment.CenterHorizontally,
  ) {
    Text(text = "Agora Voice call", fontWeight = FontWeight.Bold, fontSize = 24.sp)
    Spacer(modifier = Modifier.height(16.dp))
    Text(text = "Compose multiplatform", fontSize = 18.sp)
    Spacer(modifier = Modifier.height(50.dp))
    InputFields()
  }
}

@Composable
private fun InputFields() {
  val model: MainViewModel = viewModel(modelClass = MainViewModel::class, keys = listOf("key")) {
    MainViewModel()
  }
  val channelNameState = remember { mutableStateOf(TextFieldValue()) }
  val toaster = rememberToasterState()
  var info = remember { mutableStateOf("") }

  LaunchedEffect(info) {
    model.someLiveData.collect {
      Logger.i(it)
      toaster.show(it)
      info.value = it
    }
  }

  Column(
    modifier = Modifier.padding(horizontal = 20.dp),
    verticalArrangement = Arrangement.Center,
  ) {
    TextField(
      value = channelNameState.value,
      onValueChange = { channelNameState.value = it },
      label = { Text("Channel Name ") },
      placeholder = { Text("Enter channel name") },
      modifier = Modifier
        .align(
          alignment = Alignment.CenterHorizontally
        )
        .fillMaxWidth()
    )
    Spacer(modifier = Modifier.height(16.dp))

  }

  Spacer(modifier = Modifier.height(60.dp))

  Button(
    onClick = {
      model.joinCall(channelNameState.value.text)
      // toaster.show("testing", ToastType.Normal)
    },
    contentPadding = PaddingValues(
      horizontal = 20.dp,
      vertical = 10.dp
    )
  ) {
    Text(text = "Join", fontWeight = FontWeight.Bold, fontSize = 18.sp)
  }

  Spacer(modifier = Modifier.height(100.dp))

  Button(
    onClick = {
      model.endCall()
    },
    contentPadding = PaddingValues(
      horizontal = 20.dp,
      vertical = 10.dp
    )
  ) {
    Text(text = "End Call", fontWeight = FontWeight.Bold, fontSize = 18.sp)
  }

  Spacer(modifier = Modifier.height(32.dp))
  Text(text = info.value, fontWeight = FontWeight.Bold, fontSize = 18.sp )
}
