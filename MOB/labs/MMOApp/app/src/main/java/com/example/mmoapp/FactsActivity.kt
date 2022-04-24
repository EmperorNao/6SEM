package com.example.mmoapp

import android.os.AsyncTask
import android.os.Bundle
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.room.Room
import okhttp3.*
import java.io.IOException


class FactsActivity : AppCompatActivity(), InfoUser {

    val facts: MutableList<String> = mutableListOf()
    var titleView: TextView? = null
    var platformView: TextView? = null
    var genreView: TextView? = null
    var statusView: TextView? = null
    var developerView: TextView? = null
    var releaseView: TextView? = null
    var descriptionView: TextView? = null

    lateinit var db: AnimeDatabase
    lateinit var animeDao: AnimeDao


    var id: Int = 0
    lateinit var fragment: InfoFragment

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

            id = intent.getIntExtra("id", 0)
           // fragment = InfoFragment(
//            val frag = fragmentManager.findFragmentById(R.id.info_fragment) as InfoFragment

//        db = Room.databaseBuilder(applicationContext, AnimeDatabase::class.java, "anime").build()
//        animeDao = db.animeDao()
//
//        getSupportActionBar()?.setDisplayHomeAsUpEnabled(true);
//
//        titleView = findViewById(R.id.title) as TextView?
//        platformView = findViewById(R.id.platform) as TextView?
//        genreView = findViewById(R.id.genre) as TextView?
//        statusView = findViewById(R.id.status) as TextView?
//        developerView = findViewById(R.id.developer) as TextView?
//        releaseView = findViewById(R.id.release) as TextView?
//        descriptionView = findViewById(R.id.description) as TextView?
//

    }

    private fun downloadContent(id: Int) {

        val ids = IntArray(1)
        ids[0] = id

        LoadFileTask().execute(id)

    }

    override fun get_cur_id(): Int {

        return id

    }

}