package com.example.mmoapp

import android.os.AsyncTask
import android.os.Bundle
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.room.Room
import okhttp3.*
import java.io.IOException






class FactsActivity : AppCompatActivity() {

    val facts: MutableList<String> = mutableListOf()
    //val adapter = FactsAdapter(facts)
    var titleView: TextView? = null
    var platformView: TextView? = null
    var genreView: TextView? = null
    var statusView: TextView? = null
    var developerView: TextView? = null
    var releaseView: TextView? = null
    var descriptionView: TextView? = null

    lateinit var db: AnimeDatabase
    lateinit var animeDao: AnimeDao
    val client = OkHttpClient()

    private inner class LoadFileTask : AsyncTask<Int, Void, Anime>() {
        override fun doInBackground(vararg p0: Int?): Anime? {

            var arr: IntArray = IntArray(1)
            arr[0] = p0.get(0) as Int
            val animes = animeDao.loadAllByIds(arr)
            if (animes.isNotEmpty()) {

                val obj = animes[0]

                this@FactsActivity.runOnUiThread(Runnable {
                    titleView?.setText(obj.get_name())
                    platformView?.setText(obj.get_platform())
                    genreView?.setText(obj.get_genre())
                    statusView?.setText(obj.get_status())
                    developerView?.setText(obj.get_developer())
                    releaseView?.setText(obj.get_release())
                    descriptionView?.setText(obj.get_description())
                })

                return obj

            }
            else {

                throw Exception("Don't find anime object with id = $arr[0]")

            }

        }

        override fun onPostExecute(obj: Anime) {

            titleView?.setText(obj.get_name())
            platformView?.setText(obj.get_platform())
            genreView?.setText(obj.get_genre())
            statusView?.setText(obj.get_status())
            developerView?.setText(obj.get_developer())
            releaseView?.setText(obj.get_release())
            descriptionView?.setText(obj.get_description())

        }

    }


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_facts)

        db = Room.databaseBuilder(applicationContext, AnimeDatabase::class.java, "anime").build()
        animeDao = db.animeDao()

        getSupportActionBar()?.setDisplayHomeAsUpEnabled(true);

        //val recyclerView = findViewById<RecyclerView>(R.id.facts_recycler)
        /*recyclerView.layoutManager = LinearLayoutManager(this)
        recyclerView.adapter = adapter*/
        titleView = findViewById(R.id.title) as TextView?
        platformView = findViewById(R.id.platform) as TextView?
        genreView = findViewById(R.id.genre) as TextView?
        statusView = findViewById(R.id.status) as TextView?
        developerView = findViewById(R.id.developer) as TextView?
        releaseView = findViewById(R.id.release) as TextView?
        descriptionView = findViewById(R.id.description) as TextView?

        intent.getIntExtra("id", 0)?.let { downloadContent(it) }

/*        layoutInflater.inflate(
            com.example.animeapp.R.layout.activity_facts,
            parent,
            false
        )*/

    }

    private fun downloadContent(id: Int) {

        val ids = IntArray(1)
        ids[0] = id

        LoadFileTask().execute(id)


        /*val request = Request.Builder()
            .url("https://www.mmobomb.com/api1/game?id=" + id)
            .build()

        client.newCall(request).enqueue(object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                e.printStackTrace()
            }

            override fun onResponse(call: Call, response: Response) {
                response.use {

                    if (!response.isSuccessful) throw IOException("Unexpected code $response")

                    val jsonResponse = JSONObject(response.body!!.string())

                    val title = jsonResponse.getString("title")
                    val platform = jsonResponse.getString("platform")
                    val genre = jsonResponse.getString("genre")
                    val status = jsonResponse.getString("status")
                    val developer = jsonResponse.getString("developer")
                    val release = jsonResponse.getString("release_date")
                    val description = jsonResponse.getString("short_description")



                    this@FactsActivity.runOnUiThread(Runnable {
                        titleView?.setText(title)
                        platformView?.setText(platform)
                        genreView?.setText(genre)
                        statusView?.setText(status)
                        developerView?.setText(developer)
                        releaseView?.setText(release)
                        descriptionView?.setText(description)
                    })
                   *//* val jsonFacts: JSONArray = jsonResponse.getJSONArray("data")

                    this@FactsActivity.runOnUiThread(Runnable {
                        adapter.notifyDataSetChanged()
                    })*//*



                }

            }

        })*/

    }

}