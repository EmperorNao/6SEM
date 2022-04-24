package com.example.mmoapp

import android.content.Context
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.room.Room
import okhttp3.*
import org.json.JSONArray
import java.io.File
import java.io.IOException


interface ListUser {

    public fun selectItem(id: Int)

}


class ListFragment: Fragment() {


    var anime: MutableList<Anime> = mutableListOf<Anime>()
    lateinit var client: OkHttpClient
    lateinit var adapter: AnimeAdapter



    lateinit var db: AnimeDatabase
    lateinit var animeDao: AnimeDao
    lateinit var activity: ListUser

    override fun onCreateView(inflater: LayoutInflater, parent: ViewGroup?,
                              savedInstanceState: Bundle?): View? {
        return inflater.inflate(
            R.layout.list_fragment,
            parent,
            false
        )
    }


    override fun onAttach(context: Context) {
        super.onAttach(context)

        try {
            activity = context as ListUser
        } catch (e: ClassCastException ) {
            throw ClassCastException(activity.toString() + " must implement ListUser")
        }
    }


    override fun onStart() {
        super.onStart()

        var httpCacheDirectory: File = File(requireActivity().applicationContext.cacheDir, "http-cache")
        var cacheSize = 300L * 1024 * 1024 // 300 MiB
        client = OkHttpClient.Builder()
            .cache(
                Cache(
                directory = httpCacheDirectory,
                maxSize = cacheSize // 10 MiB
            )
            )
            .build()
        adapter = AnimeAdapter(client, anime, {position -> activity.selectItem(anime[position].get_id()) })

        db = Room.databaseBuilder(requireActivity().applicationContext, AnimeDatabase::class.java, "anime").build()
        animeDao = db.animeDao()

        val recyclerView = requireActivity().findViewById<RecyclerView>(R.id.main_recycler)
        recyclerView.layoutManager = GridLayoutManager(getActivity(), resources.getInteger(R.integer.nrows))
        recyclerView.adapter = adapter

        downloadContent()


    }

    private fun downloadContent() {

        val request = Request.Builder()
            .url("https://www.mmobomb.com/api1/games")
            .build()

        client.newCall(request).enqueue(object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                e.printStackTrace()



                var anime: MutableList<Anime> = animeDao.getAll()

                for (i in 0 until anime.size) {

                    adapter.addData(
                        anime[i]
                    )

                }
                requireActivity().runOnUiThread(Runnable {
                    adapter.notifyDataSetChanged()
                })

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
                    requireActivity().runOnUiThread(Runnable {
                        adapter.notifyDataSetChanged()
                    })

                }

            }

        })

    }



}