package com.example.mmoapp


import android.content.Context
import android.os.AsyncTask
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.room.Room
import okhttp3.*
import org.json.JSONArray
import java.io.File
import java.io.IOException


interface InfoUser {

    fun get_cur_id(): Int

}


class InfoFragment(): Fragment() {

    lateinit var listener: InfoUser




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

    override fun onCreateView(inflater: LayoutInflater, parent: ViewGroup?,
                              savedInstanceState: Bundle?): View? {
        return inflater.inflate(
            R.layout.info_fragment,
            parent,
            false
        )
    }


    override fun onAttach(context: Context) {
        super.onAttach(context)

        try {
            listener = context as InfoUser
        } catch (e: ClassCastException ) {
            throw ClassCastException(activity.toString() + " must implement ListUser")
        }

    }


    override fun onStart() {
        super.onStart()


        db = Room.databaseBuilder(requireContext(), AnimeDatabase::class.java, "anime").build()
        animeDao = db.animeDao()


        titleView = requireActivity().findViewById(R.id.title) as TextView?
        platformView = requireActivity().findViewById(R.id.platform) as TextView?
        genreView = requireActivity().findViewById(R.id.genre) as TextView?
        statusView = requireActivity().findViewById(R.id.status) as TextView?
        developerView = requireActivity().findViewById(R.id.developer) as TextView?
        releaseView = requireActivity().findViewById(R.id.release) as TextView?
        descriptionView = requireActivity().findViewById(R.id.description) as TextView?

        downloadContent(listener.get_cur_id())

    }

    private inner class LoadFileTask : AsyncTask<Int, Void, Anime>() {
        override fun doInBackground(vararg p0: Int?): Anime? {

            var arr: IntArray = IntArray(1)
            arr[0] = p0.get(0) as Int
            val animes = animeDao.loadAllByIds(arr)
            if (animes.isNotEmpty()) {

                val obj = animes[0]

//
//                titleView?.setText(obj.get_name())
//                platformView?.setText(obj.get_platform())
//                genreView?.setText(obj.get_genre())
//                statusView?.setText(obj.get_status())
//                developerView?.setText(obj.get_developer())
//                releaseView?.setText(obj.get_release())
//                descriptionView?.setText(obj.get_description())


                requireActivity().runOnUiThread(Runnable {

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

    private fun downloadContent(id: Int) {

        val ids = IntArray(1)
        ids[0] = id

        LoadFileTask().execute(id)

    }



}