modded class ActionDeployObject : ActionContinuousBase
{			 
	protected int m_LastSync = 0;
	protected bool m_CanPlaceHere = false;
	protected vector m_LastCheckLocation = vector.Zero;
	
	#ifdef BASICMAP
	protected ref BasicTerritoryMapMarker BASICT_Marker;
	#endif
	
	void ~ActionDeployObject() {
		RemoveTheBasicMapMarker();
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (super.ActionCondition( player, target, item )){
			if (GetGame().IsServer()){
				return true;
			}
			PlayerBase thePlayer = PlayerBase.Cast(player);
			if (thePlayer && thePlayer.GetHologramLocal() && thePlayer.GetHologramLocal().GetProjectionEntity()) {
				
				if (vector.Distance(m_LastCheckLocation, thePlayer.GetHologramLocal().GetProjectionEntity().GetPosition()) > 0.4){
					#ifdef BASICMAP
					bool ShowTerritoryOnMap = BasicMap().ShowSelfOnMap();
					if (item && item.IsInherited(TerritoryFlagKit) && ShowTerritoryOnMap){
						if (!BASICT_Marker){
							Print("[BasicTerritory] [BasicMap] Creating Marker for TerritoryFlag");
							BASICT_Marker = new BasicTerritoryMapMarker("", thePlayer.GetHologramLocal().GetProjectionEntity().GetPosition());
							BASICT_Marker.SetRadius(GameConstants.REFRESHER_RADIUS);
							BasicMap().AddMarker("BasicTerritories", BASICT_Marker);
						} else if ( BASICT_Marker.GetPosition() != thePlayer.GetHologramLocal().GetProjectionEntity().GetPosition() ) {
							if (BASICT_Marker.GetPosition() == vector.Zero){
								BasicMap().AddMarker("BasicTerritories", BASICT_Marker);
							}
							BASICT_Marker.SetPosition(thePlayer.GetHologramLocal().GetProjectionEntity().GetPosition());
							BASICT_Marker.SetRadius(GameConstants.REFRESHER_RADIUS);
						}
					} else {
						if (BASICT_Marker && BASICT_Marker.GetPosition() != vector.Zero && ShowTerritoryOnMap){
							BasicMap().RemoveMarker("BasicTerritories", BASICT_Marker);
							BASICT_Marker.SetPosition(vector.Zero);
							BASICT_Marker.SetRadius(0);
						}
					}
					#endif
					string theGUID = "";
					if (thePlayer.GetIdentity()) {
						theGUID = thePlayer.GetIdentity().GetId();
					}
					return CanIPlaceHere(item, thePlayer.GetHologramLocal().GetProjectionEntity(), thePlayer.GetHologramLocal().GetProjectionEntity().GetPosition(), theGUID);
				} else {
					return m_CanPlaceHere;
				}
			} else {
				RemoveTheBasicMapMarker();
			}
			return true;
		} else {
			RemoveTheBasicMapMarker();
		}
		return false;
	}
	
	override void OnEndClient( ActionData action_data  )
	{
		super.OnEndClient( action_data );
	}
	
	void RemoveTheBasicMapMarker(){
	#ifdef BASICMAP
		if (GetGame() && GetGame().IsClient() && BASICT_Marker && BASICT_Marker.GetPosition() != vector.Zero ){
			Print("[BasicTerritory] [BasicMap] Removing Marker for TerritoryFlag");
			BasicMap().RemoveMarker(BasicMap().CLIENT_KEY , BASICT_Marker);
			BASICT_Marker.SetPosition(vector.Zero);
			BASICT_Marker.SetRadius(0);
		}
	#endif
	}
	
	protected bool CanIPlaceHere(EntityAI kit, EntityAI item, vector pos, string GUID = ""){
		int curTime  = GetGame().GetTime();
		m_LastCheckLocation = pos;
		if (pos == vector.Zero || !item || !kit){
			m_CanPlaceHere = false;
			return m_CanPlaceHere;
		} else if (!GetBasicTerritoriesConfig().CanBuildHere(pos, item.GetType()) || !GetBasicTerritoriesConfig().CanBuildHere(pos, kit.GetType()) ){
			GetBasicTerritoriesConfig().SendNotification( GetBasicTerritoriesConfig().NoBuildZoneMessage, TerritoryIcons.NoBuildZone);
			m_CanPlaceHere = false;
			return m_CanPlaceHere;
		} else if ( GetBasicTerritoriesConfig().IsInWhiteList(item.GetType()) || GetBasicTerritoriesConfig().IsInWhiteList(kit.GetType()) ){
			m_CanPlaceHere = true;
			return m_CanPlaceHere;
		} else {
			array<Object> objects = new array<Object>;
			array<CargoBase> proxyCargos = new array<CargoBase>;
			float theRadius = GameConstants.REFRESHER_RADIUS;
			if ( kit.IsInherited(TerritoryFlagKit) ){
				theRadius = GameConstants.REFRESHER_RADIUS * 2;
			}
			GetGame().GetObjectsAtPosition(pos, theRadius, objects, proxyCargos);
			TerritoryFlag theFlag;
			if (objects){
				for (int i = 0; i < objects.Count(); i++ ){
					if ( Class.CastTo( theFlag, objects.Get(i) ) ){
						if ( kit.IsInherited(TerritoryFlagKit) ){
							GetBasicTerritoriesConfig().SendNotification(GetBasicTerritoriesConfig().TerritoryConflictMessage, TerritoryIcons.TerritoryConflict);
							#ifdef BASICMAP
							if (BASICT_Marker){
								BASICT_Marker.SetOverLaping(true);
							}
							#endif
							m_CanPlaceHere = false;
							return m_CanPlaceHere;
						}
						if (theFlag.HasRaisedFlag()){
							if (m_LastSync < curTime){
								m_LastSync = curTime + 60000;
								theFlag.SyncTerritory();
							}
							m_CanPlaceHere = theFlag.CheckPlayerPermission(GUID, TerritoryPerm.DEPLOY);
							if (!m_CanPlaceHere){
								GetBasicTerritoriesConfig().SendNotification(GetBasicTerritoriesConfig().WithinTerritoryWarning, TerritoryIcons.WithinTerritory);
							}
							return m_CanPlaceHere;
						}
					}
				}
			} 
			if (kit && kit.IsInherited(TerritoryFlagKit) ){
				#ifdef BASICMAP
				if (BASICT_Marker){
					BASICT_Marker.SetOverLaping(false);
				}
				#endif
				m_CanPlaceHere = true;
				return m_CanPlaceHere;
			}
		}
		if (GetBasicTerritoriesConfig().RequireTerritory){
			
			GetBasicTerritoriesConfig().SendNotification(GetBasicTerritoriesConfig().TerritoryRequiredWarningMessage, TerritoryIcons.TerritoryRequiredWarning);
			m_CanPlaceHere = false;
			
		} else {
			string DeSpawnWarningMessage = GetBasicTerritoriesConfig().DeSpawnWarningMessage;
			int LifeTime = 0;
			int itemLifetime = 0;
			ItemBase theItem;
			ItemBase theKit;
			string ItemName = kit.GetDisplayName();
			
			theItem = ItemBase.Cast(item);
			itemLifetime = GetBasicTerritoriesConfig().GetKitLifeTime(theItem.GetType());
			if (itemLifetime <= 0 && theItem){
				itemLifetime =  theItem.GetTSyncedLifeTime();
			}
			theKit = ItemBase.Cast(kit);
			LifeTime = GetBasicTerritoriesConfig().GetKitLifeTime(theKit.GetType());
			if (LifeTime <= 0 && theKit){
				LifeTime =  theKit.GetTSyncedLifeTime();
			}
			//Print("The Item: " + theItem.GetType() + " TSynced: " + theItem.GetTSyncedLifeTime()  + " TheKit: " + theKit.GetType() + " TSynced: " + theKit.GetTSyncedLifeTime() );
			if (itemLifetime > LifeTime){
				LifeTime = itemLifetime;
				ItemName = item.GetDisplayName();
			} 
			GetBasicTerritoriesConfig().GetKitLifeTime(kit.GetType());
			string NiceExpireTime = GetBasicTerritoriesConfig().NiceExpireTime(LifeTime);
			if (NiceExpireTime != ""){
				DeSpawnWarningMessage.Replace("$LIFETIME$", NiceExpireTime);
				DeSpawnWarningMessage.Replace("$ITEMNAME$", ItemName); 
				GetBasicTerritoriesConfig().SendNotification(DeSpawnWarningMessage, TerritoryIcons.DeSpawnWarning);
			}
			m_CanPlaceHere = true;
		}
		return m_CanPlaceHere;
	}
};