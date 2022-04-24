package com.example.mmoapp

import android.app.Fragment
import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.FragmentActivity
import androidx.fragment.app.FragmentManager
import androidx.fragment.app.FragmentTransaction
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.room.Room
import okhttp3.*
import okhttp3.internal.cache.CacheInterceptor
import org.json.JSONArray
import java.io.File
import java.io.IOException


class AnimeActivity : FragmentActivity(), ListUser {

    var anime: MutableList<Anime> = mutableListOf<Anime>()
    lateinit var client: OkHttpClient
    lateinit var adapter: AnimeAdapter



    lateinit var db: AnimeDatabase
    lateinit var animeDao: AnimeDao
    lateinit var fragment: ListFragment
    lateinit var fm: FragmentManager
    lateinit var tr: FragmentTransaction

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_anime)

        fragment = ListFragment()
        fm = supportFragmentManager
        tr = fm.beginTransaction()
        tr.add(R.id.list_fragment, fragment)
        tr.commitAllowingStateLoss()

//
//        var httpCacheDirectory: File = File(applicationContext.cacheDir, "http-cache")
//        var cacheSize = 300L * 1024 * 1024 // 300 MiB
//        client = OkHttpClient.Builder()
//        .cache(Cache(
//            directory = httpCacheDirectory,
//            maxSize = cacheSize // 10 MiB
//        ))
//        .build()
//        adapter = AnimeAdapter(client, anime, {position -> onListItemClick(position) })
//
//        db = Room.databaseBuilder(applicationContext, AnimeDatabase::class.java, "anime").build()
//        animeDao = db.animeDao()
//
//        val recyclerView = findViewById<RecyclerView>(R.id.main_recycler)
//        recyclerView.layoutManager = GridLayoutManager(this, resources.getInteger(R.integer.nrows))
//        recyclerView.adapter = adapter
//
//
//        downloadContent()

    }

    private fun onListItemClick(position: Int) {

        val intent = Intent(this, FactsActivity::class.java)
        //intent.putExtra("id", anime[position].get_id() );
        intent.putExtra("id", position);
        startActivity(intent)

    }


    private fun downloadContent() {

        val request = Request.Builder()
            .url("https://www.mmobomb.com/api1/games")
            .build()

        client.newCall(request).enqueue(object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                e.printStackTrace()
            }

            override fun onResponse(call: Call, response: Response) {
                response.use {

                    if (!response.isSuccessful) throw IOException("Unexpected code $response")

                    //val jsonResponse = JSONArray(response.body!!.string()).getJSONObject(0)

                    val jsonAnime: JSONArray = JSONArray(response.body!!.string())

                    var anime: MutableList<Anime> = animeDao.getAll()

                    if (jsonAnime.length() != anime.size) {

                        anime = mutableListOf<Anime>()
                        for (i in 0 until jsonAnime.length()) {

                            val jsonAnimeItem = jsonAnime.getJSONObject(i)

//                        downloadImage(
//                            Anime(
//                                jsonAnimeItem.getInt("anime_id"),
//                                jsonAnimeItem.getString("anime_name")
//                            ),
//                            jsonAnimeItem.getString("anime_img")
//                        )
                            /*adapter.addData(
                                Anime(
                                    jsonAnimeItem.getInt("id"),
                                    jsonAnimeItem.getString("title"),
                                    jsonAnimeItem.getString("thumbnail")
                                )
                            )*/
                            val id = jsonAnimeItem.getInt("id")
                            val title = jsonAnimeItem.getString("title")
                            val url = jsonAnimeItem.getString("thumbnail")
                            val platform = jsonAnimeItem.getString("platform")
                            val genre = jsonAnimeItem.getString("genre")
                            val status = ""//jsonAnimeItem.getString("status")
                            val developer = jsonAnimeItem.getString("developer")
                            val release = jsonAnimeItem.getString("release_date")
                            val description = jsonAnimeItem.getString("short_description")

                            var obj: Anime = Anime(
                                id,
                                title,
                                url
                            )

                            obj.set_genre(genre)
                            obj.set_platform(platform)
                            obj.set_developer(developer)
                            obj.set_description(description)
                            obj.set_status(status)
                            obj.set_release(release)

                            anime.add(obj)

                        }

                        animeDao.insertAll(anime)

                    }

                    for (i in 0 until anime.size) {

                        adapter.addData(
                            anime[i]
                        )

                    }
                    this@AnimeActivity.runOnUiThread(Runnable {
                        adapter.notifyDataSetChanged()
                    })


                }

            }

        })

    }

    override fun selectItem(id: Int) {
        this.onListItemClick(id)
    }

}