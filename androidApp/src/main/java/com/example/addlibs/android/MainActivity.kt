package com.example.addlibs.android

import android.os.Bundle
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
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
import androidx.compose.material3.*
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.platform.LocalLifecycleOwner
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.input.TextFieldValue
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.lifecycle.viewmodel.viewModelFactory
import com.example.addlibs.AgoraRTCClient

class MainActivity : ComponentActivity() {
  override fun onCreate(savedInstanceState: Bundle?) {
    super.onCreate(savedInstanceState)
    setContent {
      MyApplicationTheme {
        LoginPage()
      }
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
private fun InputFields(
  model: MainViewModel = viewModel<MainViewModel>()
) {
  val lifecycleOwner = LocalLifecycleOwner.current
  val channelNameState = remember { mutableStateOf(TextFieldValue()) }

  val context = LocalContext.current
  LaunchedEffect(key1 = context) {
    model.someLiveData.observe(lifecycleOwner) {
      Toast
        .makeText(context, it, Toast.LENGTH_SHORT)
        .show()
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
}

@Composable
fun GreetingView(text: String) {
  Text(text = text)
}

@Preview
@Composable
fun DefaultPreview() {
  MyApplicationTheme {
    LoginPage()
  }
}
