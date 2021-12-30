package com.advmeds.server

import java.util.concurrent.atomic.AtomicBoolean

object Server {

    private val isStart = AtomicBoolean(false)

    fun loadSo() {
        System.loadLibrary("advmedsServer")
    }

    fun startServer(port: Int) {
        if (isStart.get()) {
            return
        }
        isStart.set(true)
        Thread {
            nativeStartServer(port)
        }.start()

    }

    fun stopServer() {
        nativeStopServer()
        isStart.set(false)
    }

    private external fun nativeStartServer(port: Int)

    private external fun nativeStopServer()

}