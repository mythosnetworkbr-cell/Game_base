package com.nyx.roleplay.mobile

/**
 * Android-side contract for the future native SA-MP Mobile bridge.
 *
 * This file intentionally contains no Android SDK dependency yet. It can be
 * moved behind a Godot Android plugin/JNI implementation without changing the
 * NYX state contract.
 */
class NyxNativeBridge {
    data class PlayerState(
        val playerId: Int,
        val sequence: Long,
        val x: Float,
        val y: Float,
        val z: Float,
        val rotationY: Float,
        val health: Float,
        val armour: Float,
        val skin: Int,
        val money: Int,
        val bank: Int,
        val jobId: Int,
        val organizationId: Int,
        val organizationRank: Int,
        val adminLevel: Int,
    )

    data class VehicleState(
        val vehicleId: Int,
        val sequence: Long,
        val modelId: Int,
        val x: Float,
        val y: Float,
        val z: Float,
        val rotationX: Float,
        val rotationY: Float,
        val rotationZ: Float,
        val velocityX: Float,
        val velocityY: Float,
        val velocityZ: Float,
        val driverPlayerId: Int,
        val health: Float,
        val fuel: Float,
        val color1: Int,
        val color2: Int,
    )

    interface Listener {
        fun onPlayerState(state: PlayerState)
        fun onVehicleState(state: VehicleState)
        fun onVehicleCreated(state: VehicleState)
        fun onVehicleRemoved(vehicleId: Int)
        fun onChat(message: String)
        fun onConnectionChanged(connected: Boolean, reason: String)
    }

    private var listener: Listener? = null

    fun setListener(listener: Listener?) {
        this.listener = listener
    }

    /** Entry point for the eventual JNI/RakNet adapter. */
    fun submitPlayerState(state: PlayerState) {
        listener?.onPlayerState(state)
    }

    /** Entry point for the eventual JNI/RakNet adapter. */
    fun submitVehicleState(state: VehicleState) {
        listener?.onVehicleState(state)
    }

    fun notifyVehicleCreated(state: VehicleState) {
        listener?.onVehicleCreated(state)
    }

    fun notifyVehicleRemoved(vehicleId: Int) {
        listener?.onVehicleRemoved(vehicleId)
    }

    fun notifyChat(message: String) {
        listener?.onChat(message)
    }

    fun notifyConnection(connected: Boolean, reason: String) {
        listener?.onConnectionChanged(connected, reason)
    }
}
