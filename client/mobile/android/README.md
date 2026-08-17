# NYX Mobile Native Bridge

This directory contains the Android-side contract for the future native SA-MP/RakNet adapter.

## Integration layers

1. **GameMode Pawn** remains authoritative for RP rules, money, jobs, organizations, vehicles and persistence.
2. **Native Android bridge** receives normalized player/vehicle/chat/connection events.
3. **Godot adapter** (`client/godot/scripts/nyx_native_bridge.gd`) converts native callbacks into engine signals.
4. **Godot sync layer** (`nyx_sync.gd`) interpolates remote state and owns the prototype presentation layer.
5. **GTA asset loader** will replace prototype meshes when DFF/TXD/COL/IDE/IPL/MAP packages are supplied.

## RakNet boundary

The current branch does **not** pretend to implement RakNet. The `NyxNativeBridge` Kotlin class is a stable ABI-facing contract. The production adapter must implement the actual SA-MP packet/session handshake and map packets into the normalized state structures before enabling the Godot singleton.

## Android requirements

- ARM64 primary target.
- Internet/network-state permissions enabled by the Godot export preset.
- Keep the bridge transport-independent so launcher updates do not change the GameMode contract.
