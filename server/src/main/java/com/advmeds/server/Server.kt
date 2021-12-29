package com.advmeds.server

object Server {

    fun loadSo() {
        System.loadLibrary("advmedsServer")
    }

    external fun nativeStartServer(port: Int)

    external fun nativeStopServer()

}