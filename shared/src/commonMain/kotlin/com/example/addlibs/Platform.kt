package com.example.addlibs

interface Platform {
  val name: String
}

expect fun getPlatform(): Platform