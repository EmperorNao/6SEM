package com.example.mmoapp

import android.content.Intent
import android.os.Bundle
import androidx.fragment.app.FragmentActivity
import androidx.fragment.app.FragmentManager
import androidx.fragment.app.FragmentTransaction
import okhttp3.*
import org.json.JSONArray
import java.io.IOException


class GameActivity : FragmentActivity(), ListUser {


    private lateinit var fragment: ListFragment
    private lateinit var fm: FragmentManager
    private lateinit var tr: FragmentTransaction


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_game)

        fragment = ListFragment()
        fm = supportFragmentManager
        tr = fm.beginTransaction()
        tr.add(R.id.list_fragment, fragment)
        tr.commitAllowingStateLoss()


    }

    private fun onListItemClick(position: Int) {

        val intent = Intent(this, FactsActivity::class.java)
        intent.putExtra("id", position);
        startActivity(intent)

    }

    override fun selectItem(id: Int) {
        this.onListItemClick(id)
    }

}