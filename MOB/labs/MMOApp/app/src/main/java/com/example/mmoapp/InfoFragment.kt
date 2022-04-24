package com.example.mmoapp


import android.content.Context
import android.os.AsyncTask
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.room.Room


interface InfoUser {

    fun currentId(): Int

}


class InfoFragment : Fragment() {

    private lateinit var listener: InfoUser

    lateinit var titleView: TextView
    lateinit var platformView: TextView
    lateinit var genreView: TextView
    lateinit var statusView: TextView
    lateinit var developerView: TextView
    lateinit var releaseView: TextView
    lateinit var descriptionView: TextView


    private lateinit var db: GameDatabase
    private lateinit var gameDao: GameDao


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

        titleView = requireActivity().findViewById(R.id.title)
        platformView = requireActivity().findViewById(R.id.platform)
        genreView = requireActivity().findViewById(R.id.genre)
        statusView = requireActivity().findViewById(R.id.status)
        developerView = requireActivity().findViewById(R.id.developer)
        releaseView = requireActivity().findViewById(R.id.release)
        descriptionView = requireActivity().findViewById(R.id.description)

        db = Room.databaseBuilder(requireContext(), GameDatabase::class.java, "Game").build()
        gameDao = db.gameDao()

        downloadContent(listener.currentId())

    }


    private inner class LoadFileTask : AsyncTask<Int, Void, Game>() {
        override fun doInBackground(vararg args: Int?): Game {

            val obj = gameDao.loadById(args[0] as Int)
            if (obj != null) {
                return obj
            }
            throw Exception("Don't find id in DB")

        }

        override fun onPostExecute(obj: Game) {

            requireActivity().runOnUiThread {

                titleView.text = obj.get_name()
                platformView.text = obj.get_platform()
                genreView.text = obj.get_genre()
                statusView.text = obj.get_status()
                developerView.text = obj.get_developer()
                releaseView.text = obj.get_release()
                descriptionView.text = obj.get_description()

            }

        }

    }

    private fun downloadContent(id: Int) {
        LoadFileTask().execute(id)
    }

}