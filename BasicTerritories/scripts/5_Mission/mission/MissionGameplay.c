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
	
	
	void RPCBasicTerritoriesModSettings( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		Param1< BasicTerritoriesConfig > data; 
		if ( !ctx.Read( data ) ) return;
		Print("[BASICT][Client] Received Config From Server");
		m_BasicTerritoriesConfig = data.param1;
	}
}