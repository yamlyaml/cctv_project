package com.github.thibaultbee.srtplayer

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import com.github.thibaultbee.srtplayer.databinding.ActivitySrtplayerBinding
import com.github.thibaultbee.srtplayer.player.SrtDataSource


class SRTplayerActivity : AppCompatActivity() {

    private lateinit var binding: ActivitySrtplayerBinding
    private val viewModel: SRTplayerActivityViewModel by lazy {
        ViewModelProvider(this).get(SRTplayerActivityViewModel::class.java)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivitySrtplayerBinding.inflate(layoutInflater)
        setContentView(binding.root)

        supportFragmentManager
            .beginTransaction()
            .replace(R.id.settings, SettingsFragment())
            .commit()

        binding.playerView.player = viewModel.player

        binding.updateButton.setOnClickListener {
            viewModel.player.release()
            binding.playerView.player = viewModel.player


       }

    }

    override fun onResume() {
        super.onResume()
        binding = ActivitySrtplayerBinding.inflate(layoutInflater)
        setContentView(binding.root)

        supportFragmentManager
            .beginTransaction()
            .replace(R.id.settings, SettingsFragment())
            .commit()

        binding.playerView.player = viewModel.player

        binding.updateButton.setOnClickListener {
            viewModel.player.release()
            binding.playerView.player = viewModel.player


        }
    }

    override fun onStop() {
        super.onStop()
        viewModel.player.release()
    }
}

