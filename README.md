# ReFreeLook
AMX Mod X module whichs allows spectating admins to use any camera modes whatever `mp_forcecamera` or `mp_forcechasecam` value.
This only works for latest [ReGameDLL_CS](https://github.com/s1lentq/ReGameDLL_CS) version.

Originally created by [Arkshine](https://github.com/Arkshine/AdminFreeLook).

##Configuration (cvars)

* __`afl_admin_access_flags`__

  Admin's access flags.
  Ability to observe without being restricted by gameplay cvar.
  Flags are additives.

  Default flags are (from `users.ini` file) :

  ```
  ; Access flags:
  ; a - immunity (can't be kicked/baned/slayed/slaped and affected by other commmands)
  ; b - reservation (can join on reserved slots)
  ; c - amx_kick command
  ; d - amx_ban and amx_unban commands (permanent and temporary bans)
  ; e - amx_slay and amx_slap commands
  ; f - amx_map command
  ; g - amx_cvar command (not all cvars will be available)
  ; h - amx_cfg command
  ; i - amx_chat and other chat commands
  ; j - amx_vote and other vote commands
  ; k - access to sv_password cvar (by amx_cvar command)
  ; l - access to amx_rcon command and rcon_password cvar (by amx_cvar command)
  ; m - custom level A (for additional plugins)
  ; n - custom level B
  ; o - custom level C
  ; p - custom level D
  ; q - custom level E
  ; r - custom level F
  ; s - custom level G
  ; t - custom level H
  ; u - menu access
  ; v - amx_ban and amx_unban commands (temporary bans only, about amx_unban, only self performed ban during map gonna be allowed)
  ; z - user (no admin)
  ```

  Default value : *"d"*

##Installation

1. Make sure that you have ReGameDLL_CS, more details [here](https://github.com/s1lentq/ReGameDLL_CS#how-can-use-it).
2. Stop your server whatever it's your first install or just updating.
3. From [Releases section](https://github.com/s1lentq/ReFreeLook/releases/latest), get the binary file you need:
<table>
	<tbody>
		<tr>
			<th align="center">OS</th>
			<th align="center">Windows</th>
			<th align="center">Linux</th>
		</tr>
		<tr>
			<td align="center">Binary file</td>
			<td align="center">`refreelook_amxx.dll`</td>
			<td align="center">`refreelook_amxx_i386.so`</td>
		</tr>
	</tbody>
</table>
4. Upload it in your *amxmodx/modules/* folder.
5. Open your *configs/modules.ini* and add at the end `refreelook`
6. Configure `mp_forcechasecam` and `mp_forcecamera` gameplay cvars. Both should have the same value.
7. Make sure `mp_fadetoblack` is set to 0.
8. Configure `afl_admin_access_flags` modules cvar.
9. Start your server.
