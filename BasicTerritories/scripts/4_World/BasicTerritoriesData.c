class BasicTerritoriesData
{
	protected static ref BasicTerritoriesData s_Instance;

	protected ref map<string, int> m_FlagpoleData;

	void BasicTerritoriesData() {
		m_FlagpoleData = new map<string, int>;
	}

	static BasicTerritoriesData GetInstance()
	{
		if (!s_Instance) s_Instance = new BasicTerritoriesData();
		return s_Instance;
	}

	bool IsAllowedToBuildOrJoinTerritory(string playerId) {
		Print("[BasicTerritoriesData] IsAllowedToBuildOrJoinTerritory");
		if(!playerId) return false;
		if(GetBasicTerritoriesConfig().MaxPlayerTerritoriesAsOwnerOrMember == 0) return true;
		if(m_FlagpoleData.Contains(playerId) && m_FlagpoleData.Get(playerId) <= GetBasicTerritoriesConfig().MaxPlayerTerritoriesAsOwnerOrMember) return true;
		return false;
	}

	void SyncTerritoryInformation(string playerId) {
		if (GetGame().IsServer()){
            autoptr array<Man> players = new array<Man>;
            GetGame().GetPlayers( players );
            for (int i = 0; i < players.Count(); i++){
                PlayerBase player = PlayerBase.Cast(players.Get(i));
                if (player.GetIdentity() && player.GetIdentity().GetId() == playerId){
					player.SyncTerritoryInformation();
                    return;
                }
            }
        }
	}

	void AddPlayer(string playerId) {
		Print("[BasicTerritoriesData] Adding player with id " + playerId);
		if(!playerId) return;
		if(!m_FlagpoleData.Contains(playerId)) m_FlagpoleData.Insert(playerId, 0)
		int i = m_FlagpoleData.Get(playerId);
		m_FlagpoleData.Insert(playerId, i++);
		SyncTerritoryInformation(playerId);
	}

	void AddPlayers(TStringArray playerIds) {
		Print("[BasicTerritoriesData] Adding players");
		if(!playerIds) return;
		foreach(string playerId: playerIds) {
			AddPlayer(playerId);
		}
	}

	void RemovePlayer(string playerId) {
		Print("[BasicTerritoriesData] Removing player with id " + playerId);
		if(!playerId) return;
		if(!m_FlagpoleData.Contains(playerId)) return;
		if(m_FlagpoleData.Get(playerId) < 2) m_FlagpoleData.Remove(playerId);
		int i = m_FlagpoleData.Get(playerId);
		m_FlagpoleData.Insert(playerId, i--);
		SyncTerritoryInformation(playerId);
	}

	void RemovePlayers(TStringArray playerIds) {
		Print("[BasicTerritoriesData] Removing players");
		if(!playerIds) return;
		foreach(string playerId: playerIds) {
			RemovePlayer(playerId);
		}
	}
}
