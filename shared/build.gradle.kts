plugins {
  alias(libs.plugins.kotlinMultiplatform)
  alias(libs.plugins.kotlinCocoapods)
  alias(libs.plugins.androidLibrary)
}

kotlin {
  androidTarget {
    compilations.all {
      kotlinOptions {
        jvmTarget = "1.8"
      }
    }
  }
  iosX64()
  iosArm64()
  iosSimulatorArm64()

  cocoapods {
    summary = "Some description for the Shared Module"
    homepage = "Link to the Shared Module homepage"
    version = "1.0"
    ios.deploymentTarget = "16.0"
    podfile = project.file("../iosApp/Podfile")
    framework {
      baseName = "shared"
      isStatic = true
    }

    pod("AgoraRtcEngine_iOS") {
      version = "4.2.6"
      moduleName = "AgoraRtcKit"
    }
  }

  sourceSets {
    commonMain.dependencies {
      //put your multiplatform dependencies here
      implementation("org.jetbrains.kotlinx:kotlinx-coroutines-core:1.7.3")
      implementation("co.touchlab:kermit:1.0.0")
    }
    commonTest.dependencies {
      implementation(libs.kotlin.test)
    }
    androidMain.dependencies {
      implementation("io.agora.rtc:voice-sdk:4.2.6")
      implementation("androidx.startup:startup-runtime:1.2.0-alpha02")
    }
    all {
      languageSettings {
        optIn("kotlinx.cinterop.ExperimentalForeignApi")
      }
    }
  }
}

android {
  namespace = "com.example.addlibs"
  compileSdk = 34
  defaultConfig {
    minSdk = 29
  }
}