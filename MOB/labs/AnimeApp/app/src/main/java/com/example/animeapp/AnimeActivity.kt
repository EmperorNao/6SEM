package com.example.animeapp

import android.content.Intent
import android.graphics.BitmapFactory
import android.os.Bundle
import android.text.Html
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.RecyclerView
import okhttp3.*
import org.json.JSONArray
import org.json.JSONObject
import java.io.IOException
import kotlin.collections.Map
import kotlin.collections.MutableList
import kotlin.collections.MutableMap
import kotlin.collections.mutableListOf
import kotlin.collections.mutableMapOf
import kotlin.collections.set
import kotlin.collections.toTypedArray


class AnimeActivity : AppCompatActivity() {

    var anime: MutableList<Anime> = mutableListOf<Anime>()
    var adapter: AnimeAdapter = AnimeAdapter(anime, {position -> onListItemClick(position) })
    val client = OkHttpClient()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_anime)

        val recyclerView = findViewById<RecyclerView>(R.id.main_recycler)
        recyclerView.layoutManager = GridLayoutManager(this, resources.getInteger(R.integer.nrows))
        recyclerView.adapter = adapter

        downloadContent()

    }

    private fun onListItemClick(position: Int) {

        val intent = Intent(this, FactsActivity::class.java)
        intent.putExtra("anime_name", anime[position].get_name() );
        startActivity(intent)

    }


    private fun downloadContent() {

        val request = Request.Builder()
            .url("https://anime-facts-rest-api.herokuapp.com/api/v1")
            .build()

        client.newCall(request).enqueue(object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                e.printStackTrace()
            }

            override fun onResponse(call: Call, response: Response) {
                response.use {

                    if (!response.isSuccessful) throw IOException("Unexpected code $response")

                    val jsonResponse = JSONObject(response.body!!.string())

                    val jsonAnime: JSONArray = jsonResponse.getJSONArray("data")
                    for (i in 0 until jsonAnime.length()) {

                        val jsonAnimeItem = jsonAnime.getJSONObject(i)

//                        downloadImage(
//                            Anime(
//                                jsonAnimeItem.getInt("anime_id"),
//                                jsonAnimeItem.getString("anime_name")
//                            ),
//                            jsonAnimeItem.getString("anime_img")
//                        )
                        adapter.addData(
                            Anime(
                                jsonAnimeItem.getInt("anime_id"),
                                jsonAnimeItem.getString("anime_name"),
                                jsonAnimeItem.getString("anime_img")
                            )
                        )
                        this@AnimeActivity.runOnUiThread(Runnable {
                            adapter.notifyDataSetChanged()
                        })

                    }

                }

            }

        })

    }

}