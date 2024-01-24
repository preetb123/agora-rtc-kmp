package com.example.addlibs

import android.app.Application
import android.content.Context
import androidx.startup.Initializer

object AppContext {
  private lateinit var application: Application

  fun setup(context: Context) {
    application = context as Application
  }

  fun get(): Context {
    if (::application.isInitialized.not()) throw Exception("Application context isn't initialized")
    return application.applicationContext
  }
}

internal class AppContextInitializer : Initializer<Context> {
  override fun create(context: Context): Context {
    AppContext.setup(context.applicationContext)
    return AppContext.get()
  }

  override fun dependencies(): MutableList<Class<out Initializer<*>>> {
    return mutableListOf()
  }
}