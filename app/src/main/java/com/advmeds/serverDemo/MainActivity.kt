package com.advmeds.serverDemo

import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.padding
import androidx.compose.material.Button
import androidx.compose.material.MaterialTheme
import androidx.compose.material.Surface
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.advmeds.server.Server
import com.advmeds.serverDemo.ui.theme.ServerDemoTheme
import java.net.HttpURLConnection
import java.net.URL

class MainActivity : ComponentActivity() {
    private val tag = "serverDemo"
    private val serverIp = "127.0.0.1"
    private val port = 9999
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Server.loadSo()

        setContent {
            ServerDemoTheme {
                // A surface container using the 'background' color from the theme
                Surface(color = MaterialTheme.colors.background) {
                    Greeting (testConnection = {
                        startConnection()
                    },
                        startServer = {
                            Server.startServer(port)
                        },
                        stopServer = {
                        Server.stopServer()
                    })
                }
            }
        }
    }

    private fun startConnection() {
        Thread{
            kotlin.runCatching {
                val url = URL("http://$serverIp:$port")
                val connection = url.openConnection() as HttpURLConnection
             /*   connection.doOutput = true
                connection.doInput = true*/
                connection.requestMethod = "GET"
                connection.setRequestProperty("test", "test value")
                connection.connect()
                Log.d(tag, "code: ${connection.responseCode}")
            }.onFailure {
                Log.e(tag, "error", it)
            }

        }.start()

    }
}

@Composable
fun Greeting(
    testConnection: () -> Unit,
    startServer: () -> Unit,
    stopServer: () -> Unit
) {
    Column(modifier = Modifier.padding(8.dp)) {
        Button(
            modifier = Modifier.padding(8.dp),
            onClick = testConnection) {
            Text(text = "test connection")
        }

        Button(
            modifier = Modifier.padding(8.dp),
            onClick = startServer
        ) {
            Text(text = "start server")
        }

        Button(
            modifier = Modifier.padding(8.dp),
            onClick = stopServer
        ) {
            Text(text = "stop server")
        }
    }

}

@Preview(showBackground = true)
@Composable
fun DefaultPreview() {
    ServerDemoTheme {
//        Greeting {}
    }
}