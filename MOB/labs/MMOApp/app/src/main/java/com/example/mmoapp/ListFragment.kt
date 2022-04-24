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

    fun selectItem(id: Int)

}


class ListFragment: Fragment() {

    private lateinit var client: OkHttpClient
    private lateinit var adapter: GameAdapter


    private lateinit var db: GameDatabase
    private lateinit var gameDao: GameDao
    private lateinit var activity: ListUser


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
            throw ClassCastException("$activity must implement ListUser")
        }

    }

    override fun onStart() {
        super.onStart()

        client = OkHttpClient.Builder().cache(
                Cache(
                directory = File(requireActivity().applicationContext.cacheDir, "http-cache"),
                maxSize = 300L * 1024 * 1024 // 300 MiB
                )
            ).build()


        adapter = GameAdapter(client)
        { position -> activity.selectItem(adapter.getData()[position].get_id()) }


        db = Room.databaseBuilder(requireActivity().applicationContext, GameDatabase::class.java, "Game").build()
        gameDao = db.gameDao()


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

            }

            override fun onResponse(call: Call, response: Response) {
                response.use {

                    if (!response.isSuccessful) throw IOException("Unexpected code $response")

                    val jsonData = JSONArray(response.body!!.string())
                    var games: MutableList<Game> = gameDao.getAll()

                    if (jsonData.length() != games.size) {

                        games = mutableListOf()
                        for (i in 0 until jsonData.length()) {

                            val jsonGame = jsonData.getJSONObject(i)

                            val id = jsonGame.getInt("id")
                            val title = jsonGame.getString("title")
                            val url = jsonGame.getString("thumbnail")
                            val platform = jsonGame.getString("platform")
                            val genre = jsonGame.getString("genre")
                            val status = ""//jsonGame.getString("status")
                            val developer = jsonGame.getString("developer")
                            val release = jsonGame.getString("release_date")
                            val description = jsonGame.getString("short_description")

                            val obj = Game(
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

                            games.add(obj)

                        }

                        gameDao.insertAll(games)

                    }

                    adapter.setData(games)
                    requireActivity().runOnUiThread {
                        adapter.notifyDataSetChanged()
                    }

                }

            }

        })

    }



}