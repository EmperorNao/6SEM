package com.example.animeapp

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import okhttp3.*
import org.json.JSONArray
import org.json.JSONObject
import java.io.IOException

class FactsActivity : AppCompatActivity() {

    val client = OkHttpClient()
    val facts: MutableList<String> = mutableListOf()
    val adapter = FactsAdapter(facts)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_facts)

        getSupportActionBar()?.setDisplayHomeAsUpEnabled(true);

        val recyclerView = findViewById<RecyclerView>(R.id.facts_recycler)
        recyclerView.layoutManager = LinearLayoutManager(this)
        recyclerView.adapter = adapter
        intent.getStringExtra("anime_name")?.let { downloadContent(it) }

    }

    private fun downloadContent(anime_name: String) {

        val request = Request.Builder()
            .url("https://anime-facts-rest-api.herokuapp.com/api/v1/" + anime_name)
            .build()

        client.newCall(request).enqueue(object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                e.printStackTrace()
            }

            override fun onResponse(call: Call, response: Response) {
                response.use {

                    if (!response.isSuccessful) throw IOException("Unexpected code $response")

                    val jsonResponse = JSONObject(response.body!!.string())

                    val jsonFacts: JSONArray = jsonResponse.getJSONArray("data")
                    for (i in 0 until jsonFacts.length()) {
                        val jsonFact = jsonFacts.getJSONObject(i)

                        adapter.addData(jsonFact.getString("fact"))

                    }
                    this@FactsActivity.runOnUiThread(Runnable {
                        adapter.notifyDataSetChanged()
                    })

                }

            }

        })

    }

}