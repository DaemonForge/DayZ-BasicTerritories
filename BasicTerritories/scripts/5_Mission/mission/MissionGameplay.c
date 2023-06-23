modded class MissionGameplay extends MissionBase
{	
	void MissionGameplay() {
		GetRPCManager().AddRPC( "BASICT", "RPCBasicTerritoriesModSettings", this, SingeplayerExecutionType.Both );
	}
	
	override void OnMissionStart(){
		super.OnMissionStart();
		Print("[BASICT][Client] Requesting Config From Server");
		GetRPCManager().SendRPC("BASICT", "RPCBasicTerritoriesModSettings", new Param1< BasicTerritoriesConfig >( NULL ), true, NULL);
	}
	
	
	void RPCBasicTerritoriesModSettings( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
	{
		Param1< BasicTerritoriesConfig > data;

		if ( !ctx.Read( data ) ) return;

		Print("[BASICT][Client] Received Config From Server");

		m_BasicTerritoriesConfig = data.param1;
		
		#ifdef BASICMAP

		int i;

		if (GetBasicTerritoriesConfig().NoBuildZones )
		{
			BasicMap().ClearMarkers("TerritoryNoBuildZones");

			bool SomeNoBuildZonesOnTheMap = false;

			for (i = 0; i < GetBasicTerritoriesConfig().NoBuildZones.Count(); i++)
			{
				if (GetBasicTerritoriesConfig().NoBuildZones.Get(i) && GetBasicTerritoriesConfig().NoBuildZones.Get(i).DrawOnMap)
				{
					BasicMapCircleMarker tmpMarker = new BasicMapCircleMarker("",  GetBasicTerritoriesConfig().NoBuildZones.Get(i).GetPos(), "BasicTerritories\\images\\NoBuild.paa", {189, 38, 78},150);
					tmpMarker.SetRadius(GetBasicTerritoriesConfig().NoBuildZones.Get(i).R);
					tmpMarker.SetShowCenterMarker(true);
					tmpMarker.SetHideIntersects(true);
					tmpMarker.SetCanEdit(false);
					tmpMarker.SetGroup("TerritoryNoBuildZones");
					BasicMap().AddMarker("TerritoryNoBuildZones",tmpMarker);
					SomeNoBuildZonesOnTheMap = true;
				}
			}

			if (SomeNoBuildZonesOnTheMap)
			{
				BasicMap().RegisterGroup("TerritoryNoBuildZones", new BasicMapGroupMetaData("TerritoryNoBuildZones", "No Build Areas"),NULL);
			}

			bool SomeDoBuildZonesOnTheMap = false;

			for (i = 0; i < GetBasicTerritoriesConfig().DoBuildZones.Count(); i++)
			{
				if (GetBasicTerritoriesConfig().DoBuildZones.Get(i) && GetBasicTerritoriesConfig().DoBuildZones.Get(i).DrawOnMap)
				{
					BasicMapCircleMarker tmpMarker2 = new BasicMapCircleMarker("",  GetBasicTerritoriesConfig().DoBuildZones.Get(i).GetPos(), "BasicTerritories\\images\\Build.paa", {212, 175, 55},180);
					tmpMarker2.SetRadius(GetBasicTerritoriesConfig().DoBuildZones.Get(i).R);
					tmpMarker2.SetShowCenterMarker(true);
					tmpMarker2.SetHideIntersects(true);
					tmpMarker2.SetCanEdit(false);
					tmpMarker2.SetGroup("TerritoryDoBuildZones");
					BasicMap().AddMarker("TerritoryDoBuildZones",tmpMarker2);
					SomeDoBuildZonesOnTheMap = true;
				}
			}

			if (SomeDoBuildZonesOnTheMap)
			{
				BasicMap().RegisterGroup("TerritoryDoBuildZones", new BasicMapGroupMetaData("TerritoryDoBuildZones", "No Build Areas"),NULL);
			}

		}
		#endif
	}
}
