<h1 align="center">CS Match BOT</h1>
<p align="center">Counter-Strike 1.6 Match BOT Plugin for Metamod</p>

<p align="center">
    <a href="https://github.com/SmileYzn/MatchBot/issues"><img alt="GitHub Issues" src="https://img.shields.io/github/issues-raw/smileyzn/MatchBot?style=flat-square"></a>
    <a href="https://github.com/SmileYzn/MatchBot/actions"><img alt="GitHub Workflow Status" src="https://img.shields.io/github/actions/workflow/status/SmileYzn/MatchBot/msbuild.yml?branch=main&label=Windows&style=flat-square"></a>
    <a href="https://github.com/SmileYzn/MatchBot/actions"><img alt="GitHub Workflow Status" src="https://img.shields.io/github/actions/workflow/status/smileyzn/MatchBot/makefile.yml?branch=main&label=Linux&style=flat-square"></a>
    <a href="https://github.com/SmileYzn/MatchBot/releases/latest"><img src="https://img.shields.io/github/downloads/SmileYzn/MatchBot/total?label=Download%40latest&style=flat-square&logo=github&logoColor=white" alt="Download"></a>
</p>

<h3>Description</h3>
<p>
This mod allows the server to run a full match without any admin to control the server.<br>
Players must be choose a team, say .ready in chat and wait for server start the match, or just wait for time counter ends.<br>
At the end, server will start a vote map to changelevel to next map automatically.<br>
</p>

<h3>Requirements</h3>
<ul>
    <li>ReHLDS</li>
    <li>ReGameDLL_CS</li>
    <li>Metamod</li>
</ul> 

<h3>Features</h3>
<ul>
<li>Automatic Ready System (Timer based like CS:GO does or Ready System method)</li>
<li>Custom configs changes bettwen match states</li>
<li>Server slots management with spectators and HLTV slots support</li>
<li>Automatic ban players that use rage quit from game</li>
<li>Automatic LO3 (Live on three restarts)</li>
<li>Automatic Teams balancer (Players are balanced when enter in game)</li>
<li>Support for block players pick teams, or ender directly in game</li>
<li>Many commands to admin control the match server and the players</li>
<li>Configurable required players to start match, also minimum players to stop if players left</li>
<li>Game based on max rounds rules (MR15 like)</li>
<li>Mod support overtime rule, sudden death or end directly (In case of tied match)</li>
<li>A complete player (.help) and admin (!help) help system</li>
<li>Player and Admin commands in chat or console</li>
<li>Warmup Rounds with some deathmatch gameplay</li>
<li>Custom map list for vote for next map, teams and more</li>
<li>Mod support player vote kick, timeout pause, vote map, surrender (Match end vote)</li>
<li>Supports captain pickup, random balanced, balance by skill from warmup, swap teams, or kinfe round</li>
<li>Support best of x rounds on start to dettermine who will won the first round (MD3 Like)</li>
<li>Display round damge stats with some commands like .dmg, .hp or .sum and others</li>
<li>Multi language support</li>
<li>Minimalist admin flag system to manage the admins in game</li>
<li>Dead talk support (Dead teammate's can talk)</li>
<li>Anti Flood for chat and radio messages</li>
<li>Mod runs into ReHLDS with ReGameDLL_CS server</li>
<li>Match BOT can run on Windows or Linux servers</li>
<li>Custom server configuration to optmize the game</li>
<li>Other features that is not listed here you can see in your wiki</li>
</ul>

<h3>Match BOT Server Variables</h3>

<details>
  <summary>Click to expand</summary>

| Matchbot.cfg commands list         |  Default value | Description                                    |
| :--------------------------------- | :-----:  | :--------------------------------------------- |
| mb_log_tag                         | BOT      | Match BOT Log Tag. <br/>Here you can put any tag you wish.|
| mb_language                        | en       | Match BOT Language. <br/>`en` English US. <br/>`bp` Brazilian Portuguese.<br/>`es` Spanish Spain.<br/> `ru` Russian RU.<br/> You can edit/create more languages in matchbot\language.json |
| mb_admin_prefix                    | !       | Match BOT administrator command game chat prefix. <br /> For example `!menu` opens administrator menu. |
| mb_player_prefix                   | .       | Match BOT player command game chat prefix. <br /> For example `.dmg` show damage when player dead. |
| mb_players_min                     | 10      | Minimum players needed to start match. |
| mb_players_max                     | 10      | Maximum allowed players in match. |
| mb_play_rounds                     | 30      | Rounds to play before execute overtime.|
| mb_play_rounds_ot                  | 6       | Round to play in overtime. |
| mb_play_ot_mode                    | 3       | Overtime type. <br /> `0` Sudden death round. <br /> `1` Play overtime. <br /> `2` End match tied. <br /> `3` Users vote. |
| mb_ready_type                      | 1       | Ready system type.<br /> `0` Disabled. <br /> `1` Ready System. <br /> `2` Ready Timer. |
| mb_ready_time                      | 60      | Ready system timer delay in seconds. <br /> Only works with `mb_ready_type 2` |
| mb_team_pick_type                  | -1      | Team pickup type when match begin. <br /> `-1` Enable vote. <br /> `0` Leaders. <br /> `1` Random. <br /> `2` None. <br /> `3` Skill balanced. <br /> `4` Swap teams. <br /> `5` Knife round.|
| mb_team_pick_menu                  | abcdef   | Only works with `mb_team_pick_type -1`. <br/>This allows you to make your team pickup menu.<br/> `0` Leaders. <br /> `b` Random. <br /> `c` None. <br /> `d` Skill balanced. <br /> `e` Swap teams. <br /> `f` Knife round|
| mb_vote_map_type                   | 1        | Vote map type. It lets players to choose map or play random map. <br /> `1` Vote map. <br /> `2` Random map. |
| mb_vote_map_auto                   | 2        | Start vote map at match end. <br /> `0` Disabled. <br /> `1` Enabled. <br /> `2` Only when minimum players reached. |
| mb_vote_map_fail                   | 1        | Actions to perform when votemap fails. <br /> `0` Continue match. <br /> `1` Restart vote map. <br /> `2` Choose random map. |
| mb_knife_round                     | 0        | Play Knife Round to choose starting sides. <br /> `0` Disabled. <br /> `1` Enabled.|
| mb_score_type                      | 0        | Scores display method. <br /> `0` Default scores with phrases. <br /> `1` Show all teams and scores. |
| mb_scoreboard_team                 | 1        | Store team scores in scoreboard. <br /> `0` Disabled. <br /> `1` Enabled.|
| mb_scoreboard_player               | 1        | Store player scores in scoreboard. <br /> `0` Disabled. <br /> `1` Enabled.|
| mb_gamename                        | 1        | Display states and scores at game description. <br /> `0` Disabled. <br /> `1` Enabled.|
| mb_player_vote_kick                | 5        | Mininum of players in a team to enable vote kick command for players. <br /> Set to `0` to disable vote kick command.|
| mb_player_vote_map                 | 5        | Mininum of players in a team to enable vote map command for players. <br /> Set to `0` to disable vote map command.|
| mb_player_vote_pause               | 5        | Mininum of players in a team to enable vote pause command for players. <br /> Set to `0` to disable vote pause command.|
| mb_player_vote_restart             | 5        | Mininum of players in a team to enable vote kick command for players. <br /> Set to `0` to disable vote restart command.|
| mb_player_vote_surrender           | 5        | Mininum of players in a team to enable vote kick command for players. <br /> Set to `0` to disable vote surrender command.|
| mb_round_end_stats                 | 0        | Show round stats on end. <br /> `0` Disabled. <br /> `1` Show round damage in chat. <br /> `2` Show round summary in chat. <br /> `3` Show round damage in console. <br /> `4` Show round summary in console. |
| mb_stats_commands                  | abcd     | Enabled round stats commands in chat. <br /> `a` Enable `.hp` command. <br /> `b` Enable `.dmg` command. <br /> `c` Enable `.rdmg` command. <br /> `d` Enable `.sum` command.|
| mb_restrict_weapons                |000000000000000000000000000000000000000| Restricted Weapons by item index slot position (1 to block item, 0 to allow). <br />`0` Shieldgun. <br /> `1` P228. <br /> `2` Glock. <br /> `3` Scout. <br /> `4` Hegrenade. <br /> `5` Xm1014. <br /> `6` C4. <br /> `7` Mac10. <br /> `8` Aug. <br /> `9` Smokegrenade. <br /> `10` Elite. <br /> `11` Fiveseven. <br /> `12` Ump45. <br /> `13` Sg550. <br /> `14` Galil. <br /> `15` Famas. <br /> `16` Usp. <br /> `17` Glock18. <br /> `18` Awp. <br /> `19` Mp5n. <br /> `20` M249. <br /> `21` M3. <br /> `22` M4a1. <br /> `23` Tmp. <br /> `24` G3sg1. <br /> `25` Flashbang. <br /> `26` Deagle. <br /> `27` Sg552. <br /> `28` Ak47. <br /> `29` Knife. <br /> `30` P90. <br /> `31` Nvg. <br /> `32` Defusekit. <br /> `33` Kevlar. <br /> `34` Assault. <br /> `35` Longjump. <br /> `36` Sodacan. <br /> `37` Healthkit. <br /> `38` Antidote. <br /> `39` Battery. |
| mb_extra_smoke_count               | 2        | Extra Smokegranade explosion fix .<br /> `0` Disabled. <br /> `n` Number of extra smoke puffs. |
| mb_pause_time                      | 60.0     | Amount of seconds to pause match. <br /> `0` Disabled. <br /> `n` Or number of seconds to pause the match. |
| mb_retry_mode                      | 0        | Anti reconnect mode. <br /> `0` Disabled. <br /> `1` Enable when player explicity drop from server. <br /> `2` Enable for any disconnect reason. |
| mb_retry_time                      | 30.0     | Anti reconnect time  <br /> Time in seconds to prevent the player reconnect to server |
| mb_help_file                       | cstrike/addons/matchbot/users_help.html     | Users Help File or Website url (Without HTTPS). <br /> If is website url, works only with HTTP (Not HTTPS).|
| mb_help_file_admin                 | cstrike/addons/matchbot/admin_help.html      | Admin Help File or Website url (Without HTTPS). <br /> If is website url, works only with HTTP (Not HTTPS).|
| mb_cfg_match_bot                   | matchbot.cfg  | Match Bot main config. <br /> Executed when Match Bot loads at new map.|
| mb_cfg_warmup                      | warmup.cfg    | Warmup config. <br /> Executed at Warmup session.|
| mb_cfg_start                       | start.cfg     | Start config. <br /> Executed when vote system starts vote teams or vote map.|
| mb_cfg_1st                         | esl.cfg       | First Half config. <br /> Executed when match is live at first half.|
| mb_cfg_halftime                    | halftime.cfg  | Half Time config. <br /> Executed when match is in half time.|
| mb_cfg_2nd                         | esl.cfg       | Second Half config. <br /> Executed when match is live at second half.|
| mb_cfg_overtime                    | esl-ot.cfg    | Overtime config. <br /> Executed at overtime extras rounds.|
| mb_cfg_end                         | end.cfg       | End config. <br /> Executed right after match ends.|
</details>

<h3>Why CS 1.6 it's sucks</h3>
<p>
In first place, You're welcome!<br>  
Actually we have CS:GO a more modern version of the game.<br>  
But we can still have fun with a game like Counter-Strike that have been played more than 20 years.<br>  
I think i can help community to have more clean servers that want to play a competitive match.
</p>

<h3>I need Help!</h3>
<p>Visit your <a href="https://github.com/SmileYzn/MatchBot/wiki">wiki</a> to know what more you can do with Match Bot</p>
<p><a href="https://discord.gg/jFxKr9RSRc" target="_new">Discord Server</a></p>
<p>Before open an issue, make sure that you have updated ReHLDS with ReGameDLL_CS</p>
